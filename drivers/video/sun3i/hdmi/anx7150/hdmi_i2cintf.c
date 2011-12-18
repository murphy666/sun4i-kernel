#include "hdmi_i2cintf.h"
/*

static ES_FILE      * p_i2c = 0;;

__s32 ANX7150_i2c_Request(void)
{
	__u32 clock = 400000;
	
    p_i2c = eLIBs_fopen("b:\\BUS\\TWI1", "r");//��1·twi
    if(!p_i2c)
    {
        __err("open i2c device fail!\n");
        return -1;
    }
	eLIBs_fioctrl(p_i2c, TWI_SET_SCL_CLOCK, 0, (void *)clock);
	
	return 0;
}

__s32 ANX7150_i2c_Release(void)
{
	eLIBs_fclose(p_i2c);
	p_i2c = 0;

	return EPDK_OK;
}

__s32 ANX7150_i2c_write_p0_reg(__u8 offset, __u8 d)
{
	__twi_dev_para_ex_t stwi;
	__u8			 data;
	__u8            byte_addr[2];    // ���豸�ļĴ�����ַ�����ֽڴ�ŵ͵�ַ�����ֽڴ�Ÿߵ�ַ
	__s32            ret;
        
	eLIBs_memset(&stwi, 0, sizeof(__twi_dev_para_ex_t));

	data = d;
	byte_addr[0] = offset;
	byte_addr[1] = 0;
	stwi.slave_addr 	  =  ANX7150_PORT0_ADDR/2;	   // ���豸�еĵ�ַ
	stwi.slave_addr_flag	=  0   ;	 // 7 bit address  
	stwi.byte_addr		= byte_addr; //���豸�ļĴ�����ַ
	stwi.byte_addr_width	=  1;		 // ������һ�����������
	stwi.byte_count	   =   1;		 //Ҫ���ͻ���յ����ݴ�С
	stwi.data 			= &data; 	 // ����buffer�ĵ�ַ

	ret = eLIBs_fioctrl(p_i2c, TWI_WRITE_SPEC_RS, 0, (void *)&stwi);
	if(ret != EPDK_OK)
	{
		__wrn("i2c 7150 write port0:%x,%x failed!\n",offset,d);

		return EPDK_FAIL;
	}
	else
	{
		//__wrn("i2c 7150 write port0:%x,%x ok!\n",offset,d);
	}
	
	return EPDK_OK;
}

__s32 ANX7150_i2c_write_p1_reg(__u8 offset, __u8 d)
{
	__twi_dev_para_ex_t stwi;
	__u8			 data;
	__u8			byte_addr[2];	 // ���豸�ļĴ�����ַ�����ֽڴ�ŵ͵�ַ�����ֽڴ�Ÿߵ�ַ
	__s32			 ret;
		
	eLIBs_memset(&stwi, 0, sizeof(__twi_dev_para_ex_t));

	data = d;
	byte_addr[0] = offset;
	byte_addr[1] = 0;
	stwi.slave_addr 	  =  ANX7150_PORT1_ADDR/2;	   // ���豸�еĵ�ַ
	stwi.slave_addr_flag	=  0   ;	 // 7 bit address  
	stwi.byte_addr		= byte_addr; //���豸�ļĴ�����ַ
	stwi.byte_addr_width	=  1;		 // ������һ�����������
	stwi.byte_count    =   1;		 //Ҫ���ͻ���յ����ݴ�С
	stwi.data			= &data;	 // ����buffer�ĵ�ַ

	ret = eLIBs_fioctrl(p_i2c, TWI_WRITE_SPEC_RS, 0, (void *)&stwi);
	if(ret != EPDK_OK)
	{
		__wrn("i2c 7150 write port1:%x,%x failed!\n",offset,d);

		return EPDK_FAIL;
	}
	else
	{
		//__wrn("i2c 7150 write port1:%x,%x ok!\n",offset,d);
	}
	
	return EPDK_OK;
}


__s32 ANX7150_i2c_read_p0_reg(__u8 offset, __u8 *d)
{
	__twi_dev_para_ex_t stwi;
	__u8			byte_addr[2];	 // ���豸�ļĴ�����ַ�����ֽڴ�ŵ͵�ַ�����ֽڴ�Ÿߵ�ַ
	__s32			 ret;
		
	eLIBs_memset(&stwi, 0, sizeof(__twi_dev_para_ex_t));

	byte_addr[0] = offset;
	byte_addr[1] = 0;
	stwi.slave_addr 	  =  ANX7150_PORT0_ADDR/2;	   // ���豸�еĵ�ַ
	stwi.slave_addr_flag	=  0   ;	 // 7 bit address  
	stwi.byte_addr		= byte_addr; //���豸�ļĴ�����ַ
	stwi.byte_addr_width	=  1;		 // ������һ�����������
	stwi.byte_count    =   1;		 //Ҫ���ͻ���յ����ݴ�С
	stwi.data			= d;	 // ����buffer�ĵ�ַ

	ret = eLIBs_fioctrl(p_i2c, TWI_READ_SPEC_RS, 0, (void *)&stwi);
	if(ret != EPDK_OK)
	{
		__wrn("i2c 7150 read port0:%x failed!\n",offset);

		return EPDK_FAIL;
	}
	else
	{
		//__wrn("i2c 7150 read port0:%x,%x ok!\n",offset,*d);
	}

	return EPDK_OK;
}


__s32 ANX7150_i2c_read_p1_reg(__u8 offset, __u8 *d)
{
	__twi_dev_para_ex_t stwi;
	__u8			byte_addr[2];	 // ���豸�ļĴ�����ַ�����ֽڴ�ŵ͵�ַ�����ֽڴ�Ÿߵ�ַ
	__s32			 ret;
		
	eLIBs_memset(&stwi, 0, sizeof(__twi_dev_para_ex_t));

	byte_addr[0] = offset;
	byte_addr[1] = 0;
	stwi.slave_addr 	  =  ANX7150_PORT1_ADDR/2;	   // ���豸�еĵ�ַ
	stwi.slave_addr_flag	=  0   ;	 // 7 bit address  
	stwi.byte_addr		= byte_addr; //���豸�ļĴ�����ַ
	stwi.byte_addr_width	=  1;		 // ������һ�����������
	stwi.byte_count    =   1;		 //Ҫ���ͻ���յ����ݴ�С
	stwi.data			= d;	 // ����buffer�ĵ�ַ

	ret = eLIBs_fioctrl(p_i2c, TWI_READ_SPEC_RS, 0, (void *)&stwi);
	if(ret != EPDK_OK)
	{
		__wrn("i2c 7150 read port1:%x failed!\n", offset);

		return EPDK_FAIL;
	}
	else
	{
		//__wrn("i2c 7150 read port1:%x,%x ok!\n",offset,*d);
	}
	
	return EPDK_OK;
}

*/

