/*
*********************************************************************************************************
*                                                      eBase
*                                           the Abstract of Hardware
*
*                                   (c) Copyright 2006-2010, holigun China
*                                                   All Rights Reserved
*
* File        :  spi_private.h
* Date        :  2010-11-27
* Author      :  Victor
* Version     :  v1.00
* Description:  
*                                   Operation for SPI module,aw1620
* History     :
*      <author>          <time>             <version >          <desc>
*       Victor         2010-11-27              1.0           create this file     
*********************************************************************************************************
*/
#include "spi_private.h"
#include <asm/io.h>


/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_set_cs
*
*  Description:
*              ����Ƭѡ�ź�,ʵ����оƬ��ͬ��Ƭѡ������ͬ�����Ϊ4����
*
*  Parameters:
*       chipselect :   Ƭѡѡ��:0,1,2,3
*       base_addr:   SPI�Ĵ�������ַ
*
*  Return value:
*       ��ʼ���ɹ�����AW_SPI_OK��ʧ�ܷ���AW_SPI_FAIL
*  Notes:
*   
****************************************************************************************************
*/
s32 aw_spi_set_cs(u32 chipselect, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
   
    if (chipselect < 4)
    {
        reg_val &= ~SPI_CTL_SS_MASK;//SS-chip select, clear two bits
        reg_val |= chipselect << SPI_SS_BIT_POS;//set chip select 

        writel(reg_val, base_addr + SPI_CTL_REG);
        
//        spi_msg("Chip Select set succeed! cs = %d\n", chipselect);
        return AW_SPI_OK;
    }
    else 
    {   
        spi_wrn("Chip Select set fail! cs = %d\n", chipselect);
        return AW_SPI_FAIL;
    }

}

s32 aw_spi_sel_dma_type(u32 dma_type, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    
//    spi_msg("set dma type %s\n", dma_mode ? "ddma" : "ndma");
    reg_val &= ~SPI_CTL_DMAMOD;
    if (dma_type)
    {
        reg_val |= 1 << 5;
    }
    writel(reg_val, base_addr + SPI_CTL_REG);
        
    return AW_SPI_OK;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_config
*
*  Description:
*             ������,ģʽ-POL,PHA,Ƭѡ����-SSPOL,����bit��ʼ-LMTF,�Զ����0/1-DDB,������Чburst-DHB
*             ��ʱĬ�����õ���:SSCTL=0��SMC=1,TBW=0,�Ƿ���burst֮�䱣��ƬѡSSCTLΪ0�� SMCĬ��Ϊ1,��smartģʽ��
*             TBW=0,��ʾ8bit��
*  Parameters:
*           master:    slave���û���master���ã�1-master��0-slave
*           config:      ����λͼ
*           base_addr��    spi�Ĵ�������ַ
*  Return value:
*           ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_config(u32 master, u32 config, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    /*1. POL */
    if (config & SPI_POL_ACTIVE_)
    {
        reg_val |= SPI_CTL_POL;
    }
    else
    {
        reg_val &= ~SPI_CTL_POL;/*default POL = 0 */
    }
    /*2. PHA */
    if (config & SPI_PHA_ACTIVE_)
    {
        reg_val |= SPI_CTL_PHA;
    }
    else
    {
        reg_val &= ~SPI_CTL_PHA;/*default PHA = 0 */
    }   
    /*3. SSPOL,chip select signal polarity */
    if (config & SPI_CS_HIGH_ACTIVE_)
    {
        reg_val &= ~SPI_CTL_SSPOL;
    }
    else
    {
        reg_val |= SPI_CTL_SSPOL;/*default SSPOL = 1,Low level effective */
    }    
    /*4. LMTF--LSB/MSB transfer first select */
    if (config & SPI_LSB_FIRST_ACTIVE_)
    {
        reg_val |= SPI_CTL_LMTF;
    }
    else
    {
        reg_val &= ~SPI_CTL_LMTF;/*default LMTF =0, MSB first */
    }        
    
    /*master mode: set DDB,DHB,SMC,SSCTL*/
    if(master == 1)
    {   
        /*5. dummy burst type */
        if (config & SPI_DUMMY_ONE_ACTIVE_)
        {
            reg_val |= SPI_CTL_DDB;
        }
        else
        {
            reg_val &= ~SPI_CTL_DDB;/*default DDB =0, ZERO */
        } 
        /*6.discard hash burst-DHB */
        if (config & SPI_RECEIVE_ALL_ACTIVE_)
        {
            reg_val &= ~SPI_CTL_DHB;
        }
        else
        {
            reg_val |= SPI_CTL_DHB;/*default DHB =1, discard unused burst */
        } 
        
        /*7. set SMC = 1 , SSCTL = 0 ,TPE = 1 */
        reg_val &= ~SPI_CTL_SSCTL;     
        reg_val |=  SPI_CTL_T_PAUSE_EN;
    }    
    else
    {
        /* tips for slave mode config */
        spi_msg("slave mode configurate control register.\n");
    }

    writel(reg_val, base_addr + SPI_CTL_REG);

    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_restore_state
*
*  Description:
*              ������ɺ󣬼Ĵ����ָ�Ĭ������
*               master : DHB=1,SMC=1,reset rxFIFO&txFIFO,SSPOL=1,POL=1,PHA=1,master,TPE=1,module enable��
*                        DDB=0,SS=0,XCH=0,SSCTL=0,LMTF=0,TBW=0��           
*               slave  : reset rxFIFO&txFIFO��SSPOL=1,POL=1,PHA=1,module enable.
*
*  Parameters:
*       master :   0-slave��1-master
*       base_addr:   SPI�Ĵ�������ַ
*
*  Return value:
*       none
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_restore_state(u32 master, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

//config spi control register
//| 15 |  14  |  13  |  12  |  11  |  10  |  9  |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//| DHB|  DDB |     SS      | SMC  |  XCH |TFRST|RFRST|SSCTL| MSB | TBW |SSPOL| POL | PHA | MOD | EN  |
//| 1  |   0  |     00      |  1   |   0  |  0  |  0  |  0  |  0  |  0  |  1  |  1  |  1  |  1  |  1  | 
    //master mode
    if(master)
    {
        reg_val |= (SPI_CTL_DHB|SPI_CTL_SSPOL|SPI_CTL_POL|SPI_CTL_PHA
                        |SPI_CTL_FUNC_MODE|SPI_CTL_EN); 
        reg_val |= SPI_CTL_T_PAUSE_EN;/* new bit,transmit pause enable,stop smart dummy when rxfifo full*/                                    
                                    
        reg_val &= ~(SPI_CTL_DDB|SPI_CTL_SS_MASK|SPI_CTL_XCH|SPI_CTL_SSCTL|SPI_CTL_LMTF);//|SPI_CTL_TBW); //deleted SPI_CTL_TBW bit for aw1623, this bit is defined for dma mode select, 2011-5-26 19:55:32
    }
    else/* slave mode */
    {
         reg_val |= (SPI_CTL_SSPOL|SPI_CTL_POL|SPI_CTL_PHA||SPI_CTL_EN); 
                                    
        reg_val &= ~(SPI_CTL_DHB|SPI_CTL_FUNC_MODE|SPI_CTL_DDB|SPI_CTL_SS_MASK
                            |SPI_CTL_XCH|SPI_CTL_SSCTL|SPI_CTL_LMTF);//|SPI_CTL_TBW);            //deleted SPI_CTL_TBW bit for aw1623, this bit is defined for dma mode select, 2011-5-26 19:55:32               
        
    }

    spi_msg("control register set default value: %x \n", reg_val);
    writel(reg_val, base_addr + SPI_CTL_REG);
    
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_set_clk
*
*  Description:
*                   ��(ahb_clk/2)/spi_clk ��= 256 ʱ����CDR2����256ʱ����CDR1��
*               
*  Parameters:
*           spi_clk:  ��Ҫ���õ�spi����ʱ��Ƶ��
*           ahb_clk�� ahb���ߵ�Ƶ��
*           base_addr: �Ĵ�������ַ
*  Return value:
*           ��
*  Notes:
*           ��������masterģʽ��slaveģʽ�����á�
****************************************************************************************************
*/
void aw_spi_set_clk(u32 spi_clk, u32 ahb_clk, void *base_addr)
{
    u32 reg_val = 0;
    u32 N = 0;
    u32 div_clk = (ahb_clk>>1)/spi_clk;
    
//    spi_msg("set spi clock %d, mclk %d\n", spi_clk, ahb_clk);
    reg_val = readl(base_addr + SPI_CLK_RATE_REG);

    /* CDR2 */
    if(div_clk <= SPI_CLK_SCOPE)
    {
        if (div_clk != 0)
        {
            div_clk--;
        }
        reg_val &= ~SPI_CLKCTL_CDR2;
        reg_val |= (div_clk|SPI_CLKCTL_DRS);
        
//        spi_msg("CDR2 - n = %d \n", div_clk);
        
    }
    else /* CDR1 */
    {
        //search 2^N
        while(1)
        {                  
            if(div_clk == 1)
            {
                break;
            }
            div_clk >>= 1;//divide by 2
            N++;            
        };
        reg_val &= ~(SPI_CLKCTL_CDR1|SPI_CLKCTL_DRS);
        reg_val |= (N<<8);  
        
//        spi_msg("CDR1 - n = %d \n", N);
    }    
   
    writel(reg_val, base_addr + SPI_CLK_RATE_REG);
  
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_start_xfer
*
*  Description:
*                   ��SMC=1ʱ����XCHλ��1���������䣬����������bit���Զ����㣬
*                ������ѯ���bitҲ����֪�������Ƿ������
*
*  Parameters:
*           base_addr:   �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_start_xfer(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val |= SPI_CTL_XCH;
    writel(reg_val, base_addr + SPI_CTL_REG);    
}


/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_query_xfer
*
*  Description:
*                   ��smcģʽ�£���ѯ�Ƿ���ɴ���,����0��ʾ������ɣ����ط����ʾ���ڴ�����
*
*  Parameters:
*           base_addr:   �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
u32 aw_spi_query_xfer(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    
    return (reg_val & SPI_CTL_XCH);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_enable_bus
*
*  Description:
*                 ʹ��spi bus
*
*  Parameters:
*           base_addr:   �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_enable_bus(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val |= SPI_CTL_EN;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_disable_bus
*
*  Description:
*                �ر�spi bus
*
*  Parameters:
*           base_addr:   �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_disable_bus(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val &= ~SPI_CTL_EN;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_set_master
*
*  Description:
*                ����masterģʽ
*
*  Parameters:
*           base_addr:   �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_set_master(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val |= SPI_CTL_FUNC_MODE;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_set_slave
*
*  Description:
*                ����slaveģʽ
*
*  Parameters:
*           base_addr:   �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_set_slave(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val &= ~SPI_CTL_FUNC_MODE;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_set_tbw
*
*  Description:
*               ����spi�Ŀ��0��ʾ8bits��20��ֻ��8bit��û��16bit����32bit��
*
*  Parameters:
*           bits_per_word: 0-8bit��1-������20оƬ��֧��8bit����Ŀ�ȡ�
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_set_tbw(u32 bits_per_word, void *base_addr)
{   
/* 
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    u32 flag = (0x1 & bits_per_word);
    
    if(flag)
    {
       spi_wrn("aw1623 just support 8 bits per word!\n");
    }

    reg_val &= ~SPI_CTL_TBW;// 8BITS              //deleted for aw1623, this bit is defined for dma mode select, 2011-5-26 19:55:32
    writel(reg_val, base_addr + SPI_CTL_REG);
*/
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_disable_irq
*
*  Description:
*               �ر��ж�����
*
*  Parameters:
*           bitmap: �ж�λͼ
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_disable_irq(u32 bitmap, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_INT_CTL_REG);
    bitmap &= SPI_INTEN_MASK;
    reg_val &= ~bitmap;
    writel(reg_val, base_addr + SPI_INT_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_enable_irq
*
*  Description:
*              ʹ���ж�����
*
*  Parameters:
*           bitmap: �ж�λͼ
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_enable_irq(u32 bitmap, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_INT_CTL_REG);
    bitmap &= SPI_INTEN_MASK;
    reg_val |= bitmap;
    writel(reg_val, (base_addr + SPI_INT_CTL_REG));
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_disable_dma_irq
*
*  Description:
*              �ر�dma�ж�����
*
*  Parameters:
*           bitmap: �ж�λͼ
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_disable_dma_irq(u32 bitmap, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_DMA_CTL_REG);
    bitmap &= SPI_DRQEN_MASK;
    reg_val &= ~bitmap;
    writel(reg_val, base_addr + SPI_DMA_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_enable_dma_irq
*
*  Description:
*              ʹ��dma�ж�����
*
*  Parameters:
*           bitmap: �ж�λͼ
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_enable_dma_irq(u32 bitmap, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_DMA_CTL_REG);
    bitmap &= SPI_DRQEN_MASK;
    reg_val |= bitmap;
    writel(reg_val, base_addr + SPI_DMA_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_qry_irq_pending
*
*  Description:
*              ��ѯ�ж�pending
*
*  Parameters:
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
u32 aw_spi_qry_irq_pending(void *base_addr)
{
    return ( SPI_STAT_MASK & readl(base_addr + SPI_STATUS_REG) );
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_clr_irq_pending
*
*  Description:      
*              �����Ӧ�ж�pending
*
*  Parameters:
*           pending_bit:   �ж�pending
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_clr_irq_pending(u32 pending_bit, void *base_addr)
{
    pending_bit &= SPI_STAT_MASK;
    writel(pending_bit, base_addr + SPI_STATUS_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_query_txfifo
*
*  Description:      
*             ��ѯ����fifo���ֽ���
*
*  Parameters:
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
u32 aw_spi_query_txfifo(void *base_addr)
{
    u32 reg_val = ( SPI_FIFO_TXCNT & readl(base_addr + SPI_FIFO_STA_REG) );
    
    reg_val >>= SPI_TXCNT_BIT_POS;
    
    return reg_val;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_query_rxfifo
*
*  Description:      
*            ��ѯ����fifo���ֽ���
*
*  Parameters:
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
u32 aw_spi_query_rxfifo(void *base_addr)
{
    u32 reg_val = ( SPI_FIFO_RXCNT & readl(base_addr + SPI_FIFO_STA_REG) );
    
    reg_val >>= SPI_RXCNT_BIT_POS;
    
    return reg_val;
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_reset_fifo
*
*  Description:      
*            ��λtxFIFO��rxFIFO
*
*  Parameters:
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_reset_fifo(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    
    reg_val |= (SPI_CTL_RST_RXFIFO|SPI_CTL_RST_TXFIFO);
    
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_set_bc_wtc
*
*  Description:      
*           �����ܵĴ��䳤��BC�����÷��͵����ݳ���WTC,���Ϊ�������ݵĳ���
*           �ܵĴ��䳤�ȴ�С����Ϊ0xffffff���ܵķ��ͳ��ȴ�С����Ϊ0xffffff��
*           
*  Parameters:
*           tx_len :  ���ͳ���
*           rx_len :  ���ճ���
*           base_addr:     �Ĵ�������ַ
*       
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_set_bc_wtc(u32 tx_len, u32 rx_len, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_BC_REG);
    reg_val &= ~SPI_BC_BC_MASK;
    reg_val |= ( SPI_BC_BC_MASK & (tx_len+rx_len) );
    writel(reg_val, base_addr + SPI_BC_REG);    
    
//    spi_msg("\n-- BC = %d --\n", readl(base_addr + SPI_BC_REG));
    
    reg_val = readl(base_addr + SPI_TC_REG);
    reg_val &= ~SPI_TC_WTC_MASK;
    reg_val |= (SPI_TC_WTC_MASK & tx_len);
    writel(reg_val, base_addr + SPI_TC_REG);
//    spi_msg("\n-- TC = %d --\n", readl(base_addr + SPI_TC_REG));
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_ss_ctrl
*
*  Description:      
*                  Ƭѡ���ƣ��Ƿ���spi����������Ƭѡ�����ֶ����ơ�
*                   linux�½����ֶ����ƣ�melis�½����Զ����ƣ���Ϊlinux�£�ÿ�������Ϊtx��rx��
*                   ����ЩоƬҪ����һ����������в�����Ƭѡ�仯��������̽���linux��cs_activedʵ�֡�
*
*  Parameters:
*           base_addr:   �Ĵ�������ַ
*           on_off   :   0-automatic control chipselect level, 1: manual
*
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_ss_ctrl(void *base_addr, u32 on_off)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    on_off &= 0x1;
    if(on_off)
    {
        reg_val |= SPI_CTL_SS_CTRL;
    }
    else
    {
        reg_val &= ~SPI_CTL_SS_CTRL;
    }
    
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_ss_level
*
*  Description:      
*            ss_ctrl����������ӿڲ���Ч��
*
*  Parameters:
*           base_addr:     �Ĵ�������ַ
*           hi_lo    :  0:low level��   1:high level
*  Return value:
*          ��
*  Notes:
*   
****************************************************************************************************
*/
void aw_spi_ss_level(void *base_addr, u32 hi_lo)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    hi_lo &= 0x1;
    if(hi_lo)
    {
        reg_val |= SPI_CTL_SS_LEVEL;
    }
    else
    {
        reg_val &= ~SPI_CTL_SS_LEVEL;
    }
    
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_set_waitclk_cnt
*
*  Description:      
*                   wait clock counter 
*               ��ÿ��data transfer �У�����N���ȴ�״̬��spiģ�����SPI_SCLK��ΪN��WCC��
*               ����һ��data transfer���ӳ١�
*
*  Parameters:
*           base_addr    :     �Ĵ�������ַ
*           waitclk_cnt  :  0:low level��   1:high level
*  Return value:
*          ��
*  Notes:
*           ������maser mode ��Ч��
****************************************************************************************************
*/
void aw_spi_set_waitclk_cnt(u32 waitclk_cnt, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_WAIT_REG);
    reg_val &= ~SPI_WAIT_CLK_MASK;
    waitclk_cnt &= SPI_WAIT_CLK_MASK;
    reg_val |= waitclk_cnt;
    writel(reg_val, base_addr + SPI_WAIT_REG);
}

/*
****************************************************************************************************
*
*  FunctionName:           aw_spi_set_sample_delay
*
*  Description:      
*                   ���ٴ�����ʹ�á�������ʱ��
*
*  Parameters:
*           base_addr :  �Ĵ�������ַ
*           on_off    :  1: �������ٴ���ģʽ��0: ���ٴ���ģʽ��һ��10M���ҡ�  
*  Return value:
*          ��
*  Notes:
*           
****************************************************************************************************
*/
void aw_spi_set_sample_delay(u32 on_off, void *base_addr)
{
    u32 reg_val = readl(base_addr+SPI_CTL_REG);
    reg_val &= ~SPI_CTL_MASTER_SDC;
    reg_val |= on_off;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

