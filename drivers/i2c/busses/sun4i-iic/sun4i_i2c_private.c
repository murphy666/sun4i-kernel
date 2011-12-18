/*
*********************************************************************************************************
*                                                      eBase
*                                           the Abstract of Hardware
*
*                                   (c) Copyright 2006-2010, AW China
*                                                   All Rights Reserved
*
* File        :  twi_private.c
* Date        :  2010-11-13
* Author      :  victor
* Version     :  v1.00
* Description:  
*                                   Operation for TWI module,aw1620
* History     :
*      <author>          <time>             <version >          <desc>
*       victor         2010-11-13              1.0           create this file     
*
*********************************************************************************************************
*/
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/dma.h>

#include "sun4i_i2c_private.h"
//#define CONFIG_FPGA_SIM
/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_get_byte
*
*  Description:
*               ��ȡ���ݣ��ٰ�flag�����
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*       buffer : ������ݵ�ָ��
*
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_get_byte(void *base_addr, unsigned char  *buffer)
{    
    *buffer = (unsigned char)( TWI_DATA_MASK & readl(base_addr + TWI_DATA_REG) );
    aw_twi_clear_irq_flag(base_addr);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_get_last_byte
*
*  Description:
*              ֻȡ���ݣ���flag��stop������
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*       buffer : ������ݵ�ָ��
*
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_get_last_byte(void *base_addr, unsigned char  *buffer)
{    
    *buffer = (unsigned char)( TWI_DATA_MASK & readl(base_addr + TWI_DATA_REG) );
    return;
}


/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_put_byte
*
*  Description:
*               �Ȱ����ݷŵ�data register���ٰ�irq flag���㣬�������͡�
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*          buffer  :  �������ݵ�ָ��
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_put_byte(void *base_addr, const unsigned char *buffer)
{
    //write data and clear irq flag to trigger send flow
    writel((unsigned int)*buffer, base_addr + TWI_DATA_REG);
    aw_twi_clear_irq_flag(base_addr);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_enable_irq
*
*  Description:
*               ���ƼĴ���,�ж�ʹ��λ
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_enable_irq(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    
    /* enable irq: 2011-8-18 13:39:05
     * 1 when enable irq for next operation, set intflag to 1 to prevent to clear it by a mistake
     *   (intflag bit is write-0-to-clear bit)
     * 2 Similarly, mask startbit and stopbit to prevent to set it twice by a mistake
     *   (start bit and stop bit are self-clear-to-0 bits)
     */
    reg_val |= (TWI_CTL_INTEN | TWI_CTL_INTFLG);
    reg_val &= ~(TWI_CTL_STA | TWI_CTL_STP);
    writel(reg_val, base_addr + TWI_CTL_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_disable_irq
*
*  Description:
*               ���ƼĴ���,�ж�ʹ��λ���ر��ж�
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_disable_irq(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val &= ~TWI_CTL_INTEN;
    writel(reg_val, base_addr + TWI_CTL_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_disable_bus
*
*  Description:
*               ���ƼĴ���,�ر�twiģ�顣
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_disable_bus(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val &= ~TWI_CTL_BUSEN;
    writel(reg_val, base_addr + TWI_CTL_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_enable_bus
*
*  Description:
*               ���ƼĴ���,��twiģ�顣
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_enable_bus(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val |= TWI_CTL_BUSEN;
    writel(reg_val, base_addr + TWI_CTL_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_set_start
*
*  Description:
*               ���ƼĴ���,����start�źš�startλ���Զ����㡣
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_set_start(void *base_addr)
{    
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val |= TWI_CTL_STA;
    writel(reg_val, base_addr + TWI_CTL_REG);
    return;    
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_get_start
*
*  Description:
*              ��ȡstart bit��״̬����Ҫ������ѯstart�Ƿ񷢳���
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           start��״̬
*  Notes:
*   
****************************************************************************************************
*/ 
unsigned int aw_twi_get_start(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val >>= 5;
    return reg_val & 1;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_set_stop
*
*  Description:
*               ���ƼĴ���,����stop�źš�stopλ���Զ����㡣
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_set_stop(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val |= TWI_CTL_STP;
    writel(reg_val, base_addr + TWI_CTL_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_get_stop
*
*  Description:
*              ��ȡstop bit��״̬����Ҫ������ѯstop�Ƿ񷢳���
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           stop��״̬
*  Notes:
*   
****************************************************************************************************
*/ 
unsigned int aw_twi_get_stop(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val >>= 4;
    return reg_val & 1;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_disable_ack
*
*  Description:
*               ���ƼĴ���,�ڷ���ack��nack�ڼ䣬���Զ�����nack��
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_disable_ack(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val &= ~TWI_CTL_ACK;
    writel(reg_val, base_addr + TWI_CTL_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_enable_ack
*
*  Description:
*               ���ƼĴ���,�ڷ���ack��nack�ڼ䣬���Զ�����ack��
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_enable_ack(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val |= TWI_CTL_ACK;
    writel(reg_val, base_addr + TWI_CTL_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_clear_irq_flag
*
*  Description:
*               ���ƼĴ���,����жϵ�����־λ��
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_clear_irq_flag(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    reg_val &= ~TWI_CTL_INTFLG;//0x 1111_0111
    writel(reg_val ,base_addr + TWI_CTL_REG);
    
    //read two more times to make sure that interrupt flag does really be cleared
    {
        unsigned int temp;
        temp = readl(base_addr + TWI_CTL_REG);
        temp |= readl(base_addr + TWI_CTL_REG);
    }
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_query_irq_flag
*
*  Description:
*               ���ƼĴ���,��ѯ�жϱ�־λ��
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*           �����жϱ�־λ״̬��
*  Notes:
*   
****************************************************************************************************
*/ 
unsigned int aw_twi_query_irq_flag(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CTL_REG);
    return (reg_val & TWI_CTL_INTFLG);//0x 0000_1000
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_query_irq_status
*
*  Description:
*               ���ƼĴ���,��ѯ�ж�״̬�롣
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*          �ж�״̬�롣
*  Notes:
*   
****************************************************************************************************
*/ 
unsigned int aw_twi_query_irq_status(void *base_addr)
{ 
    unsigned int reg_val = readl(base_addr + TWI_STAT_REG);
    return (reg_val & TWI_STAT_MASK);
}

/*
****************************************************************************************************
*
*  FunctionName:           _twi_set_clk
*
*  Description:
*              ʱ�ӼĴ���
*  Parameters:
*       clk_n   :  ��Ƶϵ��CLK_N
*       clk_m   :  ��Ƶϵ��CLK_M
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
static void _twi_set_clk(unsigned int clk_n, unsigned int clk_m, void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_CLK_REG);
    printk("_twi_set_clk: clk_n = %d, clk_m = %d\n", clk_n, clk_m);
    reg_val &= ~(TWI_CLK_DIV_M | TWI_CLK_DIV_N);
    reg_val |= ( clk_n |(clk_m << 3) );
    writel(reg_val, base_addr + TWI_CLK_REG);
    return;
}

/*   
*/
/*
****************************************************************************************************
*
*  FunctionName:      aw_twi_set_clock
*
*  Description:
*             ʱ�ӼĴ���,Ҫ�����õ�sclʱ��Ƶ�ʸ�ʵ�ʼ�������Ĵ���һ�������Ǳƽ���
*                    Fin is APB CLOCK INPUT;
*                    Fsample = F0 = Fin/2^CLK_N; 
*                              F1 = F0/(CLK_M+1);
*                              
*                    Foscl = F1/10 = Fin/(2^CLK_N * (CLK_M+1)*10); 
*                    Foscl is clock SCL;100KHz or 400KHz      
*                    clkΪ�����apbʱ��Դ��Ҫ�������ʱ��Ϊsclk
*  Parameters:
*       clk_in   :  apb clkʱ��
*       sclk_req   :  Ҫ���õ�sclʱ��Ƶ�� ��λ:HZ
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_set_clock(unsigned int clk_in, unsigned int sclk_req, void *base_addr)
{ 
    unsigned int clk_m = 0;
    unsigned int clk_n = 0;
    unsigned int _2_pow_clk_n = 1;
    unsigned int src_clk      = clk_in/10;   
    unsigned int divider      = src_clk/sclk_req;  // 400khz or 100khz  
    unsigned int sclk_real    = 0;      // the real clock frequency

    //added by young, for testing
#ifdef CONFIG_FPGA_SIM
   {
        clk_m = 2;
        clk_n = 3;
        goto set_clk;
    }
#endif

    if (divider==0)
    {
        clk_m = 1;
        goto set_clk;
    }
    // search clk_n and clk_m,from large to small value so that can quickly find suitable m & n.
    while (clk_n < 8) // 3bits max value is 8 
    {
        // (m+1)*2^n = divider -->m = divider/2^n -1;
        clk_m = (divider/_2_pow_clk_n) - 1; 
        //clk_m = (divider >> (_2_pow_clk_n>>1))-1;
        while (clk_m < 16) // 4bits max value is 16
        {
            sclk_real = src_clk/(clk_m + 1)/_2_pow_clk_n; //src_clk/((m+1)*2^n)
            if (sclk_real <= sclk_req)
            {
                goto set_clk;
            }
            else
            {
                clk_m++;
            }
        }
        clk_n++;
        _2_pow_clk_n *= 2; // mutilple by 2
    }

set_clk:    
    _twi_set_clk(clk_n, clk_m, base_addr);

    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_soft_reset
*
*  Description:
*              ��λ�Ĵ���
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_soft_reset(void *base_addr)
{
    unsigned int reg_val = readl(base_addr + TWI_SRST_REG);
    reg_val |= TWI_SRST_SRST;//set soft reset bit,0x0000 0001
    writel(reg_val, base_addr + TWI_SRST_REG);    
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_soft_reset
*
*  Description:
*              ��ǿ���ԼĴ���,SLAVE ADDRESS+R�յ�ACK�����������Ҫдdata register �Ļ���Ҫ��������Ĵ���
*           ���������I2C����ʽ��
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*           efr    :   ����ȡֵ0x00,0x01,0x02,0x03
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_set_EFR(void *base_addr, unsigned int efr)
{
    unsigned int reg_val = readl(base_addr + TWI_EFR_REG);
    
    reg_val &= ~TWI_EFR_MASK;
    efr     &= TWI_EFR_MASK;
    reg_val |= efr;    
    writel(reg_val, base_addr + TWI_EFR_REG);
    return;	
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_set_sda
*
*  Description:
*                   ����sda�ĸߵ͵�ƽ,�ڿ���ʹ��SDA������µ�������ӿڲ������塣
*               TWI�߿��ƼĴ���,�ɹ��ϲ��ioctrl���ã����õ�ǰ��sda״̬�Ա������
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*           hi_lo  :  1-high level��0-low level
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_set_sda(void *base_addr, unsigned int hi_lo)
{
    unsigned int reg_val = readl(base_addr + TWI_LCR_REG);
    reg_val &= ~TWI_LCR_SDA_CTL;
    hi_lo   &= 0x01;// mask 
    reg_val |= (hi_lo << 1);
    writel(reg_val, base_addr + TWI_LCR_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_get_sda
*
*  Description:
*               �õ�sda��״̬,����ӿڿɹ��ϲ�ֱ�ӵ��ã�����Ҫ����ʹ��sda����λ��
*           TWI�߿��ƼĴ���,�ɹ��ϲ��ioctrl���ã��õ���ǰ��sda״̬�Ա������
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/
unsigned int aw_twi_get_sda(void *base_addr)
{
    unsigned int status = 0;
    status = TWI_LCR_SDA_STATE_MASK & readl(base_addr + TWI_LCR_REG);
    status >>= 4;
    return  (status&0x1);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_set_scl
*
*  Description:
*               ����scl�ĸߵ͵�ƽ,�ڿ���ʹ��SCL������µ�������ӿڲ������塣
*               TWI�߿��ƼĴ���,�ɹ��ϲ��ioctrl���ã����õ�ǰ��scl״̬�Ա������
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*           hi_lo  :  1-high level��0-low level
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/ 
void aw_twi_set_scl(void *base_addr, unsigned int hi_lo)
{
    unsigned int reg_val = readl(base_addr + TWI_LCR_REG);
    reg_val &= ~TWI_LCR_SCL_CTL;
    hi_lo   &= 0x01;// mask 
    reg_val |= (hi_lo<<3);
    writel(reg_val, base_addr + TWI_LCR_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_get_scl
*
*  Description:
*               �õ�scl��״̬,����ӿڿɹ��ϲ�ֱ�ӵ��ã�����Ҫ����ʹ��scl����λ��
*           TWI�߿��ƼĴ���,�ɹ��ϲ��ioctrl���ã��õ���ǰ��scl״̬�Ա������
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/
unsigned int aw_twi_get_scl(void *base_addr)
{
    unsigned int status = 0;
    status = TWI_LCR_SCL_STATE_MASK & readl(base_addr + TWI_LCR_REG);
    status >>= 5;
    return  (status&0x1);    
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_enable_LCR
*
*  Description:
*               ʹ�ܺ�����ʹ��sda��scl�߿ء�
*           TWI�߿��ƼĴ���,�ɹ��ϲ��ioctrl���ã��Ա������
*           
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*       sda_scl    :  0-ʹ��sda��1-ʹ��scl
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/
void aw_twi_enable_LCR(void *base_addr, unsigned int sda_scl)
{
    unsigned int reg_val = readl(base_addr + TWI_LCR_REG);
    sda_scl &= 0x01;// mask 
    if(sda_scl)
    {
        reg_val |= TWI_LCR_SCL_EN;//enable scl line control
    }
    else
    {
        reg_val |= TWI_LCR_SDA_EN;//enable sda line control
    }
    writel(reg_val, base_addr + TWI_LCR_REG);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_twi_disable_LCR
*
*  Description:
*               ʹ�ܺ������ر�sda��scl���ơ�
*           TWI�߿��ƼĴ���,�ɹ��ϲ��ioctrl���ã��Ա������
*           
*  Parameters:
*       base_addr  :  �Ĵ�������ַ
*       sda_scl    :  0-�ر�sda��1-�ر�scl
*  Return value:
*          �ޡ�
*  Notes:
*   
****************************************************************************************************
*/
void aw_twi_disable_LCR(void *base_addr, unsigned int sda_scl)
{
    unsigned int reg_val = readl(base_addr + TWI_LCR_REG);
    sda_scl &= 0x01;// mask 
    if(sda_scl)
    {
        reg_val &= ~TWI_LCR_SCL_EN;//disable scl line control
    }
    else
    {
        reg_val &= ~TWI_LCR_SDA_EN;//disable sda line control
    }
    writel(reg_val, base_addr + TWI_LCR_REG);
    return;
}
