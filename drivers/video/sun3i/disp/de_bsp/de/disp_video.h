#ifndef __DISP_VIDEO_H_
#define __DISP_VIDEO_H_

#include "disp_display_i.h"

#define CASE_P_SOURCE 0
#define CASE_I_SAME_FRAME_RATE 1
#define CASE_I_DIFF_FRAME_RATE 2

typedef enum
{
    DIT_MODE_WEAVE = 0, 
    DIT_MODE_BOB = 1,
    DIT_MODE_MAF = 2,
    DIT_MODE_MAF_BOB = 3,
}dit_mode_t;

typedef struct frame_para
{
    __bool  enable;

	__disp_video_fb_t video_cur;
	__disp_video_fb_t video_new;
	__u32 pre_frame_addr0;

    __bool  have_got_frame;
    __u32   case_num;
	__bool	fetch_field;//for scaler
	__bool	fetch_bot;//for dit if dit enable,else for scaler
	__u32   field_cnt; //for dit:  counter of current display time after set framebuffer
	__bool	out_field;
	__bool	out_bot;
	__bool  dit_enable;
	dit_mode_t  dit_mode;
	__bool  tempdiff_en;
	__bool  diagintp_en;
	
}frame_para_t;


typedef struct tv_mode_info
{
	__u8	id;
	__s32	width;
	__s32	height;
	__bool	interlace;
	__s32	frame_rate;
	__s32	vb_line;
}tv_mode_info_t;

__s32 Video_Operation_In_Vblanking(__u32 sel);
extern frame_para_t g_video[2];


#endif
