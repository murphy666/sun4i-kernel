/*
********************************************************************************************************
*                          SUN4I----HDMI AUDIO
*                   (c) Copyright 2002-2004, All winners Co,Ld.
*                          All Right Reserved
*
* FileName: sun4i-codecchip.h   author:chenpailin  date:2011-07-19 
* Description: 
* Others: 
* History:
*   <author>      <time>      <version>   <desc> 
*   chenpailin   2011-07-19     1.0      modify this module 
********************************************************************************************************
*/

#ifndef CODECCHIP_H_
#define CODECCHIP_H_

typedef struct hdmi_audio
{
	__u8    hw_intf;        /* 0:iis  1:spdif 2:pcm */
	__u16	fs_between;     /* fs */
	__u32   sample_rate;    /*sample rate*/  
	__u8    clk_edge;       /* 0:*/
	__u8    ch0_en;         /* 1 */
	__u8    ch1_en;         /* 0 */
	__u8 	ch2_en;         /* 0 */
	__u8 	ch3_en;         /* 0 */
	__u8	word_length;    /* 32 */
	__u8    shift_ctl;      /* 0 */
	__u8    dir_ctl;        /* 0 */
	__u8    ws_pol;
	__u8    just_pol;
}hdmi_audio_t;


typedef struct
{
    __s32 (*hdmi_audio_enable)(__u8 mode, __u8 channel);
    __s32 (*hdmi_set_audio_para)(hdmi_audio_t * audio_para);
}__audio_hdmi_func;


/*define display driver command*/
typedef enum tag_HDMI_CMD
{
    /* command cache on/off                         */
		HDMI_CMD_SET_VIDEO_MOD,
		HDMI_CMD_GET_VIDEO_MOD,
		HDMI_CMD_SET_AUDIO_PARA,
		HDMI_CMD_AUDIO_RESET_NOTIFY,            /*iis reset finish notify    */
		HDMI_CMD_CLOSE,                         /*iis reset finish notify    */
		HDMI_CMD_MOD_SUPPORT,                   /*�ж�ĳһ��hdmiģʽ�Ƿ�֧��*/
		HDMI_CMD_AUDIO_ENABLE,
		HDMI_CMD_GET_HPD_STATUS,
}__hdmi_cmd_t;

extern struct snd_soc_dai codecchip_dai;
extern struct snd_soc_codec_device soc_codec_dev_codecchip;

#endif
