/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

/*
 * Public  General Config
 */
#define AUTOCONF_INCLUDED
#define RTL871X_MODULE_NAME "92CU"
#define DRV_NAME "rtl8192cu"

#define CONFIG_USB_HCI	1

#define CONFIG_RTL8192C	1

#define PLATFORM_LINUX	1


/*
 * Internal  General Config
 */
//#define CONFIG_PWRCTRL
//#define CONFIG_H2CLBK

#define CONFIG_EMBEDDED_FWIMG	1

#define CONFIG_R871X_TEST	1

#define CONFIG_80211N_HT	1

#define CONFIG_RECV_REORDERING_CTRL	1

//#define CONFIG_TCP_CSUM_OFFLOAD_RX	1

//#define CONFIG_DRVEXT_MODULE	1

#ifndef CONFIG_MP_INCLUDED
#define CONFIG_IPS	1
#ifdef CONFIG_IPS
	//#define CONFIG_IPS_LEVEL_2	1 //enable this to set default IPS mode to IPS_LEVEL_2
#endif
#define SUPPORT_HW_RFOFF_DETECTED	1

#define CONFIG_LPS	1
#define CONFIG_BT_COEXIST	1
//befor link
#define CONFIG_ANTENNA_DIVERSITY	 	
//after link
#ifdef CONFIG_ANTENNA_DIVERSITY
#define CONFIG_SW_ANTENNA_DIVERSITY	 
//#define CONFIG_HW_ANTENNA_DIVERSITY	
#endif

#else
#define CONFIG_MP_IWPRIV_SUPPORT	1
#endif

#define CONFIG_AP_MODE	1
#define CONFIG_NATIVEAP_MLME	1

//	Added by Albert 20110314
#define CONFIG_P2P	1

//	Added by Kurt 20110511
//#define CONFIG_TDLS	1
			
#ifdef CONFIG_AP_MODE
	#ifndef CONFIG_NATIVEAP_MLME
		#define CONFIG_HOSTAPD_MLME	1
	#endif			
	//#define CONFIG_FIND_BEST_CHANNEL	1
#endif

#define CONFIG_SKB_COPY	1//for amsdu

#define CONFIG_LED
#ifdef CONFIG_LED
	#define CONFIG_SW_LED
#endif //CONFIG_LED

#define CONFIG_HANDLE_JOINBSS_ON_ASSOC_RSP

#define USB_INTERFERENCE_ISSUE // this should be checked in all usb interface
#define CONFIG_GLOBAL_UI_PID

#define CONFIG_LAYER2_ROAMING
#define CONFIG_LAYER2_ROAMING_RESUME
//#define CONFIG_ADAPTOR_INFO_CACHING_FILE // now just applied on 8192cu only, should make it general...
//#define CONFIG_RESUME_IN_WORKQUEUE
//#define CONFIG_SET_SCAN_DENY_TIMER
#define CONFIG_LONG_DELAY_ISSUE
#define CONFIG_NEW_SIGNAL_STAT_PROCESS

/*
 * Interface  Related Config
 */
//#define CONFIG_USB_INTERRUPT_IN_PIPE	1

#ifndef CONFIG_MINIMAL_MEMORY_USAGE
	#define CONFIG_USB_TX_AGGREGATION	1
	#define CONFIG_USB_RX_AGGREGATION	1
#endif

#define CONFIG_PREALLOC_RECV_SKB	1
#define CONFIG_REDUCE_USB_TX_INT	1
//#define CONFIG_EASY_REPLACEMENT	1

/* 
 * CONFIG_USE_USB_BUFFER_ALLOC_XX uses Linux USB Buffer alloc API and is for Linux platform only now!
 */
#define CONFIG_USE_USB_BUFFER_ALLOC_TX 1	// For TX path
//#define CONFIG_USE_USB_BUFFER_ALLOC_RX 1	// For RX path

#define CONFIG_USB_VENDOR_REQ_PREALLOC 1

//#define CONFIG_USB_SUPPORT_ASYNC_VDN_REQ 1


/*
 * HAL  Related Config
 */

#define RTL8192C_RX_PACKET_NO_INCLUDE_CRC	1

#define SUPPORTED_BLOCK_IO



#define RTL8192CU_FW_DOWNLOAD_ENABLE	1

#define CONFIG_ONLY_ONE_OUT_EP_TO_LOW	0

#define CONFIG_OUT_EP_WIFI_MODE	0

#define ENABLE_USB_DROP_INCORRECT_OUT	0

#define RTL8192CU_ASIC_VERIFICATION	0	// For ASIC verification.

#define RTL8192CU_ADHOC_WORKAROUND_SETTING	1

#define DISABLE_BB_RF	0

#define RTL8191C_FPGA_NETWORKTYPE_ADHOC 0

#ifdef CONFIG_MP_INCLUDED
	#define MP_DRIVER 1
	#undef CONFIG_USB_TX_AGGREGATION
	#undef CONFIG_USB_RX_AGGREGATION
#else
	#define MP_DRIVER 0
#endif


/*
 * Platform  Related Config
 */
#ifdef CONFIG_PLATFORM_MN10300
#define CONFIG_SPECIAL_SETTING_FOR_FUNAI_TV

#if	defined (CONFIG_SW_ANTENNA_DIVERSITY)
	#undef CONFIG_SW_ANTENNA_DIVERSITY
	#define CONFIG_HW_ANTENNA_DIVERSITY
#endif

#endif

#ifndef CONFIG_WISTRON_PLATFORM
//#define CONFIG_DYNAMIC_ALLOCIATE_VENDOR_CMD	1
#define SILENT_RESET_FOR_SPECIFIC_PLATFOM	1
#endif

#ifdef CONFIG_PLATFORM_TI_DM365
#define CONFIG_SPECIFIC_URB_NUM
#endif


/*
 * Debug  Related Config
 */
//#define CONFIG_DEBUG_RTL871X

#define DBG	0
#define CONFIG_DEBUG_RTL819X

#define CONFIG_PROC_DEBUG	1

//#define DBG_IO
//#define DBG_DELAY_OS
//#define DBG_MEM_ALLOC
//#define DBG_IOCTL

//#define DBG_TX
//#define DBG_XMIT_BUF
//#define DBG_TX_DROP_FRAME

//#define DBG_RX_DROP_FRAME
//#define DBG_RX_SEQ

//#define DBG_SHOW_MCUFWDL_BEFORE_51_ENABLE

