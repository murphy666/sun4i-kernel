/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : gpiov2.h
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
#ifndef	  __GPIO_V2_H__
#define	  __GPIO_V2_H__

#define   EGPIO_FAIL             (-1)
#define   EGPIO_SUCCESS          (0)

typedef enum
{
	PIN_PULL_DEFAULT 	= 	0xFF,
	PIN_PULL_DISABLE 	=	0x00,
	PIN_PULL_UP			=	0x01,
	PIN_PULL_DOWN		=	0x02,
	PIN_PULL_RESERVED	=	0x03
}pin_pull_level_t;



typedef	enum
{
	PIN_MULTI_DRIVING_DEFAULT	=	0xFF,
	PIN_MULTI_DRIVING_0			=	0x00,
	PIN_MULTI_DRIVING_1			=	0x01,
	PIN_MULTI_DRIVING_2			=	0x02,
	PIN_MULTI_DRIVING_3			=	0x03
}pin_drive_level_t;

typedef enum
{
    PIN_DATA_LOW    ,
    PIN_DATA_HIGH   ,
    PIN_DATA_DEFAULT = 0XFF
}pin_data_t;


#define	PIN_PHY_GROUP_A			0x00
#define	PIN_PHY_GROUP_B			0x01
#define	PIN_PHY_GROUP_C			0x02
#define	PIN_PHY_GROUP_D			0x03
#define	PIN_PHY_GROUP_E			0x04
#define	PIN_PHY_GROUP_F			0x05
#define	PIN_PHY_GROUP_G			0x06
#define	PIN_PHY_GROUP_H			0x07
#define	PIN_PHY_GROUP_I			0x08
#define	PIN_PHY_GROUP_J			0x09


typedef struct
{
    char  gpio_name[32];
    int port;
    int port_num;
    int mul_sel;
    int pull;
    int drv_level;
    int data;
}
user_gpio_set_t;

/*
************************************************************************************************************
*
*                                             gpio_init
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    �� GPIO�����ʼ��������һ��GPIO����ַ
*
*
************************************************************************************************************
*/
extern  int      gpio_init(void);
/*
************************************************************************************************************
*
*                                             gpio_exit
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��  GPIO�����˳����޲�����ֱ���˳�
*
*
************************************************************************************************************
*/
extern  int      gpio_exit(void);
/*
************************************************************************************************************
*
*                                             gpio_Request
*
*    �������ƣ�
*
*    �����б� gpio_list �û����ݵ�ַ�����ڴ����û���GPIO������Ϣ
*               group_count_max  �û����ݵĸ����������ֵӦ�ô��ڻ��ߵ����û�ʵ�ʵ�GPIO����
*
*    ����ֵ  �� ����ɹ�������һ����������򷵻�0ֵ
*
*    ˵��    �� GPIO�����û����ݰ��սṹ�崫�ݣ�һ���ṹ�屣��һ��GPIO��Ϣ��
*
*
************************************************************************************************************
*/
extern  unsigned gpio_request                 (user_gpio_set_t *gpio_list,                                                              unsigned group_count_max               );
extern  unsigned gpio_request_ex(char *main_name, const char *sub_name);  //�豸����GPIO������չ�ӿ�
/*
************************************************************************************************************
*
*                                             gpio_Release
*
*    �������ƣ�
*
*    �����б�  p_handler  ���뵽�ľ��
*                if_release_to_default_status   �ͷź��״̬�������ͷź�״̬���䣬���Ա��ȫ����״̬�����Ա������ǰ��״̬
*
*    ����ֵ  ��
*
*    ˵��    �� �û�����ʹ��GPIO���ͷŵ���
*
*
************************************************************************************************************
*/
extern  int      gpio_release                 (unsigned p_handler,                                                                      int if_release_to_default_status       );
/*
************************************************************************************************************
*
*                                             gpio_get_all_pin_status
*
*    �������ƣ�
*
*    �����б� p_handler  ���뵽�ľ��
*               gpio_status  �����û����ݵĵ�ַ
*               gpio_count_max �����û����ݵĽṹ��ĸ����������ֵӦ�ô��ڻ��ߵ���ʵ�ʵ�GPIO����
*               if_get_from_hardware  ϣ����ȡ����GPIO��Ϣ��Դ��ʵ�ʵļĴ���״̬��������
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_get_all_pin_status(unsigned p_handler, user_gpio_set_t *gpio_status, unsigned gpio_count_max, unsigned if_get_from_hardware);
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_get_one_pin_status      (unsigned p_handler, user_gpio_set_t *gpio_status,             const char *gpio_name,     unsigned if_get_from_hardware          );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_set_one_pin_status      (unsigned p_handler, user_gpio_set_t *gpio_status,             const char *gpio_name,     unsigned if_set_to_current_input_status);
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_set_one_pin_io_status   (unsigned p_handler, unsigned         if_set_to_output_status, const char *gpio_name                                             );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_set_one_pin_pull        (unsigned p_handler, unsigned         set_pull_status,         const char *gpio_name                                             );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_set_one_pin_driver_level(unsigned p_handler, unsigned         set_driver_level,        const char *gpio_name                                             );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_read_one_pin_value      (unsigned p_handler,                                           const char *gpio_name                                             );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_write_one_pin_value     (unsigned p_handler, unsigned         value_to_gpio,           const char *gpio_name                                             );



#endif	//__GPIO_V2_H__

