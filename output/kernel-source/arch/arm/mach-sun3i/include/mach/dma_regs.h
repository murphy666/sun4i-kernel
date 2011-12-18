#ifndef _DMA_REGS_
#define _DMA_REGS_

#define SOFTWINNER_DMA_BASE             0x01c02000	/* DMA */

/* DMA Register definitions */
#define SW_DMA_DIRQEN      (0x0000)
#define SW_DMA_DIRQPD      (0x0004)
#define SW_DMA_DPRIO       (0x0008)

#define SW_DMA_DCONF       (0x00)
#define SW_DMA_DSRC        (0x04)
#define SW_DMA_DDST        (0x08)
#define SW_DMA_DCNT        (0x0C)
#define SW_DMA_DPSIZE      (0x10)
#define SW_DMA_DPSTEP      (0x14)

/* For F20: DDMA parameter register */
#define SW_DMA_DCMBK       (0x18)

#define SW_DCONF_LOADING   (1<<31)
#define SW_DCONF_BUSY      (1<<30)
#define SW_DCONF_CONTI     (1<<29)
#define SW_DCONF_WAIT      (7<<26)
#define SW_DCONF_DSTDW     (3<<24)    /* destination data width */
#define SW_DCONF_DWBYTE    (0<<24)
#define SW_DCONF_DWHWORD   (1<<24)
#define SW_DCONF_DWWORD    (2<<24)
#define SW_DCONF_DSTBL     (1<<23)    /* destination burst lenght */
#define SW_DCONF_DSTAT     (3<<21)    /* destination address type */
#define SW_DCONF_DSTTP     (31<<21)    /* destination type */
#define SW_DCONF_SRCDW     (3<< 8)    /* source data width */
#define SW_DCONF_SWBYTE    (0<< 8)
#define SW_DCONF_SWHWORD   (1<< 8)
#define SW_DCONF_SWWORD    (2<< 8)
#define SW_DCONF_SRCBL     (1<< 7)    /* source burst lenght */
#define SW_DCONF_SRCAT     (3<< 5)    /* source address type */
#define SW_DCONF_SRCTP     (31<<0)    /* source type */

#endif    // #ifndef _DMA_REGS_
