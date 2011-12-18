/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : pin_ops.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>
#include <mach/platform.h>

#include <mach/gpio_v2.h>
#include <mach/script_v2.h>


#define CSP_OSAL_PHY_2_VIRT(phys, size) SW_VA_PORTC_IO_BASE
#define CSP_OSAL_MALLOC(size) kmalloc((size), GFP_ATOMIC)
#define CSP_OSAL_FREE(ptr) kfree((ptr))

#define	CSP_PIN_PHY_ADDR_BASE	SW_PA_PORTC_IO_BASE
#define	CSP_PIN_PHY_ADDR_SIZE	0x1000

u32     gpio_g_pioMemBase = 0;
#define PIOC_REGS_BASE gpio_g_pioMemBase

extern char sys_cofig_data[];
extern char sys_cofig_data_end[];
#define __REG(x)                        (*(volatile unsigned int *)(x))

#define PIO_REG_CFG(n, i)               ((volatile unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00))
#define PIO_REG_DLEVEL(n, i)            ((volatile unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14))
#define PIO_REG_PULL(n, i)              ((volatile unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C))
#define PIO_REG_DATA(n) 	            ((volatile unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24 + 0x10))

#define PIO_REG_CFG_VALUE(n, i)          __REG( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00)
#define PIO_REG_DLEVEL_VALUE(n, i)       __REG( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14)
#define PIO_REG_PULL_VALUE(n, i)         __REG( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C)
#define PIO_REG_DATA_VALUE(n) 	         __REG( PIOC_REGS_BASE + ((n)-1)*0x24 + 0x10)
//#define FPGA_RUNTIME_ENV

typedef struct
{
	int mul_sel;
	int pull;
	int drv_level;
	int data;
}
gpio_status_set_t;

typedef struct
{
	char    gpio_name[32];
	int port;
	int port_num;
	gpio_status_set_t user_gpio_status;
	gpio_status_set_t hardware_gpio_status;
}
system_gpio_set_t;

/*
****************************************************************************************************
*
*             CSP_PIN_init
*
*  Description:
*       init
*
*  Parameters:
*		p_sys_srv	:	��csp������ϵͳ����
*		p_cfg		:	pin������
*  Return value:
*		EGPIO_SUCCESS/EGPIO_FAIL
****************************************************************************************************
*/
int gpio_init(void)          //gpio��ʼ�������ӿ�
{
	printk("Init eGon pin module V2.0\n");
	gpio_g_pioMemBase = (u32)CSP_OSAL_PHY_2_VIRT(CSP_PIN_PHY_ADDR_BASE , CSP_PIN_PHY_ADDR_SIZE);
    #ifdef FPGA_RUNTIME_ENV
        return script_parser_init((char *)(sys_cofig_data));
    #else
        return script_parser_init((char *)__va(CONFIG_SW_SYSMEM_RESERVED_BASE));
    #endif
}
fs_initcall(gpio_init);
/*
****************************************************************************************************
*
*             CSP_PIN_exit
*
*  Description:
*       exit
*
*  Parameters:
*
*  Return value:
*		EGPIO_SUCCESS/EGPIO_FAIL
****************************************************************************************************
*/
__s32 gpio_exit(void)		    //gpio�˳������ӿ�
{
	return 0;
}
/*
************************************************************************************************************
*
*                                             CSP_GPIO_Request
*
*    �������ƣ�
*
*    �����б�gpio_list      ��������õ���GPIO���ݵ����飬GPIO��ֱ��ʹ���������
*
*			   group_count_max  ����ĳ�Ա������GPIO�趨��ʱ�򣬽�������GPIO��󲻳������ֵ
*
*    ����ֵ  ��
*
*    ˵��    ����ʱû������ͻ���
*
*
************************************************************************************************************
*/
u32 gpio_request(user_gpio_set_t *gpio_list, __u32 group_count_max)  //�豸����GPIO�����ӿ�
{
	char               *user_gpio_buf;                                        //����char��������
	system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;					  //user_gpio_set���������ڴ�ľ��
	user_gpio_set_t  *tmp_user_gpio_data;
	__u32				real_gpio_count = 0, first_port;                      //����������Ч��GPIO�ĸ���
	__u32               tmp_group_func_data = 0;
	__u32               tmp_group_pull_data = 0;
	__u32               tmp_group_dlevel_data = 0;
	__u32               tmp_group_data_data = 0;
	__u32               func_change = 0, pull_change = 0;
	__u32               dlevel_change = 0, data_change = 0;
	volatile __u32  *tmp_group_func_addr = NULL, *tmp_group_pull_addr = NULL;
	volatile __u32  *tmp_group_dlevel_addr = NULL, *tmp_group_data_addr = NULL;
	__u32  port, port_num, port_num_func, port_num_pull;
	__u32  pre_port = 0x7fffffff, pre_port_num_func = 0x7fffffff;
	__u32  pre_port_num_pull = 0x7fffffff;
	__s32               i, tmp_val;

	//����û����������ݵ���Ч��
	if((!gpio_list) || (!group_count_max))
	{
		return (u32)0;
	}
	//�鿴�û�����������Ч�ĸ���
	for(i = 0; i < group_count_max; i++)
	{
		tmp_user_gpio_data = gpio_list + i;                 //gpio_set����ָ��ÿ��GPIO�����Ա
		if(!tmp_user_gpio_data->port)
		{
			continue;
		}
		real_gpio_count ++;
	}

	//printk("to malloc space for pin \n");
	//������Ч��GPIO��������ռ�
	user_gpio_buf = (char *)CSP_OSAL_MALLOC(16 + sizeof(system_gpio_set_t) * real_gpio_count);   //�����ڴ棬������16���ֽڣ����ڴ��GPIO��������Ϣ
	if(!user_gpio_buf)
	{
		return (u32)0;
	}
	//add by young for debug
	//printk("gpio module: alloc user_gpio_buf addr is:  0x%x \n", (unsigned int)user_gpio_buf);
	memset(user_gpio_buf, 0, 16 + sizeof(system_gpio_set_t) * real_gpio_count);         //����ȫ������
	*(int *)user_gpio_buf = real_gpio_count;                                           //������Ч��GPIO����
	user_gpio_set = (system_gpio_set_t *)(user_gpio_buf + 16);                         //ָ���һ���ṹ��
	//׼����һ��GPIO����
	for(first_port = 0; first_port < group_count_max; first_port++)
	{
		tmp_user_gpio_data = gpio_list + first_port;
		port     = tmp_user_gpio_data->port;                         //�����˿���ֵ
		port_num = tmp_user_gpio_data->port_num;                     //�����˿��е�ĳһ��GPIO
		if(!port)
		{
			continue;
		}
		port_num_func = (port_num >> 3);
		port_num_pull = (port_num >> 4);

		tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
		tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);  //����pull�Ĵ���
		tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull);//����level�Ĵ���
		tmp_group_data_addr    = PIO_REG_DATA(port);                 //����data�Ĵ���

		tmp_group_func_data    = *tmp_group_func_addr;
		tmp_group_pull_data    = *tmp_group_pull_addr;
		tmp_group_dlevel_data  = *tmp_group_dlevel_addr;
		tmp_group_data_data    = *tmp_group_data_addr;
		break;
	}
	if(first_port >= group_count_max)
	{
		return 0;
	}
	//�����û�����
	for(i = first_port; i < group_count_max; i++)
	{
		tmp_sys_gpio_data  = user_gpio_set + i;             //tmp_sys_gpio_dataָ�������GPIO�ռ�
		tmp_user_gpio_data = gpio_list + i;                 //gpio_set����ָ���û���ÿ��GPIO�����Ա
		port     = tmp_user_gpio_data->port;                //�����˿���ֵ
		port_num = tmp_user_gpio_data->port_num;            //�����˿��е�ĳһ��GPIO
		if(!port)
		{
			continue;
		}
		//��ʼ�����û�����
		strcpy(tmp_sys_gpio_data->gpio_name, tmp_user_gpio_data->gpio_name);
		tmp_sys_gpio_data->port                       = port;
		tmp_sys_gpio_data->port_num                   = port_num;
		tmp_sys_gpio_data->user_gpio_status.mul_sel   = tmp_user_gpio_data->mul_sel;
		tmp_sys_gpio_data->user_gpio_status.pull      = tmp_user_gpio_data->pull;
		tmp_sys_gpio_data->user_gpio_status.drv_level = tmp_user_gpio_data->drv_level;
		tmp_sys_gpio_data->user_gpio_status.data      = tmp_user_gpio_data->data;

		port_num_func = (port_num >> 3);
		port_num_pull = (port_num >> 4);

		if((port_num_pull != pre_port_num_pull) || (port != pre_port))    //������ֵ�ǰ���ŵĶ˿ڲ�һ�£��������ڵ�pull�Ĵ�����һ��
		{
			if(func_change)
			{
				*tmp_group_func_addr   = tmp_group_func_data;    //��д���ܼĴ���
				func_change = 0;
			}
			if(pull_change)
			{
				pull_change = 0;
				*tmp_group_pull_addr   = tmp_group_pull_data;    //��дpull�Ĵ���
			}
			if(dlevel_change)
			{
				dlevel_change = 0;
				*tmp_group_dlevel_addr = tmp_group_dlevel_data;  //��дdriver level�Ĵ���
			}
			if(data_change)
			{
				data_change = 0;
				*tmp_group_data_addr   = tmp_group_data_data;    //��д
			}

			tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
			tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);   //����pull�Ĵ���
			tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull); //����level�Ĵ���
			tmp_group_data_addr    = PIO_REG_DATA(port);                  //����data�Ĵ���
                    
			tmp_group_func_data    = *tmp_group_func_addr;
			tmp_group_pull_data    = *tmp_group_pull_addr;
			tmp_group_dlevel_data  = *tmp_group_dlevel_addr;
			tmp_group_data_data    = *tmp_group_data_addr;
			
		}
		else if(pre_port_num_func != port_num_func)                       //������ֵ�ǰ���ŵĹ��ܼĴ�����һ��
		{
			*tmp_group_func_addr   = tmp_group_func_data;    //��ֻ��д���ܼĴ���
			tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
			
			tmp_group_func_data    = *tmp_group_func_addr;
		}
		//���浱ǰӲ���Ĵ�������
		pre_port_num_pull = port_num_pull;                      //���õ�ǰGPIO��Ϊǰһ��GPIO
		pre_port_num_func = port_num_func;
		pre_port          = port;

		//���¹��ܼĴ���
		if(tmp_user_gpio_data->mul_sel >= 0)
		{
			tmp_val = (port_num - (port_num_func<<3)) << 2;
			tmp_sys_gpio_data->hardware_gpio_status.mul_sel = (tmp_group_func_data >> tmp_val) & 0x07;
			tmp_group_func_data &= ~(                              0x07  << tmp_val);
			tmp_group_func_data |=  (tmp_user_gpio_data->mul_sel & 0x07) << tmp_val;
			func_change = 1;
		}
		//����pull��ֵ�����Ƿ����pull�Ĵ���

		tmp_val = (port_num - (port_num_pull<<4)) << 1;

		if(tmp_user_gpio_data->pull >= 0)
		{
			tmp_sys_gpio_data->hardware_gpio_status.pull = (tmp_group_pull_data >> tmp_val) & 0x03;
			if(tmp_user_gpio_data->pull >= 0)
			{
				tmp_group_pull_data &= ~(                           0x03  << tmp_val);
				tmp_group_pull_data |=  (tmp_user_gpio_data->pull & 0x03) << tmp_val;
				pull_change = 1;
			}
		}
		//����driver level��ֵ�����Ƿ����driver level�Ĵ���
		if(tmp_user_gpio_data->pull >= 0)
		{
			tmp_sys_gpio_data->hardware_gpio_status.drv_level = (tmp_group_dlevel_data >> tmp_val) & 0x03;
			if(tmp_user_gpio_data->drv_level >= 0)
			{
				tmp_group_dlevel_data &= ~(                                0x03  << tmp_val);
				tmp_group_dlevel_data |=  (tmp_user_gpio_data->drv_level & 0x03) << tmp_val;
				dlevel_change = 1;
			}
		}
		//�����û����룬�Լ����ܷ�������Ƿ����data�Ĵ���
		if(tmp_user_gpio_data->mul_sel == 1)
		{
			if(tmp_user_gpio_data->data >= 0)
			{
				tmp_val = tmp_user_gpio_data->data;
				tmp_val &= 1;
				tmp_group_data_data &= ~(1 << port_num);
				tmp_group_data_data |= tmp_val << port_num;
				data_change = 1;
			}
		}
	}
	//forѭ��������������ڻ�û�л�д�ļĴ���������д�ص�Ӳ������
	if(tmp_group_func_addr)                         //ֻҪ���¹��Ĵ�����ַ���Ϳ��Զ�Ӳ����ֵ
	{                                               //��ô�����е�ֵȫ����д��Ӳ���Ĵ���
		*tmp_group_func_addr   = tmp_group_func_data;       //��д���ܼĴ���
		if(pull_change)
		{
			*tmp_group_pull_addr   = tmp_group_pull_data;    //��дpull�Ĵ���
		}
		if(dlevel_change)
		{
			*tmp_group_dlevel_addr = tmp_group_dlevel_data;  //��дdriver level�Ĵ���
		}
		if(data_change)
		{
			*tmp_group_data_addr   = tmp_group_data_data;    //��дdata�Ĵ���
		}
	}
	
	return (u32)user_gpio_buf;
}
EXPORT_SYMBOL_GPL(gpio_request);
/*
************************************************************************************************************
*
*                                             CSP_GPIO_Request_EX
*
*    �������ƣ�
*
*    ����˵��: main_name   �������������ƣ�ƥ��ģ��(��������)
*
*			   sub_name    �������Ӽ����ƣ�����ǿգ���ʾȫ��������Ѱ�ҵ�ƥ��ĵ���GPIO
*
*    ����ֵ  ��0 :    err
*              other: success
*
*    ˵��    ����ʱû������ͻ���
*
*
************************************************************************************************************
*/
u32 gpio_request_ex(char *main_name, const char *sub_name)  //�豸����GPIO������չ�ӿ�
{
	user_gpio_set_t    *gpio_list=NULL;
	user_gpio_set_t     one_gpio;
   	__u32               gpio_handle;
	__s32               gpio_count;

	//printk("enter gpio_request_ex func \n");
	if(!sub_name){
    		gpio_count = script_parser_mainkey_get_gpio_count(main_name);
    		if(gpio_count <= 0)
    		{
                printk("err: gpio count < =0 ,gpio_count is: %d \n", gpio_count);
        		return 0;
    		}   		
    		gpio_list = (user_gpio_set_t *)CSP_OSAL_MALLOC(sizeof(system_gpio_set_t) * gpio_count); //����һƬ��ʱ�ڴ棬���ڱ����û�����
    		if(!gpio_list){
			printk("malloc gpio_list error \n");
        		return 0;
    		}
		if(!script_parser_mainkey_get_gpio_cfg(main_name,gpio_list,gpio_count)){
			gpio_handle = gpio_request(gpio_list, gpio_count);
			CSP_OSAL_FREE(gpio_list);
    			
		}else{
			return 0;
		}
    	}else{
    		if(script_parser_fetch((char *)main_name, (char *)sub_name, (int *)&one_gpio, (sizeof(user_gpio_set_t) >> 2)) < 0){
			printk("script parser fetch err. \n");
			return 0;
        	}
        	
        	gpio_handle = gpio_request(&one_gpio, 1);        	
    	}

        return gpio_handle;
}
EXPORT_SYMBOL(gpio_request_ex);

/*
****************************************************************************************************
*
*             CSP_PIN_DEV_release
*
*  Description:
*       �ͷ�ĳ�߼��豸��pin
*
*  Parameters:
* 		p_handler	:	handler
*       if_release_to_default_status : �Ƿ��ͷŵ�ԭʼ״̬(�Ĵ���ԭ��״̬)
*
*  Return value:
*		EGPIO_SUCCESS/EGPIO_FAIL
****************************************************************************************************
*/
__s32 gpio_release(u32 p_handler, __s32 if_release_to_default_status)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max, first_port;                    //���GPIO����
	system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
	__u32               tmp_group_func_data = 0;
	__u32               tmp_group_pull_data = 0;
	__u32               tmp_group_dlevel_data = 0;
	volatile __u32     *tmp_group_func_addr = NULL,   *tmp_group_pull_addr = NULL;
	volatile __u32     *tmp_group_dlevel_addr = NULL;
	__u32               port, port_num, port_num_pull, port_num_func;
	__u32               pre_port = 0x7fffffff, pre_port_num_func = 0x7fffffff, pre_port_num_pull = 0x7fffffff;
	__u32               i, tmp_val;

	//��鴫���ľ������Ч��
	if(!p_handler)
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	if(!group_count_max)
	{
		return EGPIO_FAIL;
	}
	if(if_release_to_default_status == 2)
	{
		//printk("gpio module :  release p_handler = %x\n",p_handler);
		CSP_OSAL_FREE((char *)p_handler);
		
		return EGPIO_SUCCESS;
	}
	user_gpio_set = (system_gpio_set_t *)(tmp_buf + 16);
	//��ȡ�û�����
	for(first_port = 0; first_port < group_count_max; first_port++)
	{
		tmp_sys_gpio_data  = user_gpio_set + first_port;
		port     = tmp_sys_gpio_data->port;                 //�����˿���ֵ
		port_num = tmp_sys_gpio_data->port_num;             //�����˿��е�ĳһ��GPIO
		if(!port)
		{
			continue;
		}
		port_num_func = (port_num >> 3);
		port_num_pull = (port_num >> 4);

		tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
		tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);  //����pull�Ĵ���
		tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull);//����level�Ĵ���

		tmp_group_func_data    = *tmp_group_func_addr;
		tmp_group_pull_data    = *tmp_group_pull_addr;
		tmp_group_dlevel_data  = *tmp_group_dlevel_addr;
		break;
	}
	if(first_port >= group_count_max)
	{
		return 0;
	}
	for(i = first_port; i < group_count_max; i++)
	{
		tmp_sys_gpio_data  = user_gpio_set + i;             //tmp_sys_gpio_dataָ�������GPIO�ռ�
		port     = tmp_sys_gpio_data->port;                 //�����˿���ֵ
		port_num = tmp_sys_gpio_data->port_num;             //�����˿��е�ĳһ��GPIO

		port_num_func = (port_num >> 3);
		port_num_pull = (port_num >> 4);

		if((port_num_pull != pre_port_num_pull) || (port != pre_port))    //������ֵ�ǰ���ŵĶ˿ڲ�һ�£��������ڵ�pull�Ĵ�����һ��
		{
			*tmp_group_func_addr   = tmp_group_func_data;    //��д���ܼĴ���
			*tmp_group_pull_addr   = tmp_group_pull_data;    //��дpull�Ĵ���
			*tmp_group_dlevel_addr = tmp_group_dlevel_data;  //��дdriver level�Ĵ���

			tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
			tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);   //����pull�Ĵ���
			tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull); //����level�Ĵ���

			tmp_group_func_data    = *tmp_group_func_addr;
			tmp_group_pull_data    = *tmp_group_pull_addr;
			tmp_group_dlevel_data  = *tmp_group_dlevel_addr;
		}
		else if(pre_port_num_func != port_num_func)                       //������ֵ�ǰ���ŵĹ��ܼĴ�����һ��
		{
			*tmp_group_func_addr   = tmp_group_func_data;    			 //��ֻ��д���ܼĴ���
			tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
			tmp_group_func_data    = *tmp_group_func_addr;
		}

		pre_port_num_pull = port_num_pull;
		pre_port_num_func = port_num_func;
		pre_port          = port;
		//���¹��ܼĴ���
		tmp_group_func_data &= ~(0x07 << ((port_num - (port_num_func<<3)) << 2));
		//����pull״̬�Ĵ���
		tmp_val              =  (port_num - (port_num_pull<<4)) << 1;
		tmp_group_pull_data &= ~(0x03  << tmp_val);
		tmp_group_pull_data |= (tmp_sys_gpio_data->hardware_gpio_status.pull & 0x03) << tmp_val;
		//����driver״̬�Ĵ���
		tmp_val              =  (port_num - (port_num_pull<<4)) << 1;
		tmp_group_dlevel_data &= ~(0x03  << tmp_val);
		tmp_group_dlevel_data |= (tmp_sys_gpio_data->hardware_gpio_status.drv_level & 0x03) << tmp_val;
	}
	if(tmp_group_func_addr)                              //ֻҪ���¹��Ĵ�����ַ���Ϳ��Զ�Ӳ����ֵ
	{                                                    //��ô�����е�ֵȫ����д��Ӳ���Ĵ���
		*tmp_group_func_addr   = tmp_group_func_data;    //��д���ܼĴ���
	}
	if(tmp_group_pull_addr)
	{
		*tmp_group_pull_addr   = tmp_group_pull_data;
	}
	if(tmp_group_dlevel_addr)
	{
		*tmp_group_dlevel_addr = tmp_group_dlevel_data;
	}

	//free(p_handler);                                //�ͷ��ڴ�
	//printk("gpio module :  release p_handler = %x\n",p_handler);
	CSP_OSAL_FREE((char *)p_handler);

	return EGPIO_SUCCESS;
}
EXPORT_SYMBOL(gpio_release);
/*
**********************************************************************************************************************
*                                               CSP_PIN_Get_All_Gpio_Status
*
* Description:
*				��ȡ�û������������GPIO��״̬
* Arguments  :
*		p_handler	:	handler
*		gpio_status	:	�����û����ݵ�����
*		gpio_count_max	:	��������������������Խ��
*       if_get_user_set_flag   :   ��ȡ��־����ʾ��ȡ�û��趨���ݻ�����ʵ������
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  gpio_get_all_pin_status(u32 p_handler, user_gpio_set_t *gpio_status, __u32 gpio_count_max, __u32 if_get_from_hardware)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max, first_port;                    //���GPIO����
	system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
	user_gpio_set_t  *script_gpio;
	__u32               port_num_func, port_num_pull;
	volatile __u32     *tmp_group_func_addr = NULL, *tmp_group_pull_addr;
	volatile __u32     *tmp_group_data_addr, *tmp_group_dlevel_addr;
	__u32               port, port_num;
	__u32               pre_port = 0x7fffffff, pre_port_num_func = 0x7fffffff, pre_port_num_pull = 0x7fffffff;
	__u32               i;

	//��鴫���ľ������Ч��
	if((!p_handler) || (!gpio_status))
	{
		return EGPIO_FAIL;
	}
	if(gpio_count_max <= 0)
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	if(group_count_max <= 0)
	{
		return EGPIO_FAIL;
	}
	user_gpio_set = (system_gpio_set_t *)(tmp_buf + 16);
	if(group_count_max > gpio_count_max)
	{
		group_count_max = gpio_count_max;
	}
	//��ȡ�û�����
	//��ʾ��ȡ�û�����������
	if(!if_get_from_hardware)
	{
		for(i = 0; i < group_count_max; i++)
		{
			tmp_sys_gpio_data = user_gpio_set + i;             //tmp_sys_gpio_dataָ�������GPIO�ռ�
			script_gpio       = gpio_status + i;               //script_gpioָ���û������Ŀռ�
			
			script_gpio->port      = tmp_sys_gpio_data->port;                       //����port����
			script_gpio->port_num  = tmp_sys_gpio_data->port_num;                   //����port_num����
			script_gpio->pull      = tmp_sys_gpio_data->user_gpio_status.pull;      //����pull����
			script_gpio->mul_sel   = tmp_sys_gpio_data->user_gpio_status.mul_sel;   //������������
			script_gpio->drv_level = tmp_sys_gpio_data->user_gpio_status.drv_level; //����������������
			script_gpio->data      = tmp_sys_gpio_data->user_gpio_status.data;      //����data����
			strcpy(script_gpio->gpio_name, tmp_sys_gpio_data->gpio_name);
		}
	}
	else
	{
		for(first_port = 0; first_port < group_count_max; first_port++)
		{
			tmp_sys_gpio_data  = user_gpio_set + first_port;
			port     = tmp_sys_gpio_data->port;               //�����˿���ֵ
			port_num = tmp_sys_gpio_data->port_num;           //�����˿��е�ĳһ��GPIO
			
			if(!port)
			{
				continue;
			}
			port_num_func = (port_num >> 3);
			port_num_pull = (port_num >> 4);
			tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
			tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);   //����pull�Ĵ���
			tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull); //����level�Ĵ���
			tmp_group_data_addr    = PIO_REG_DATA(port);                  //����data�Ĵ���
			break;
		}
		if(first_port >= group_count_max)
		{
			return 0;
		}
		for(i = first_port; i < group_count_max; i++)
		{
			tmp_sys_gpio_data = user_gpio_set + i;             //tmp_sys_gpio_dataָ�������GPIO�ռ�
			script_gpio       = gpio_status + i;               //script_gpioָ���û������Ŀռ�

			port     = tmp_sys_gpio_data->port;                //�����˿���ֵ
			port_num = tmp_sys_gpio_data->port_num;            //�����˿��е�ĳһ��GPIO
			
			script_gpio->port = port;                          //����port����
			script_gpio->port_num  = port_num;                 //����port_num����
			strcpy(script_gpio->gpio_name, tmp_sys_gpio_data->gpio_name);
			
			port_num_func = (port_num >> 3);
			port_num_pull = (port_num >> 4);
			
			if((port_num_pull != pre_port_num_pull) || (port != pre_port))    //������ֵ�ǰ���ŵĶ˿ڲ�һ�£��������ڵ�pull�Ĵ�����һ��
			{
				tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
				tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);   //����pull�Ĵ���
				tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull); //����level�Ĵ���
				tmp_group_data_addr    = PIO_REG_DATA(port);                  //����data�Ĵ���
			}
			else if(pre_port_num_func != port_num_func)                       //������ֵ�ǰ���ŵĹ��ܼĴ�����һ��
			{
				tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //���¹��ܼĴ�����ַ
			}

			pre_port_num_pull = port_num_pull;
			pre_port_num_func = port_num_func;
			pre_port          = port;
			//���û��ؼ���ֵ
			script_gpio->pull      = (*tmp_group_pull_addr   >> ((port_num - (port_num_pull<<4))<<1)) & 0x03;    //����pull����
			script_gpio->drv_level = (*tmp_group_dlevel_addr >> ((port_num - (port_num_pull<<4))<<1)) & 0x03;    //������������
			script_gpio->mul_sel   = (*tmp_group_func_addr   >> ((port_num - (port_num_func<<3))<<2)) & 0x07;    //������������
			if(script_gpio->mul_sel <= 1)
			{
				script_gpio->data  = (*tmp_group_data_addr   >>   port_num) & 0x01;                              //����data����
			}
			else
			{
				script_gpio->data = -1;
			}
		}
	}

	return EGPIO_SUCCESS;
}
EXPORT_SYMBOL(gpio_get_all_pin_status);
/*
**********************************************************************************************************************
*                                               CSP_GPIO_Get_One_PIN_Status
*
* Description:
*				��ȡ�û������������GPIO��״̬
* Arguments  :
*		p_handler	:	handler
*		gpio_status	:	�����û����ݵ�����
*		gpio_name	:	Ҫ������GPIO������
*       if_get_user_set_flag   :   ��ȡ��־����ʾ��ȡ�û��趨���ݻ�����ʵ������
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  gpio_get_one_pin_status(u32 p_handler, user_gpio_set_t *gpio_status, const char *gpio_name, __u32 if_get_from_hardware)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max;                                //���GPIO����
	system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
	__u32               port_num_func, port_num_pull;
	__u32               port, port_num;
	__u32               i, tmp_val1, tmp_val2;
	
	//��鴫���ľ������Ч��
	if((!p_handler) || (!gpio_status))
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	if(group_count_max <= 0)
	{
		return EGPIO_FAIL;
	}
	else if((group_count_max > 1) && (!gpio_name))
	{
		return EGPIO_FAIL;
	}
	user_gpio_set = (system_gpio_set_t *)(tmp_buf + 16);
	//��ȡ�û�����
	//��ʾ��ȡ�û�����������
	for(i = 0; i < group_count_max; i++)
	{
		tmp_sys_gpio_data = user_gpio_set + i;             //tmp_sys_gpio_dataָ�������GPIO�ռ�
		if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
		{
			continue;
		}
		strcpy(gpio_status->gpio_name, tmp_sys_gpio_data->gpio_name);
		port                   = tmp_sys_gpio_data->port;
		port_num               = tmp_sys_gpio_data->port_num;
		gpio_status->port      = port;                                              //����port����
		gpio_status->port_num  = port_num;                                          //����port_num����
		
		if(!if_get_from_hardware)                                                    //��ǰҪ������û���Ƶ�����
		{
			gpio_status->mul_sel   = tmp_sys_gpio_data->user_gpio_status.mul_sel;   //���û����������ж�����������
			gpio_status->pull      = tmp_sys_gpio_data->user_gpio_status.pull;      //���û����������ж���pull����
			gpio_status->drv_level = tmp_sys_gpio_data->user_gpio_status.drv_level; //���û����������ж���������������
			gpio_status->data      = tmp_sys_gpio_data->user_gpio_status.data;      //���û����������ж���data����
        }
        else                                                                        //��ǰ�����Ĵ���ʵ�ʵĲ���
        {
		port_num_func = (port_num >> 3);
		port_num_pull = (port_num >> 4);
		
		tmp_val1 = ((port_num - (port_num_func << 3)) << 2);
		tmp_val2 = ((port_num - (port_num_pull << 4)) << 1);
		gpio_status->mul_sel   = (PIO_REG_CFG_VALUE(port, port_num_func)>>tmp_val1) & 0x07;       //��Ӳ���ж������ܼĴ���
		gpio_status->pull      = (PIO_REG_PULL_VALUE(port, port_num_pull)>>tmp_val2) & 0x03;      //��Ӳ���ж���pull�Ĵ���
		gpio_status->drv_level = (PIO_REG_DLEVEL_VALUE(port, port_num_pull)>>tmp_val2) & 0x03;    //��Ӳ���ж���level�Ĵ���
		if(gpio_status->mul_sel <= 1)
		{
			gpio_status->data = (PIO_REG_DATA_VALUE(port) >> port_num) & 0x01;                     //��Ӳ���ж���data�Ĵ���
		}
		else
		{
			gpio_status->data = -1;
		}
        }
		
		break;
	}

	return EGPIO_SUCCESS;
}
EXPORT_SYMBOL(gpio_get_one_pin_status);
/*
**********************************************************************************************************************
*                                               CSP_PIN_Set_One_Gpio_Status
*
* Description:
*				��ȡ�û��������GPIO��ĳһ����״̬
* Arguments  :
*		p_handler	:	handler
*		gpio_status	:	�����û����ݵ�����
*		gpio_name	:	Ҫ������GPIO������
*       if_get_user_set_flag   :   ��ȡ��־����ʾ��ȡ�û��趨���ݻ�����ʵ������
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/

__s32  gpio_set_one_pin_status(u32 p_handler, user_gpio_set_t *gpio_status, const char *gpio_name, __u32 if_set_to_current_input_status)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max;                                //���GPIO����
	system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
	user_gpio_set_t     script_gpio;
	volatile __u32     *tmp_addr;
	__u32               port_num_func, port_num_pull;
	__u32               port, port_num;
	__u32               i, reg_val, tmp_val;

	//��鴫���ľ������Ч��
	if((!p_handler) || (!gpio_name))
	{
		return EGPIO_FAIL;
	}
	if((if_set_to_current_input_status) && (!gpio_status))
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	if(group_count_max <= 0)
	{
		return EGPIO_FAIL;
	}
	user_gpio_set = (system_gpio_set_t *)(tmp_buf + 16);
	//��ȡ�û�����
	//��ʾ��ȡ�û�����������
    for(i = 0; i < group_count_max; i++)
    {
        tmp_sys_gpio_data = user_gpio_set + i;             //tmp_sys_gpio_dataָ�������GPIO�ռ�
        if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
        {
            continue;
        }

        port          = tmp_sys_gpio_data->port;                           //����port����
        port_num      = tmp_sys_gpio_data->port_num;                       //����port_num����
        port_num_func = (port_num >> 3);
        port_num_pull = (port_num >> 4);

        if(if_set_to_current_input_status)                                 //���ݵ�ǰ�û��趨����
        {
            //�޸�FUCN�Ĵ���
            script_gpio.mul_sel   = gpio_status->mul_sel;
            script_gpio.pull      = gpio_status->pull;
            script_gpio.drv_level = gpio_status->drv_level;
            script_gpio.data      = gpio_status->data;
        }
        else
        {
            script_gpio.mul_sel   = tmp_sys_gpio_data->user_gpio_status.mul_sel;
            script_gpio.pull      = tmp_sys_gpio_data->user_gpio_status.pull;
            script_gpio.drv_level = tmp_sys_gpio_data->user_gpio_status.drv_level;
            script_gpio.data      = tmp_sys_gpio_data->user_gpio_status.data;
        }

        if(script_gpio.mul_sel >= 0)
        {
        	tmp_addr = PIO_REG_CFG(port, port_num_func);
            reg_val = *tmp_addr;                      								 //�޸�FUNC�Ĵ���
            tmp_val = (port_num - (port_num_func<<3))<<2;
            reg_val &= ~(0x07 << tmp_val);
            reg_val |=  (script_gpio.mul_sel) << tmp_val;
            *tmp_addr = reg_val;
        }
        //�޸�PULL�Ĵ���
        if(script_gpio.pull >= 0)
        {
        	tmp_addr = PIO_REG_PULL(port, port_num_pull);
            reg_val = *tmp_addr;								                     //�޸�FUNC�Ĵ���
            tmp_val = (port_num - (port_num_pull<<4))<<1;
            reg_val &= ~(0x03 << tmp_val);
            reg_val |=  (script_gpio.pull) << tmp_val;
            *tmp_addr = reg_val;
        }
        //�޸�DLEVEL�Ĵ���
        if(script_gpio.drv_level >= 0)
        {
        	tmp_addr = PIO_REG_DLEVEL(port, port_num_pull);
            reg_val = *tmp_addr;                       								  //�޸�FUNC�Ĵ���
            tmp_val = (port_num - (port_num_pull<<4))<<1;
            reg_val &= ~(0x03 << tmp_val);
            reg_val |=  (script_gpio.drv_level) << tmp_val;
            *tmp_addr = reg_val;
        }
        //�޸�data�Ĵ���
        if(script_gpio.mul_sel == 1)
        {
            if(script_gpio.data >= 0)
            {
            	tmp_addr = PIO_REG_DATA(port);
                reg_val = *tmp_addr;                                   				   //�޸�DATA�Ĵ���
                reg_val &= ~(0x01 << port_num);
                reg_val |=  (script_gpio.data & 0x01) << port_num;
                *tmp_addr = reg_val;
            }
        }

        break;
    }

    return EGPIO_SUCCESS;
}
EXPORT_SYMBOL(gpio_set_one_pin_status);
/*
**********************************************************************************************************************
*                                               CSP_GPIO_Set_One_PIN_IO_Status
*
* Description:
*				�޸��û��������GPIO�е�ĳһ��IO�ڵģ��������״̬
* Arguments  :
*		p_handler	:	handler
*		if_set_to_output_status	:	���ó����״̬��������״̬
*		gpio_name	:	Ҫ������GPIO������
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  gpio_set_one_pin_io_status(u32 p_handler, __u32 if_set_to_output_status, const char *gpio_name)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max;                                //���GPIO����
	system_gpio_set_t  *user_gpio_set = NULL, *tmp_sys_gpio_data;
	volatile __u32      *tmp_group_func_addr = NULL;
	__u32               port, port_num, port_num_func;
	__u32				i, reg_val;
	
	//��鴫���ľ������Ч��
	if(!p_handler)
	{
		return EGPIO_FAIL;
	}
	if(if_set_to_output_status > 1)
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);
	if(group_count_max == 0)
	{
		return EGPIO_FAIL;
	}
	else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
				tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGPIO_FAIL;
	}

	port     = user_gpio_set->port;
	port_num = user_gpio_set->port_num;
	port_num_func = port_num >> 3;

	tmp_group_func_addr = PIO_REG_CFG(port, port_num_func);
	reg_val = *tmp_group_func_addr;
	reg_val &= ~(0x07 << (((port_num - (port_num_func<<3))<<2)));
	reg_val |=   if_set_to_output_status << (((port_num - (port_num_func<<3))<<2));
	*tmp_group_func_addr = reg_val;

	return EGPIO_SUCCESS;
}
EXPORT_SYMBOL(gpio_set_one_pin_io_status);
/*
**********************************************************************************************************************
*                                               CSP_GPIO_Set_One_PIN_Pull
*
* Description:
*				�޸��û��������GPIO�е�ĳһ��IO�ڵģ�PULL״̬
* Arguments  :
*		p_handler	:	handler
*		if_set_to_output_status	:	�����õ�pull״̬
*		gpio_name	:	Ҫ������GPIO������
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  gpio_set_one_pin_pull(u32 p_handler, __u32 set_pull_status, const char *gpio_name)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max;                                //���GPIO����
	system_gpio_set_t  *user_gpio_set = NULL, *tmp_sys_gpio_data;
	volatile __u32      *tmp_group_pull_addr = NULL;
	__u32               port, port_num, port_num_pull;
	__u32				i, reg_val;
	//��鴫���ľ������Ч��
	if(!p_handler)
	{
		return EGPIO_FAIL;
	}
	if(set_pull_status >= 4)
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);
	if(group_count_max == 0)
	{
		return EGPIO_FAIL;
	}
	else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
				tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGPIO_FAIL;
	}

	port     = user_gpio_set->port;
	port_num = user_gpio_set->port_num;
	port_num_pull = port_num >> 4;

	tmp_group_pull_addr = PIO_REG_PULL(port, port_num_pull);
	reg_val = *tmp_group_pull_addr;
	reg_val &= ~(0x03 << (((port_num - (port_num_pull<<4))<<1)));
	reg_val |=  (set_pull_status << (((port_num - (port_num_pull<<4))<<1)));
	*tmp_group_pull_addr = reg_val;

	return EGPIO_SUCCESS;
}
EXPORT_SYMBOL(gpio_set_one_pin_pull);
/*
**********************************************************************************************************************
*                                               CSP_GPIO_Set_One_PIN_driver_level
*
* Description:
*				�޸��û��������GPIO�е�ĳһ��IO�ڵģ���������
* Arguments  :
*		p_handler	:	handler
*		if_set_to_output_status	:	�����õ����������ȼ�
*		gpio_name	:	Ҫ������GPIO������
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  gpio_set_one_pin_driver_level(u32 p_handler, __u32 set_driver_level, const char *gpio_name)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max;                                //���GPIO����
	system_gpio_set_t  *user_gpio_set = NULL, *tmp_sys_gpio_data;
	volatile __u32      *tmp_group_dlevel_addr = NULL;
	__u32               port, port_num, port_num_dlevel;
	__u32				i, reg_val;
	//��鴫���ľ������Ч��
	if(!p_handler)
	{
		return EGPIO_FAIL;
	}
	if(set_driver_level >= 4)
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);
	
	if(group_count_max == 0)
	{
		return EGPIO_FAIL;
	}
	else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
				tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGPIO_FAIL;
	}

	port     = user_gpio_set->port;
	port_num = user_gpio_set->port_num;
	port_num_dlevel = port_num >> 4;
	
	tmp_group_dlevel_addr = PIO_REG_DLEVEL(port, port_num_dlevel);
	reg_val = *tmp_group_dlevel_addr;
	reg_val &= ~(0x03 << (((port_num - (port_num_dlevel<<4))<<1)));
	reg_val |=  (set_driver_level << (((port_num - (port_num_dlevel<<4))<<1)));
	*tmp_group_dlevel_addr = reg_val;

	return EGPIO_SUCCESS;
}
EXPORT_SYMBOL(gpio_set_one_pin_driver_level);
/*
**********************************************************************************************************************
*                                               CSP_GPIO_Read_One_PIN_Value
*
* Description:
*				��ȡ�û��������GPIO�е�ĳһ��IO�ڵĶ˿ڵĵ�ƽ
* Arguments  :
*		p_handler	:	handler
*		gpio_name	:	Ҫ������GPIO������
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  gpio_read_one_pin_value(u32 p_handler, const char *gpio_name)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max;                                //���GPIO����
	system_gpio_set_t  *user_gpio_set = NULL, *tmp_sys_gpio_data;
	__u32               port, port_num, port_num_func, func_val;
	__u32				i, reg_val;
	//��鴫���ľ������Ч��
	if(!p_handler)
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);

	if(group_count_max == 0)
	{
		return EGPIO_FAIL;
	}
	else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
				tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGPIO_FAIL;
	}

    port     = user_gpio_set->port;
    port_num = user_gpio_set->port_num;
    port_num_func = port_num >> 3;

    reg_val  = PIO_REG_CFG_VALUE(port, port_num_func);
    func_val = (reg_val >> ((port_num - (port_num_func<<3))<<2)) & 0x07;
    if(func_val == 0)
    {
        reg_val = (PIO_REG_DATA_VALUE(port) >> port_num) & 0x01;

        return reg_val;
    }

    return EGPIO_FAIL;
}
EXPORT_SYMBOL(gpio_read_one_pin_value);
/*
**********************************************************************************************************************
*                                               CSP_GPIO_Write_One_PIN_Value
*
* Description:
*				�޸��û��������GPIO�е�ĳһ��IO�ڵĶ˿ڵĵ�ƽ
* Arguments  :
*		p_handler	:	handler
*       value_to_gpio:  Ҫ���õĵ�ƽ�ĵ�ѹ
*		gpio_name	:	Ҫ������GPIO������
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  gpio_write_one_pin_value(u32 p_handler, __u32 value_to_gpio, const char *gpio_name)
{
	char               *tmp_buf;                                        //ת����char����
	__u32               group_count_max;                                //���GPIO����
	system_gpio_set_t  *user_gpio_set = NULL, *tmp_sys_gpio_data;
	volatile __u32     *tmp_group_data_addr = NULL;
	__u32               port, port_num, port_num_func, func_val;
	__u32				i, reg_val;
	//��鴫���ľ������Ч��
	if(!p_handler)
	{
		return EGPIO_FAIL;
	}
	if(value_to_gpio >= 2)
	{
		return EGPIO_FAIL;
	}
	tmp_buf = (char *)p_handler;
	group_count_max = *(int *)tmp_buf;
	tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);
	
	if(group_count_max == 0)
	{
		return EGPIO_FAIL;
	}
	else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
				tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGPIO_FAIL;
	}

	port     = user_gpio_set->port;
	port_num = user_gpio_set->port_num;
	port_num_func = port_num >> 3;

	reg_val  = PIO_REG_CFG_VALUE(port, port_num_func);
	func_val = (reg_val >> ((port_num - (port_num_func<<3))<<2)) & 0x07;
	if(func_val == 1)
	{
		tmp_group_data_addr = PIO_REG_DATA(port);
		reg_val = *tmp_group_data_addr;
		reg_val &= ~(1 << port_num);
		reg_val |=  (value_to_gpio << port_num);
		*tmp_group_data_addr = reg_val;

		return EGPIO_SUCCESS;
	}

	return EGPIO_FAIL;
}
EXPORT_SYMBOL(gpio_write_one_pin_value);

