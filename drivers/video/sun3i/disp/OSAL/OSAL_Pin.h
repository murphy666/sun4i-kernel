/*
*************************************************************************************
*                         			eBsp
*					   Operation System Adapter Layer
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: OSAL_Pin.h
*
* Author 		: javen
*
* Description 	: C�⺯��
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-07          1.0         create this word
*       holi     	   2010-12-02          1.1         ��Ӿ���Ľӿڣ�
*************************************************************************************
*/
#ifndef  __OSAL_PIN_H__
#define  __OSAL_PIN_H__




/*
****************************************************************************************************
*
*             CSP_PIN_DEV_req
*
*  Description:
*       �豸ĳ���߼��豸��PIN��������������ĳ���豸�Ĳ���PIN��
*
*  Parameters:
* 		dev_id			:	�߼��豸��
*		p_log_pin_list	:	�߼�PIN��list����SPI_LOG_PINS{}
*		pin_nr			:	�߼�PIN����Ŀ
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
****************************************************************************************************
*/
__hdle OSAL_PIN_DEV_req(LOG_DEVS_t dev_id ,u32 * p_log_pin_list ,u32 pin_nr);


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
*
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
****************************************************************************************************
*/
s32 OSAL_PIN_DEV_release(__hdle handle);


/*
**********************************************************************************************************************
*                                               CSP_PIN_DEV_pull_ops
*
* Description:
*				���/д�� ĳ�߼��豸���߼�PIN������״̬
* Arguments  :
*		p_handler	:	handler
*		log_pin_id	:	�߼�pin��ID
*		pull_state	:	������״̬
*		is_get		:	!=0	:get	/	==0	:set
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32  OSAL_PIN_DEV_pull_ops(__hdle handle , u32 log_pin_id ,pin_pull_t * pull_state , u32 is_get);


/*
**********************************************************************************************************************
*                                               CSP_PIN_DEV_multi_drv_ops
*
* Description:
*		���/д�� ĳ�߼��豸���߼�PIN��������
* Arguments  :
*		p_handler	:	handler
*		log_pin_id	:	�߼�pin��ID
*		pull_state	:	����������״̬
*		is_get		:	!=0	:get	/	==0	:set
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32  OSAL_PIN_DEV_multi_drv_ops(__hdle handle , u32 log_pin_id ,pin_multi_driving_t * drive_level , u32 is_get);


/*
**********************************************************************************************************************
*                                               CSP_PIN_DEV_data_pos
*
* Description:
*		���/д�� ĳ�߼��豸���߼�PIN��������
* Arguments  :
*		p_handler	:	handler
*		log_pin_id	:	�߼�pin��ID
*		data		:	dataֵ�Ķ�ȡ/����
*		is_get		:	!=0	:get	/	==0	:set
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32 OSAL_PIN_DEV_data_ops(__hdle handle , u32 log_pin_id ,u32 * data , u32 is_get);


/*
**********************************************************************************************************************
*                                               CSP_PIN_DEV_direction_ops
*
* Description:
*		���/д�� ĳ�߼��豸���߼�PIN����
* Arguments  :
*		p_handler	:	handler
*		log_pin_id	:	�߼�pin��ID
*		data		:	����Ķ�ȡ/����
*		is_get		:	!=0	:get	/	==0	:set
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32 OSAL_PIN_DEV_direction_ops(__hdle handle , u32 log_pin_id ,u32 * is_output , u32 is_get);


/*
**********************************************************************************************************************
*                                               CSP_PIN_GPIO_req
*
* Description:
*		����ĳ��GPIO��Դ
* Arguments  :
*		phy_pin_group	:	�����Ǹ��飬���ﶼ������ģ��û���ֱ�ӿ�spec
*		phy_pin_offset	:	�����ڵ�ƫ��
*
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
__hdle  OSAL_PIN_GPIO_req(u8 phy_pin_group, u32 phy_pin_offset );


/*
**********************************************************************************************************************
*                                               CSP_PIN_GPIO_release
*
* Description:
*		�ͷ�ĳ��GPIO
* Arguments  :
*		p_handler	:	handler
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32  OSAL_PIN_GPIO_release(__hdle handle);

/*
**********************************************************************************************************************
*                                               CSP_PIN_GPIO_pull_ops
*
* Description:
*		���/д�� ĳGPIO��������״̬
* Arguments  :
*		p_handler	:	handler
*		pull_state	:	������״̬
*		is_get		:	!=0	:get	/	==0	:set
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32  OSAL_PIN_GPIO_pull_ops(__hdle handle  ,pin_pull_t * pull_state , u32 is_get);

/*
**********************************************************************************************************************
*                                               CSP_PIN_GPIO_multi_drv_ops
*
* Description:
*		���/д�� ĳGPIO������������״̬
* Arguments  :
*		p_handler	:	handler
*		drive_level	:	��������
*		is_get		:	!=0	:get	/	==0	:set
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32  OSAL_PIN_GPIO_multi_drv_ops(__hdle handle  ,pin_multi_driving_t * drive_level , u32 is_get);


/*
**********************************************************************************************************************
*                                               CSP_PIN_GPIO_data_pos
*
* Description:
*		���/д�� ĳGPIO��data
* Arguments  :
*		p_handler	:	handler
*		data		:	����
*		is_get		:	!=0	:get	/	==0	:set
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32 OSAL_PIN_GPIO_data_ops(__hdle handle ,u32 * data , u32 is_get);


/*
**********************************************************************************************************************
*                                               CSP_PIN_GPIO_direction_ops
*
* Description:
*		���/д�� ĳGPIO�ķ���
* Arguments  :
*		p_handler	:	handler
*		is_output	:	����
*		is_get		:	!=0	:get	/	==0	:set
* Returns    :
*		EBSP_TRUE/EBSP_FALSE
* Notes      :
*
**********************************************************************************************************************
*/
s32 OSAL_PIN_GPIO_direction_ops(__hdle handle ,u32 * is_output , u32 is_get);


/*
**********************************************************************************************************************
*                                               CSP_PIN_GPIO_direction_ops
*
* Description:
*		�����߼��豸�����߼�PIN��������������group������offset(�ȷ���������PIN��λ��)
* Arguments  :
*		log_dev_name	:	�豸��
*		log_pin_name	:	�߼�pin������
*		phy_pin_group	:	����pin��group���룬��GPIOC16�е�C��
*		phy_pin_offset	:	����pin��ƫ�ƺţ���GPIOC16�е�16
* Returns    :
*		EBSP_FALSE	:	ʧ��
*		EBSP_TRUE	:	�ɹ�
* Notes      :
*
**********************************************************************************************************************
*/
s32 OSAL_PIN_MISC_get_phy_pin(u8 * log_dev_name , u8 * log_pin_name , u32 * phy_pin_group, u32 * phy_pin_offset);


#endif   //__OSAL_PIN_H__

