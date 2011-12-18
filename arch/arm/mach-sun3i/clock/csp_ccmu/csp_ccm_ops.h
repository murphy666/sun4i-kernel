/* 
******************************************************************************* 
*           				eBase 
*                 the Abstract of Hardware 
*                     
* 
*              (c) Copyright 2006-2010, ALL WINNER TECH. 
*           								All Rights Reserved 
* 
* File     :  d:\winners\eBase\eBSP\CSP\sun_20\HW_CCMU\ccm.h 
* Date     :  2010/11/13 9:47 
* By       :  Sam.Wu
* Version  :  V1.00 
* Description :  The Clock Control Module of the CSP
* Update   :  date      author      version     notes    
******************************************************************************* 
*/ 
/************************************************************************/
/* This CCM is divided into 2 parts: System clocks and Module clocks .
 * All the clocks constitute a tree named clock tree, then the System clocks 
 * are the internal nodes of the tree, and the Module clocks are the leaves.
 * The Frequencies of the leaves are sourced from internal nodes, As a result,
 * If the frequency of the System clock changes, all its children's frequencies
 * will changed! */
/************************************************************************/
#ifndef _CSP_CCM_OPS_H_
#define _CSP_CCM_OPS_H_


/*********************************************************************
* Method	 :    		CSP_CCM_init
* Description: You must call this method to init this CCMU module
* Parameters : 
    
* Returns    :   EBSP_TRUE/EBSP_FALSE
* Note       :
*********************************************************************/
__s32 CSP_CCM_init(void);

/*********************************************************************
* Method	 :    		CSP_CCM_exit
* Description: You must call this method before uninstall this CCMU module
* Parameters : none
* Returns    :  EBSP_TRUE/EBSP_FALSE
* Note       : 
*********************************************************************/
__s32 CSP_CCM_exit(void);


/*********************************************************************
* Method	 :    		CSP_CCM_set_sys_clk_freq
* Description:
* Parameters : 
	@CSP_CCM_sysClkNo_t sysClkNo 
	@const u32 outFreq 
* Returns    :   CSP_CCM_err_t
* Note       : CORE/VE PLL frequency range is [30,276]*1000*1000
*********************************************************************/
CSP_CCM_err_t     CSP_CCM_set_sys_clk_freq(CSP_CCM_sysClkNo_t sysClkNo, const u32 outFreq);


u32     CSP_CCM_get_sys_clk_freq(CSP_CCM_sysClkNo_t sysClkNo);

/*********************************************************************
* Method	 :    		CSP_CCM_set_sys_clk_freq_ex
* Description:Extended interface to set System clock frequency, recommended to used this
    interface CSP_CCM_set_sys_clk_freq other than if the sysClk is not oscillate and PLL.
* Parameters : 
@CSP_CCM_sysClkNo_t sysClkNo : clock number you want to set
@CSP_CCM_sysClkNo_t srcClkNo : source clock
@const u32 divideRatio 
* Returns    :   CSP_CCM_err_t
* Note       :If there is only one source clock, the @srcClkNo can be any value and it will 
be ignored!!
*********************************************************************/
CSP_CCM_err_t     CSP_CCM_set_sys_clk_freq_ex(CSP_CCM_sysClkNo_t sysClkNo, 
                                              CSP_CCM_sysClkNo_t srcClkNo,
                                              const u32 divideRatio);

s32     CSP_CCM_set_sys_clk_status(CSP_CCM_sysClkNo_t sysClkNo, u8 onOrOff);
s32  CSP_CCM_get_sys_clk_status(CSP_CCM_sysClkNo_t sysClkNo);

/*********************************************************************
* Method	 :    		CSP_CCM_set_mod_clk_freq
* Description:
* Parameters : 
	@const CSP_CCM_modClk_t * specClk 
* Returns    :   s32
* Note       :
*********************************************************************/
s32 CSP_CCM_set_mod_clk_freq(const CSP_CCM_modClkPara_t* modClk);

u32 CSP_CCM_get_mod_clk_freq(CSP_CCM_modClkNo_t modClkNo);

s32     CSP_CCM_set_mod_clk_status(CSP_CCM_modClkNo_t modClkNo, s32 onOrOff);
s32  CSP_CCM_get_mod_clk_status(CSP_CCM_modClkNo_t modClkNo);


u32         CSP_CCM_get_sys_clk_total_num(void);
u32         CSP_CCM_get_mod_clk_total_num(void);

CSP_CCM_err_t   CSP_CCM_get_sys_clk_info(CSP_CCM_sysClkNo_t clkNo, CSP_CCM_sysClkInfo_t* pInfo);
CSP_CCM_err_t   CSP_CCM_get_mod_clk_info(CSP_CCM_modClkNo_t clkNo, CSP_CCM_modClkInfo_t* pInfo);


/*********************************************************************
* Method	 :    		CSP_CCM_mod_clk_reset_control
* Description:Reset control 
* Parameters : 
	@CSP_CCM_modClkNo_t clkNo 
	@s32 resetIsValid 
* Returns    :   CSP_CCM_err_t
* Note       : clock who has reset control:
                CSP_CCM_MOD_CLK_USB_PHY0/CSP_CCM_MOD_CLK_USB_PHY1
                CSP_CCM_MOD_CLK_USB_PHY2
                CSP_CCM_MOD_CLK_VE/CSP_CCM_MOD_CLK_ACE/CSP_CCM_MOD_CLK_COM
                CSP_CCM_MOD_CLK_DE_IMAGE1/CSP_CCM_MOD_CLK_DE_IMAGE0/CSP_CCM_MOD_CLK_DE_SCAL1/CCM_MOD_CLK_DE_SCAL0
*********************************************************************/
CSP_CCM_err_t CSP_CCM_mod_clk_reset_control(CSP_CCM_modClkNo_t clkNo,s32 resetIsValid);

#endif //#ifndef _CSP_CCM_OPS_H_


