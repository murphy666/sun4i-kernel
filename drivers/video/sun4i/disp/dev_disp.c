#include "dev_disp.h"

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

fb_info_t g_fbi;
__disp_drv_t g_disp_drv;

#define MY_BYTE_ALIGN(x) ( ( (x + (4*1024-1)) >> 12) << 12)             /* alloc based on 4K byte */
static struct alloc_struct_t boot_heap_head, boot_heap_tail;


#define __inf printk

static unsigned int gbuffer[4096];
static __u32 output_type[2] = {0,0};
static __u32 suspend_status = 0;//0:normal; suspend_status&1 != 0:in early_suspend; suspend_status&2 != 0:in suspend;

static struct info_mm  g_disp_mm[10];
static int g_disp_mm_sel = 0;

atomic_t cmd_index;

static struct cdev *my_cdev;
static dev_t devid ;
static struct class *disp_class;

static struct resource disp_resource[DISP_IO_NUM] =
{
	[DISP_IO_SCALER0] = {
		.start = 0x01e00000,
		.end   = 0x01e0077f,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_SCALER1] = {
		.start = 0x01e20000,
		.end   = 0x01e2077f,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_IMAGE0] = {
		.start = 0x01e60000,
		.end   = 0x01e657ff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_IMAGE1] = {
		.start = 0x01e40000,
		.end   = 0x01e457ff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_LCDC0] = {
		.start = 0x01c0c000,
		.end   = 0x01c0cfff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_LCDC1] = {
		.start = 0x01c0d000,
		.end   = 0x01c0dfff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_TVEC0] = {
		.start = 0x01c0a000,
		.end   = 0x01c0afff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_TVEC1] = {
		.start = 0x01c1b000,
		.end   = 0x01c1bfff,
		.flags = IORESOURCE_MEM,
	},
};


__s32 disp_create_heap(__u32 pHeapHead, __u32 nHeapSize)
{
    boot_heap_head.size    = boot_heap_tail.size = 0;
    boot_heap_head.address = pHeapHead;
    boot_heap_tail.address = pHeapHead + nHeapSize;
    boot_heap_head.next    = &boot_heap_tail;
    boot_heap_tail.next    = 0;

    __inf("head:%x,tail:%x\n" ,boot_heap_head.address, boot_heap_tail.address);
    return 0;
}

void *disp_malloc(__u32 num_bytes)
{
    struct alloc_struct_t *ptr, *newptr;
    __u32  actual_bytes;

    if (!num_bytes)
    {
        return 0;
    }

    actual_bytes = MY_BYTE_ALIGN(num_bytes);    /* translate the byte count to size of long type       */

    ptr = &boot_heap_head;                      /* scan from the boot_heap_head of the heap            */

    while (ptr && ptr->next)                    /* look for enough memory for alloc                    */
    {
        if (ptr->next->address >= (ptr->address + ptr->size + (8 * 1024) + actual_bytes))
        {
            break;
        }
                                                /* find enough memory to alloc                         */
        ptr = ptr->next;
    }

    if (!ptr->next)
    {
        __wrn(" it has reached the boot_heap_tail of the heap now\n");
        return 0;                   /* it has reached the boot_heap_tail of the heap now              */
    }

    newptr = (struct alloc_struct_t *)(ptr->address + ptr->size);
                                                /* create a new node for the memory block             */
    if (!newptr)
    {
        __wrn(" create the node failed, can't manage the block\n");
        return 0;                               /* create the node failed, can't manage the block     */
    }

    /* set the memory block chain, insert the node to the chain */
    newptr->address = ptr->address + ptr->size + 4*1024;
    newptr->size    = actual_bytes;
    newptr->o_size  = num_bytes;
    newptr->next    = ptr->next;
    ptr->next       = newptr;

    return (void *)newptr->address;
}

void  disp_free(void *p)
{
    struct alloc_struct_t *ptr, *prev;

	if( p == NULL )
		return;

    ptr = &boot_heap_head;                /* look for the node which po__s32 this memory block                     */
    while (ptr && ptr->next)
    {
        if (ptr->next->address == (__u32)p)
            break;              /* find the node which need to be release                              */
        ptr = ptr->next;
    }

	prev = ptr;
	ptr = ptr->next;

    if (!ptr) return;           /* the node is heap boot_heap_tail                                               */

    prev->next = ptr->next;     /* delete the node which need be released from the memory block chain  */

    return ;
}

__s32 DRV_lcd_open(__u32 sel)
{
    __u32 i = 0;
    __lcd_flow_t *flow;

	if(g_disp_drv.b_lcd_open[sel] == 0)
	{
	    BSP_disp_lcd_open_before(sel);

	    flow = BSP_disp_lcd_get_open_flow(sel);
	    for(i=0; i<flow->func_num; i++)
	    {
	        __u32 timeout = flow->func[i].delay*HZ/1000;

	        flow->func[i].func(sel);

	    	set_current_state(TASK_INTERRUPTIBLE);
	    	schedule_timeout(timeout);

	    }

	    BSP_disp_lcd_open_after(sel);

		g_disp_drv.b_lcd_open[sel] = 1;
	}

    return 0;
}

__s32 DRV_lcd_close(__u32 sel)
{
    __u32 i = 0;
    __lcd_flow_t *flow;

	if(g_disp_drv.b_lcd_open[sel] == 1)
	{
	    BSP_disp_lcd_close_befor(sel);

	    flow = BSP_disp_lcd_get_close_flow(sel);
	    for(i=0; i<flow->func_num; i++)
	    {
	        __u32 timeout = flow->func[i].delay*HZ/1000;

	        flow->func[i].func(sel);

	    	set_current_state(TASK_INTERRUPTIBLE);
	    	schedule_timeout(timeout);

	    }

	    BSP_disp_lcd_close_after(sel);

		g_disp_drv.b_lcd_open[sel] = 0;
	}
    return 0;
}

__s32 DRV_scaler_begin(__u32 sel)
{
    long timeout = (100 * HZ)/1000;//100ms

    g_disp_drv.b_scaler_finished[sel] = 1;
    timeout = wait_event_interruptible_timeout(g_disp_drv.scaler_queue[sel], g_disp_drv.b_scaler_finished[sel] == 2, timeout);
    g_disp_drv.b_scaler_finished[sel] = 0;
    if(timeout == 0)
    {
        __wrn("wait scaler %d finished timeout\n", sel);
        return -1;
    }
    return 0;
}

void DRV_scaler_finish(__u32 sel)
{
    if(g_disp_drv.b_scaler_finished[sel] == 1)
    {
        g_disp_drv.b_scaler_finished[sel] = 2;
        wake_up_interruptible(&g_disp_drv.scaler_queue[sel]);
    }
    else
    {
        __wrn("not scaler %d begin in DRV_scaler_finish\n", sel);
    }
}


void DRV_disp_wait_cmd_finish(__u32 sel)
{
#if 0
    sys_put_wvalue((__u32)g_fbi.io[DISP_IO_SCALER0] + 0x04, 0x00010001);
    while(1)
    {
        __u32 value = sys_get_wvalue((__u32)g_fbi.io[DISP_IO_SCALER0] + 0x64);
        if((value & (0x1 << 10)) != 0)
        {
            sys_put_wvalue((__u32)g_fbi.io[DISP_IO_SCALER0] + 0x64, (0x1 << 10));

            //value = sys_get_wvalue((__u32)g_fbi.io[DISP_IO_LCDC0] + 0xfc);

            //__inf("%d\n", (value & 0x03ff0000)>>16);
            break;
        }
    }
#endif

#if 0
    __inf("1:%d\n", (sys_get_wvalue((__u32)g_fbi.io[DISP_IO_LCDC0] + 0xfc) & 0x03ff0000) >> 16);
    sys_put_wvalue((__u32)g_fbi.io[DISP_IO_IMAGE0] + 0x870, 3);
    while(1)
    {
        __u32 value = sys_get_wvalue((__u32)g_fbi.io[DISP_IO_IMAGE0] + 0x870);
        if((value & 1) == 0)
        {
            break;
        }
    }
    __inf("2:%d\n", (sys_get_wvalue((__u32)g_fbi.io[DISP_IO_LCDC0] + 0xfc) & 0x03ff0000) >> 16);
#endif

#if 1
	__u32 timeout = (20 * HZ)/1000,ret;//20ms
	__u32 i;

    timeout = (HZ * 2) / BSP_disp_get_frame_rate(sel);//wait two frame

    if(g_disp_drv.b_cache[sel] == 0 && BSP_disp_get_output_type(sel)!= DISP_OUTPUT_TYPE_NONE)
    {
        if(atomic_read(&cmd_index) >= 20)
        {
            atomic_set(&cmd_index, 0);
        }
        i = atomic_read(&cmd_index);
        atomic_inc(&cmd_index);

        g_disp_drv.b_cmd_finished[sel][i] = 1;
    	ret = wait_event_interruptible_timeout(g_disp_drv.my_queue[sel][i], g_disp_drv.b_cmd_finished[sel][i] == 2, timeout);
    	g_disp_drv.b_cmd_finished[sel][i] = 0;
    	if(ret == 0)
        {
            __inf("timeout,%d,%d,%d\n", sel,BSP_disp_get_frame_rate(sel),timeout);
        }
    }
#endif
}

__s32 DRV_disp_int_process(__u32 sel)
{
#if 1
    __u32 i = 0;

    for(i=0; i<20; i++)
    {
        if(g_disp_drv.b_cmd_finished[sel][i] == 1)
        {
            g_disp_drv.b_cmd_finished[sel][i] = 2;
            wake_up_interruptible(&g_disp_drv.my_queue[sel][i]);
            break;
        }
    }
#endif
    return 0;
}

__s32 disp_set_hdmi_func(__disp_hdmi_func * func)
{
    BSP_disp_set_hdmi_func(func);

    return 0;
}

__s32 DRV_DISP_Init(void)
{
    __disp_bsp_init_para para;
    __u32 i = 0;

    memset(&para, 0, sizeof(__disp_bsp_init_para));
    para.base_image0    = (__u32)g_fbi.io[DISP_IO_IMAGE0];
    para.base_image1    = (__u32)g_fbi.io[DISP_IO_IMAGE1];
    para.base_scaler0   = (__u32)g_fbi.io[DISP_IO_SCALER0];
    para.base_scaler1   = (__u32)g_fbi.io[DISP_IO_SCALER1];
    para.base_lcdc0     = (__u32)g_fbi.io[DISP_IO_LCDC0];
    para.base_lcdc1     = (__u32)g_fbi.io[DISP_IO_LCDC1];
    para.base_tvec0      = (__u32)g_fbi.io[DISP_IO_TVEC0];
    para.base_tvec1      = (__u32)g_fbi.io[DISP_IO_TVEC1];
    para.base_ccmu      = (__u32)g_fbi.base_ccmu;
    para.base_sdram     = (__u32)g_fbi.base_sdram;
    para.base_pioc      = (__u32)g_fbi.base_pioc;
    para.base_pwm       = (__u32)g_fbi.base_pwm;
    para.scaler_begin   		= DRV_scaler_begin;
    para.scaler_finish  		= DRV_scaler_finish;
	para.disp_int_process       = DRV_disp_int_process;

	memset(&g_disp_drv, 0, sizeof(__disp_drv_t));

    for(i=0; i<20; i++)
    {
        init_waitqueue_head(&g_disp_drv.my_queue[0][i]);
        init_waitqueue_head(&g_disp_drv.my_queue[1][i]);
    }
    init_waitqueue_head(&g_disp_drv.scaler_queue[0]);
    init_waitqueue_head(&g_disp_drv.scaler_queue[1]);

    BSP_disp_init(&para);
    BSP_disp_open();

    return 0;
}

__s32 DRV_DISP_Exit(void)
{
    Fb_Exit();
    BSP_disp_close();
    BSP_disp_exit(g_disp_drv.exit_mode);

    return 0;
}


int disp_mem_request(int sel,__u32 size)
{
#ifndef FB_RESERVED_MEM
	unsigned map_size = 0;
	struct page *page;

	if(g_disp_mm[sel].info_base != 0)
		return -EINVAL;

	g_disp_mm[sel].mem_len = size;
	map_size = PAGE_ALIGN(g_disp_mm[sel].mem_len);

	page = alloc_pages(GFP_KERNEL,get_order(map_size));
	if(page != NULL)
	{
		g_disp_mm[sel].info_base = page_address(page);
		if(g_disp_mm[sel].info_base == 0)
		{
			free_pages((unsigned long)(page),get_order(map_size));
			__wrn("page_address fail!\n");
			return -ENOMEM;
		}
		g_disp_mm[sel].mem_start = virt_to_phys(g_disp_mm[sel].info_base);
		memset(g_disp_mm[sel].info_base,0,size);

		__inf("pa=0x%08lx va=0x%p size:0x%x\n",g_disp_mm[sel].mem_start, g_disp_mm[sel].info_base, size);
		return 0;
	}
	else
	{
		__wrn("alloc_pages fail!\n");
		return -ENOMEM;
	}
#else
    __u32 ret = 0;

	ret = (__u32)disp_malloc(size);
	if(ret != 0)
	{
	    g_disp_mm[sel].info_base = (void*)ret;
	    g_disp_mm[sel].mem_start = virt_to_phys(g_disp_mm[sel].info_base);
	    memset(g_disp_mm[sel].info_base,0,size);
	    __inf("pa=0x%08lx va=0x%p size:0x%x\n",g_disp_mm[sel].mem_start, g_disp_mm[sel].info_base, size);

	    return 0;
	}
	else
	{
		__wrn("disp_malloc fail!\n");
		return -ENOMEM;
	}
#endif

}

int disp_mem_release(int sel)
{
#ifndef FB_RESERVED_MEM
	unsigned map_size = PAGE_ALIGN(g_disp_mm[sel].mem_len);
	unsigned page_size = map_size;

	if(g_disp_mm[sel].info_base == 0)
		return -EINVAL;

	free_pages((unsigned long)(g_disp_mm[sel].info_base),get_order(page_size));
	memset(&g_disp_mm[sel],0,sizeof(struct info_mm));
#else
	if(g_disp_mm[sel].info_base == 0)
		return -EINVAL;

    disp_free((void *)g_disp_mm[sel].info_base);
    memset(&g_disp_mm[sel],0,sizeof(struct info_mm));
#endif

	return 0;

}

int disp_mmap(struct file *file, struct vm_area_struct * vma)
{
	unsigned long  physics =  g_disp_mm[g_disp_mm_sel].mem_start;// - PAGE_OFFSET;
	unsigned long mypfn = physics >> PAGE_SHIFT;
	unsigned long vmsize = vma->vm_end-vma->vm_start;

	if(remap_pfn_range(vma,vma->vm_start,mypfn,vmsize,vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

int disp_open(struct inode *inode, struct file *file)
{
    return 0;
}

int disp_release(struct inode *inode, struct file *file)
{
    return 0;
}
ssize_t disp_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}

ssize_t disp_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    return 0;
}

static int __init disp_probe(struct platform_device *pdev)//called when platform_driver_register
{
	fb_info_t * info = NULL;
	struct resource *res;
	int ret = 0;
	int size;
	int i;

	__inf("disp_probe call\n");

	info = &g_fbi;

	info->dev = &pdev->dev;
	platform_set_drvdata(pdev,info);

	for(i=0;i<DISP_IO_NUM;i++)
	{
		res = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (res == NULL)
		{
			__wrn("platform_get_resource fail\n");
			ret = -ENXIO;
			if(i==DISP_IO_SCALER0)
			{
				goto dealloc_fb;
			}
			else if(i==DISP_IO_SCALER1)
			{
				goto release_regs0;
			}
			else if(i==DISP_IO_IMAGE0)
			{
				goto release_regs1;
			}
			else if(i==DISP_IO_IMAGE1)
			{
				goto release_regs2;
			}
			else if(i==DISP_IO_LCDC0)
			{
				goto release_regs3;
			}
			else if(i==DISP_IO_LCDC1)
			{
				goto release_regs4;
			}
			else if(i==DISP_IO_TVEC0)
			{
				goto release_regs5;
			}
			else if(i==DISP_IO_TVEC1)
			{
				goto release_regs6;
			}
		}

		size = (res->end - res->start) + 1;
		info->mem[i] = request_mem_region(res->start, size, pdev->name);
		if (info->mem[i] == NULL)
		{
			__wrn("request_mem_region fail\n");
			ret = -ENOENT;
			if(i==DISP_IO_SCALER0)
			{
				goto dealloc_fb;
			}
			else if(i==DISP_IO_SCALER1)
			{
				goto release_regs0;
			}
			else if(i==DISP_IO_IMAGE0)
			{
				goto release_regs1;
			}
			else if(i==DISP_IO_IMAGE1)
			{
				goto release_regs2;
			}
			else if(i==DISP_IO_LCDC0)
			{
				goto release_regs3;
			}
			else if(i==DISP_IO_LCDC1)
			{
				goto release_regs4;
			}
			else if(i==DISP_IO_TVEC0)
			{
				goto release_regs5;
			}
			else if(i==DISP_IO_TVEC1)
			{
				goto release_regs6;
			}
		}

		info->io[i] = ioremap(res->start, size);
		if (info->io[i] == NULL)
		{
			__wrn("ioremap() fail\n");
			ret = -ENXIO;
			if(i==DISP_IO_SCALER0)
			{
				goto release_mem0;
			}
			else if(i==DISP_IO_SCALER1)
			{
				goto release_mem1;
			}
			else if(i==DISP_IO_IMAGE0)
			{
				goto release_mem2;
			}
			else if(i==DISP_IO_IMAGE1)
			{
				goto release_mem3;
			}
			else if(i==DISP_IO_LCDC0)
			{
				goto release_mem4;
			}
			else if(i==DISP_IO_LCDC1)
			{
				goto release_mem5;
			}
			else if(i==DISP_IO_TVEC0)
			{
				goto release_mem6;
			}
			else if(i==DISP_IO_TVEC1)
			{
				goto release_mem7;
			}
	    }
	}

	info->base_ccmu = 0xf1c20000;
	info->base_sdram = 0xf1c01000;
	info->base_pioc = 0xf1c20800;
	info->base_pwm = 0xf1c20c00;

	__inf("SCALER0 base 0x%08x\n", (__u32)info->io[DISP_IO_SCALER0]);
	__inf("SCALER1 base 0x%08x\n", (__u32)info->io[DISP_IO_SCALER1]);
	__inf("IMAGE0 base 0x%08x\n", (__u32)info->io[DISP_IO_IMAGE0] + 0x800);
	__inf("IMAGE1 base 0x%08x\n", (__u32)info->io[DISP_IO_IMAGE1] + 0x800);
	__inf("LCDC0 base 0x%08x\n", (__u32)info->io[DISP_IO_LCDC0]);
	__inf("LCDC1 base 0x%08x\n", (__u32)info->io[DISP_IO_LCDC1]);
	__inf("TVEC0 base 0x%08x\n", (__u32)info->io[DISP_IO_TVEC0]);
	__inf("TVEC1 base 0x%08x\n", (__u32)info->io[DISP_IO_TVEC1]);
	__inf("CCMU base 0x%08x\n", info->base_ccmu);
	__inf("SDRAM base 0x%08x\n", info->base_sdram);
	__inf("PIO base 0x%08x\n", info->base_pioc);
	__inf("PWM base 0x%08x\n", info->base_pwm);


	return 0;

release_mem7:
	release_resource(info->mem[7]);
	kfree(info->mem[7]);

release_regs6:
	iounmap(info->io[6]);
release_mem6:
	release_resource(info->mem[6]);
	kfree(info->mem[6]);

release_regs5:
	iounmap(info->io[5]);
release_mem5:
	release_resource(info->mem[5]);
	kfree(info->mem[5]);

release_regs4:
	iounmap(info->io[4]);
release_mem4:
	release_resource(info->mem[4]);
	kfree(info->mem[4]);

release_regs3:
	iounmap(info->io[3]);
release_mem3:
	release_resource(info->mem[3]);
	kfree(info->mem[3]);

release_regs2:
	iounmap(info->io[2]);
release_mem2:
	release_resource(info->mem[2]);
	kfree(info->mem[2]);

release_regs1:
	iounmap(info->io[1]);
release_mem1:
	release_resource(info->mem[1]);
	kfree(info->mem[1]);

release_regs0:
	iounmap(info->io[0]);
release_mem0:
	release_resource(info->mem[0]);
	kfree(info->mem[0]);

dealloc_fb:
	platform_set_drvdata(pdev, NULL);
	kfree(info);

	return ret;
}



static int disp_remove(struct platform_device *pdev)
{
	fb_info_t *info = platform_get_drvdata(pdev);
	int i;

	__inf("disp_remove call\n");

	for(i=0;i<DISP_IO_NUM - 3;i++)
	{
		iounmap(info->io[i]);

		release_resource(info->mem[i]);
		kfree(info->mem[i]);
	}

	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
void backlight_early_suspend(struct early_suspend *h)
{
    int i = 0;

    printk("display early suspend: %s\n", __func__);

    for(i=0; i<2; i++)
    {
        output_type[i] = BSP_disp_get_output_type(i);
        if(output_type[i] == DISP_OUTPUT_TYPE_LCD)
        {
            DRV_lcd_close(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_TV)
        {
            BSP_disp_tv_close(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_VGA)
        {
            BSP_disp_vga_close(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_HDMI)
        {
            BSP_disp_hdmi_close(i);
        }
    }

    BSP_disp_clk_off(2);

    suspend_status |= 1;
}

void backlight_late_resume(struct early_suspend *h)
{
    int i = 0;

    printk("display late resume enter: %s\n", __func__);

    BSP_disp_clk_on(2);

    for(i=0; i<2; i++)
    {
        if(output_type[i] == DISP_OUTPUT_TYPE_LCD)
        {
            DRV_lcd_open(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_TV)
        {
            BSP_disp_tv_open(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_VGA)
        {
            BSP_disp_vga_open(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_HDMI)
        {
            BSP_disp_hdmi_open(i);
        }
    }

    suspend_status &= (~1);

    printk("display late resume done: %s\n", __func__);
}

static struct early_suspend backlight_early_suspend_handler =
{
    .level   = EARLY_SUSPEND_LEVEL_DISABLE_FB,
	.suspend = backlight_early_suspend,
	.resume = backlight_late_resume,
};

#endif

int disp_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifndef CONFIG_HAS_EARLYSUSPEND
    int i = 0;

    __inf("disp_suspend call\n");

    for(i=0; i<2; i++)
    {
        output_type[i] = BSP_disp_get_output_type(i);
        if(output_type[i] == DISP_OUTPUT_TYPE_LCD)
        {
            DRV_lcd_close(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_TV)
        {
            BSP_disp_tv_close(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_VGA)
        {
            BSP_disp_vga_close(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_HDMI)
        {
            BSP_disp_hdmi_close(i);
        }
    }
    BSP_disp_clk_off(3);
#else
    BSP_disp_clk_off(1);
#endif

    suspend_status |= 2;

    return 0;
}

int disp_resume(struct platform_device *pdev)
{
#ifndef CONFIG_HAS_EARLYSUSPEND
    int i = 0;

    __inf("disp_resume call\n");

    BSP_disp_clk_on(3);

    for(i=0; i<2; i++)
    {
        if(output_type[i] == DISP_OUTPUT_TYPE_LCD)
        {
            DRV_lcd_open(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_TV)
        {
            BSP_disp_tv_open(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_VGA)
        {
            BSP_disp_vga_open(i);
        }
        else if(output_type[i] == DISP_OUTPUT_TYPE_HDMI)
        {
            BSP_disp_hdmi_open(i);
        }
    }
#else
    BSP_disp_clk_on(1);
#endif

    suspend_status &= (~2);

    return 0;
}

void disp_shutdown(struct platform_device *pdev)
{
    __u32 type = 0, i = 0;

    for(i=0; i<2; i++)
    {
        type = BSP_disp_get_output_type(i);
        if(type == DISP_OUTPUT_TYPE_LCD)
        {
            DRV_lcd_close(i);
        }
    }
}

long disp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned long karg[4];
	unsigned long ubuffer[4] = {0};
	__s32 ret = 0;

	if (copy_from_user((void*)karg,(void __user*)arg,4*sizeof(unsigned long)))
	{
		__wrn("copy_from_user fail\n");
		return -EFAULT;
	}

	ubuffer[0] = *(unsigned long*)karg;
	ubuffer[1] = (*(unsigned long*)(karg+1));
	ubuffer[2] = (*(unsigned long*)(karg+2));
	ubuffer[3] = (*(unsigned long*)(karg+3));


    if(cmd < DISP_CMD_FB_REQUEST)
    {
        if((ubuffer[0] != 0) && (ubuffer[0] != 1))
        {
            __wrn("para err in disp_ioctl, screen id = %d\n", (int)ubuffer[0]);
            return -1;
        }
    }
    if(suspend_status & 2)
    {
        __wrn("ioctl:%x fail when in suspend!\n", cmd);
        return -1;
    }

	//__inf("disp_ioctl,cmd:%x\n",cmd);
    printk(KERN_ERR "SEB_DUMP CMD=%X DATA= %X %X %X %X\n", cmd, ubuffer[0], ubuffer[1], ubuffer[2], ubuffer[3]);
    switch(cmd)
    {
    //----disp global----
    	case DISP_CMD_SET_BKCOLOR:
	    {
	        __disp_color_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_color_t)))
    		{
    			return  -EFAULT;
    		}
		    ret = BSP_disp_set_bk_color(ubuffer[0], &para);
		    break;
	    }

    	case DISP_CMD_SET_COLORKEY:
    	{
    	    __disp_colorkey_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_colorkey_t)))
    		{
    			return  -EFAULT;
    		}
    		ret = BSP_disp_set_color_key(ubuffer[0], &para);
         printk(KERN_ERR "SEB_DEBUG DISP_CMD_SET_COLORKEY DATA %X Type __disp_colorkey_t\n");
		    break;
		}

    	case DISP_CMD_SET_PALETTE_TBL:
    	    if((ubuffer[1] == 0) || ((int)ubuffer[3] <= 0))
    	    {
    	        __wrn("para invalid in disp ioctrl DISP_CMD_SET_PALETTE_TBL,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[3]);
    	        return -1;
    	    }
    		if(copy_from_user(gbuffer, (void __user *)ubuffer[1],ubuffer[3]))
    		{
    			return  -EFAULT;
    		}
    		ret = BSP_disp_set_palette_table(ubuffer[0], (__u32 *)gbuffer, ubuffer[2], ubuffer[3]);
    		break;

    	case DISP_CMD_GET_PALETTE_TBL:
    	    if((ubuffer[1] == 0) || ((int)ubuffer[3] <= 0))
    	    {
    	        __wrn("para invalid in disp ioctrl DISP_CMD_GET_PALETTE_TBL,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[3]);
    	        return -1;
    	    }
    		ret = BSP_disp_get_palette_table(ubuffer[0], (__u32 *)gbuffer, ubuffer[2], ubuffer[3]);
    		if(copy_to_user((void __user *)ubuffer[1], gbuffer,ubuffer[3]))
    		{
    			return  -EFAULT;
    		}
    		break;

    	case DISP_CMD_START_CMD_CACHE:
    		ret = BSP_disp_cmd_cache(ubuffer[0]);
    		g_disp_drv.b_cache[ubuffer[0]] = 1;
    		break;

    	case DISP_CMD_EXECUTE_CMD_AND_STOP_CACHE:
    	    g_disp_drv.b_cache[ubuffer[0]] = 0;
    		ret = BSP_disp_cmd_submit(ubuffer[0]);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;

    	case DISP_CMD_GET_OUTPUT_TYPE:
    		ret =  BSP_disp_get_output_type(ubuffer[0]);
    		break;

    	case DISP_CMD_SCN_GET_WIDTH:
    		ret = BSP_disp_get_screen_width(ubuffer[0]);
         printk("SEB_DEBUG DISP_CMD_SCN_GET_HEIGHT %x\n", ubuffer[0]);
    		break;

    	case DISP_CMD_SCN_GET_HEIGHT:
    		ret = BSP_disp_get_screen_height(ubuffer[0]);
         printk("SEB_DEBUG DISP_CMD_SCN_GET_HEIGHT %x\n", ubuffer[0]);
    		break;

    	case DISP_CMD_SET_GAMMA_TABLE:
    	    if((ubuffer[1] == 0) || ((int)ubuffer[2] <= 0))
    	    {
    	        __wrn("para invalid in disp ioctrl DISP_CMD_SET_GAMMA_TABLE,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[2]);
    	        return -1;
    	    }
    		if(copy_from_user(gbuffer, (void __user *)ubuffer[1],ubuffer[2]))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_set_gamma_table(ubuffer[0], (__u32 *)gbuffer, ubuffer[2]);
    		break;

    	case DISP_CMD_GAMMA_CORRECTION_ON:
    		ret = BSP_disp_gamma_correction_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_GAMMA_CORRECTION_OFF:
    		ret = BSP_disp_gamma_correction_disable(ubuffer[0]);
    		break;

        case DISP_CMD_SET_BRIGHT:
            ret = BSP_disp_set_bright(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_GET_BRIGHT:
            ret = BSP_disp_get_bright(ubuffer[0]);
    		break;

        case DISP_CMD_SET_CONTRAST:
            ret = BSP_disp_set_contrast(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_GET_CONTRAST:
            ret = BSP_disp_get_contrast(ubuffer[0]);
    		break;

        case DISP_CMD_SET_SATURATION:
            ret = BSP_disp_set_saturation(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_GET_SATURATION:
            ret = BSP_disp_get_saturation(ubuffer[0]);
    		break;

        case DISP_CMD_ENHANCE_ON:
            ret = BSP_disp_enhance_enable(ubuffer[0], 1);
    		break;

        case DISP_CMD_ENHANCE_OFF:
            ret = BSP_disp_enhance_enable(ubuffer[0], 0);
    		break;

        case DISP_CMD_GET_ENHANCE_EN:
            ret = BSP_disp_get_enhance_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_CAPTURE_SCREEN:
    	    ret = BSP_disp_capture_screen(ubuffer[0], (__disp_capture_screen_para_t *)ubuffer[1]);
    	    break;

        case DISP_CMD_SET_SCREEN_SIZE:
            ret = BSP_disp_set_screen_size(ubuffer[0], (__disp_rectsz_t*)ubuffer[1]);
            break;

        case DISP_CMD_DE_FLICKER_ON:
            ret = BSP_disp_de_flicker_enable(ubuffer[0], 1);
            break;

        case DISP_CMD_DE_FLICKER_OFF:
            ret = BSP_disp_de_flicker_enable(ubuffer[0], 0);
            break;

    //----layer----
    	case DISP_CMD_LAYER_REQUEST:
    		ret = BSP_disp_layer_request(ubuffer[0], (__disp_layer_work_mode_t)ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_RELEASE:
    		ret = BSP_disp_layer_release(ubuffer[0], ubuffer[1]);
    		DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;

    	case DISP_CMD_LAYER_OPEN:
    		ret = BSP_disp_layer_open(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_CLOSE:
    		ret = BSP_disp_layer_close(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_SET_FB:
    	{
    	    __disp_fb_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_layer_set_framebuffer(ubuffer[0], ubuffer[1], &para);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;
    	}

    	case DISP_CMD_LAYER_GET_FB:
    	{
    	    __disp_fb_t para;

    		ret = BSP_disp_layer_get_framebuffer(ubuffer[0], ubuffer[1], &para);
    		if(copy_to_user((void __user *)ubuffer[2], &para,sizeof(__disp_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_LAYER_SET_SRC_WINDOW:
    	{
    	    __disp_rect_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_layer_set_src_window(ubuffer[0],ubuffer[1], &para);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;
        }

    	case DISP_CMD_LAYER_GET_SRC_WINDOW:
    	{
    	    __disp_rect_t para;

    		ret = BSP_disp_layer_get_src_window(ubuffer[0],ubuffer[1], &para);
    		if(copy_to_user((void __user *)ubuffer[2], &para, sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_LAYER_SET_SCN_WINDOW:
    	{
    	    __disp_rect_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_layer_set_screen_window(ubuffer[0],ubuffer[1], &para);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
         printk(KERN_ERR "SEB_DEBUG DISP_CMD_LAYER_SET_SCN_WINDOW %X %X X=%d Y=%d WIDTH=%d HEIGHT=%d\n", ubuffer[0], ubuffer[1], para.x, para.y , para.width, para.height);
    		break;
        }

    	case DISP_CMD_LAYER_GET_SCN_WINDOW:
    	{
    	    __disp_rect_t para;

    		ret = BSP_disp_layer_get_screen_window(ubuffer[0],ubuffer[1], &para);
    		if(copy_to_user((void __user *)ubuffer[2], &para, sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_LAYER_SET_PARA:
    	{
    	    __disp_layer_info_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_layer_info_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_layer_set_para(ubuffer[0], ubuffer[1], &para);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;
        }

    	case DISP_CMD_LAYER_GET_PARA:
    	{
    	    __disp_layer_info_t para;

    		ret = BSP_disp_layer_get_para(ubuffer[0], ubuffer[1], &para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_layer_info_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_LAYER_TOP:
    		ret = BSP_disp_layer_set_top(ubuffer[0], ubuffer[1]);
         printk(KERN_ERR "SEB_DEBUG DISP_CMD_LAYER_TOP %X %X\n", ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_BOTTOM:
    		ret = BSP_disp_layer_set_bottom(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_ALPHA_ON:
    		ret = BSP_disp_layer_alpha_enable(ubuffer[0], ubuffer[1], 1);
         printk(KERN_ERR "SEB_DEBUG DISP_CMD_LAYER_ALPHA_ON %X %X\n",  ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_ALPHA_OFF:
    		ret = BSP_disp_layer_alpha_enable(ubuffer[0], ubuffer[1], 0);
    		break;

    	case DISP_CMD_LAYER_SET_ALPHA_VALUE:
    		ret = BSP_disp_layer_set_alpha_value(ubuffer[0], ubuffer[1], ubuffer[2]);
         printk(KERN_ERR "SEB_DEBUG DISP_CMD_LAYER_SET_ALPHA_VALUE %X %X %X\n",ubuffer[0], ubuffer[1], ubuffer[2]);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;

    	case DISP_CMD_LAYER_CK_ON:
    		ret = BSP_disp_layer_colorkey_enable(ubuffer[0], ubuffer[1], 1);
    		break;

    	case DISP_CMD_LAYER_CK_OFF:
    		ret = BSP_disp_layer_colorkey_enable(ubuffer[0], ubuffer[1], 0);
         printk(KERN_ERR "SEB_DEBUG DISP_CMD_LAYER_CK_OFF %X %X\n",  ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_SET_PIPE:
    		ret = BSP_disp_layer_set_pipe(ubuffer[0], ubuffer[1], ubuffer[2]);
         printk(KERN_ERR "SEB_DEBUG DISP_CMD_LAYER_SET_PIPE %X %X %X\n", ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

    	case DISP_CMD_LAYER_GET_ALPHA_VALUE:
    		ret = BSP_disp_layer_get_alpha_value(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_GET_ALPHA_EN:
    		ret = BSP_disp_layer_get_alpha_enable(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_GET_CK_EN:
    		ret = BSP_disp_layer_get_colorkey_enable(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_GET_PRIO:
    		ret = BSP_disp_layer_get_piro(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_GET_PIPE:
    		ret = BSP_disp_layer_get_pipe(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_SMOOTH:
            ret = BSP_disp_layer_set_smooth(ubuffer[0], ubuffer[1],(__disp_video_smooth_t) ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_SMOOTH:
            ret = BSP_disp_layer_get_smooth(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_BRIGHT:
            ret = BSP_disp_layer_set_bright(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_BRIGHT:
            ret = BSP_disp_layer_get_bright(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_CONTRAST:
            ret = BSP_disp_layer_set_contrast(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_CONTRAST:
            ret = BSP_disp_layer_get_contrast(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_SATURATION:
            ret = BSP_disp_layer_set_saturation(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_SATURATION:
            ret = BSP_disp_layer_get_saturation(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_HUE:
            ret = BSP_disp_layer_set_hue(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_HUE:
            ret = BSP_disp_layer_get_hue(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_ENHANCE_ON:
            ret = BSP_disp_layer_enhance_enable(ubuffer[0], ubuffer[1], 1);
    		break;

        case DISP_CMD_LAYER_ENHANCE_OFF:
            ret = BSP_disp_layer_enhance_enable(ubuffer[0], ubuffer[1], 0);
    		break;

        case DISP_CMD_LAYER_GET_ENHANCE_EN:
            ret = BSP_disp_layer_get_enhance_enable(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_VPP_ON:
            ret = BSP_disp_layer_vpp_enable(ubuffer[0], ubuffer[1], 1);
    		break;

        case DISP_CMD_LAYER_VPP_OFF:
            ret = BSP_disp_layer_vpp_enable(ubuffer[0], ubuffer[1], 0);
    		break;

        case DISP_CMD_LAYER_GET_VPP_EN:
            ret = BSP_disp_layer_get_vpp_enable(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_LUMA_SHARP_LEVEL:
            ret = BSP_disp_layer_set_luma_sharp_level(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_LUMA_SHARP_LEVEL:
            ret = BSP_disp_layer_get_luma_sharp_level(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_CHROMA_SHARP_LEVEL:
            ret = BSP_disp_layer_set_chroma_sharp_level(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_CHROMA_SHARP_LEVEL:
            ret = BSP_disp_layer_get_chroma_sharp_level(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_WHITE_EXTEN_LEVEL:
            ret = BSP_disp_layer_set_white_exten_level(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_WHITE_EXTEN_LEVEL:
            ret = BSP_disp_layer_get_white_exten_level(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_BLACK_EXTEN_LEVEL:
            ret = BSP_disp_layer_set_black_exten_level(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_BLACK_EXTEN_LEVEL:
            ret = BSP_disp_layer_get_black_exten_level(ubuffer[0], ubuffer[1]);
    		break;

    //----scaler----
    	case DISP_CMD_SCALER_REQUEST:
    		ret = BSP_disp_scaler_request();
    		break;

    	case DISP_CMD_SCALER_RELEASE:
    		ret = BSP_disp_scaler_release(ubuffer[1]);
    		break;

    	case DISP_CMD_SCALER_EXECUTE:
    	{
    	    __disp_scaler_para_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_scaler_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_scaler_start(ubuffer[1],&para);
    		break;
        }

    //----hwc----
    	case DISP_CMD_HWC_OPEN:
    		ret =  BSP_disp_hwc_enable(ubuffer[0], 1);
    		break;

    	case DISP_CMD_HWC_CLOSE:
    		ret =  BSP_disp_hwc_enable(ubuffer[0], 0);
    		break;

    	case DISP_CMD_HWC_SET_POS:
    	{
    	    __disp_pos_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_pos_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_hwc_set_pos(ubuffer[0], &para);
    		break;
        }

    	case DISP_CMD_HWC_GET_POS:
    	{
    	    __disp_pos_t para;

    		ret = BSP_disp_hwc_get_pos(ubuffer[0], &para);
    		if(copy_to_user((void __user *)ubuffer[1],&para, sizeof(__disp_pos_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_HWC_SET_FB:
    	{
    	    __disp_hwc_pattern_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_hwc_pattern_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_hwc_set_framebuffer(ubuffer[0], &para);
    		break;
        }

    	case DISP_CMD_HWC_SET_PALETTE_TABLE:
			if((ubuffer[1] == 0) || ((int)ubuffer[3] <= 0))
            {
                __wrn("para invalid in display ioctrl DISP_CMD_HWC_SET_PALETTE_TABLE,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[3]);
                return -1;
            }
    		if(copy_from_user(gbuffer, (void __user *)ubuffer[1],ubuffer[3]))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_hwc_set_palette(ubuffer[0], (void*)gbuffer, ubuffer[2], ubuffer[3]);
    		break;


    //----video----
    	case DISP_CMD_VIDEO_START:
    		ret = BSP_disp_video_start(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_VIDEO_STOP:
    		ret = BSP_disp_video_stop(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_VIDEO_SET_FB:
    	{
    	    __disp_video_fb_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_video_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_video_set_fb(ubuffer[0], ubuffer[1], &para);
    		break;
        }

        case DISP_CMD_VIDEO_GET_FRAME_ID:
            ret = BSP_disp_video_get_frame_id(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_VIDEO_GET_DIT_INFO:
        {
            __disp_dit_info_t para;

            ret = BSP_disp_video_get_dit_info(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_dit_info_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    //----lcd----
    	case DISP_CMD_LCD_ON:
    		ret = DRV_lcd_open(ubuffer[0]);
    		break;

    	case DISP_CMD_LCD_OFF:
    		ret = DRV_lcd_close(ubuffer[0]);
    		break;

    	case DISP_CMD_LCD_SET_BRIGHTNESS:
    		ret = BSP_disp_lcd_set_bright(ubuffer[0], (__disp_lcd_bright_t)ubuffer[1]);
    		break;

    	case DISP_CMD_LCD_GET_BRIGHTNESS:
    		ret = BSP_disp_lcd_get_bright(ubuffer[0]);
    		break;

    	case DISP_CMD_LCD_CPUIF_XY_SWITCH:
    		ret = BSP_disp_lcd_xy_switch(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LCD_SET_SRC:
    		ret = BSP_disp_lcd_set_src(ubuffer[0], (__disp_lcdc_src_t)ubuffer[1]);
    		break;

        case DISP_CMD_LCD_USER_DEFINED_FUNC:
            ret = BSP_disp_lcd_user_defined_func(ubuffer[0], ubuffer[1], ubuffer[2], ubuffer[3]);
            break;

	//----pwm----
        case DISP_CMD_PWM_SET_PARA:
            ret = pwm_set_para(ubuffer[0], (__pwm_info_t *)ubuffer[1]);
            break;

        case DISP_CMD_PWM_GET_PARA:
            ret = pwm_get_para(ubuffer[0], (__pwm_info_t *)ubuffer[1]);
            break;


    //----tv----
    	case DISP_CMD_TV_ON:
    		ret = BSP_disp_tv_open(ubuffer[0]);
    		break;

    	case DISP_CMD_TV_OFF:
    		ret = BSP_disp_tv_close(ubuffer[0]);
    		break;

    	case DISP_CMD_TV_SET_MODE:
    		ret = BSP_disp_tv_set_mode(ubuffer[0], (__disp_tv_mode_t)ubuffer[1]);
    		break;

    	case DISP_CMD_TV_GET_MODE:
    		ret = BSP_disp_tv_get_mode(ubuffer[0]);
    		break;

    	case DISP_CMD_TV_AUTOCHECK_ON:
    		ret = BSP_disp_tv_auto_check_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_TV_AUTOCHECK_OFF:
    		ret = BSP_disp_tv_auto_check_disable(ubuffer[0]);
    		break;

    	case DISP_CMD_TV_GET_INTERFACE:
    	    if(suspend_status != 0)
    	    {
    	        ret = DISP_TV_NONE;
    	    }
    	    else
    	    {
    		    ret = BSP_disp_tv_get_interface(ubuffer[0]);
            }
    		break;

    	case DISP_CMD_TV_SET_SRC:
    		ret = BSP_disp_tv_set_src(ubuffer[0], (__disp_lcdc_src_t)ubuffer[1]);
    		break;

        case DISP_CMD_TV_GET_DAC_STATUS:
            if(suspend_status != 0)
            {
                ret = 0;
            }
            else
            {
                ret =  BSP_disp_tv_get_dac_status(ubuffer[0], ubuffer[1]);
            }
            break;

        case DISP_CMD_TV_SET_DAC_SOURCE:
            ret =  BSP_disp_tv_set_dac_source(ubuffer[0], ubuffer[1], (__disp_tv_dac_source)ubuffer[2]);
            break;

        case DISP_CMD_TV_GET_DAC_SOURCE:
            ret =  BSP_disp_tv_get_dac_source(ubuffer[0], ubuffer[1]);
            break;

    //----hdmi----
    	case DISP_CMD_HDMI_ON:
    printk(KERN_ERR "SEB_DEBUG DISP_CMD_HDMI_ON DATA %lX \n", ubuffer[0]);
    		ret = BSP_disp_hdmi_open(ubuffer[0]);
    		break;

    	case DISP_CMD_HDMI_OFF:
    printk(KERN_ERR "SEB_DEBUG DISP_CMD_HDMI_OFF DATA %lX \n", ubuffer[0]);
    		ret = BSP_disp_hdmi_close(ubuffer[0]);
    		break;

    	case DISP_CMD_HDMI_SET_MODE:
    printk(KERN_ERR "SEB_DEBUG DISP_CMD_HDMI_SET_MODE DATA %lX %lX\n", ubuffer[0], ubuffer[1]);
    		ret = BSP_disp_hdmi_set_mode(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_HDMI_GET_MODE:
    printk(KERN_ERR "SEB_DEBUG DISP_CMD_HDMI_GET_MODE DATA %lX \n", ubuffer[0]);
    		ret = BSP_disp_hdmi_get_mode(ubuffer[0]);
    printk(KERN_ERR "SEB_DEBUG RET= %lX \n", ret);
    		break;

    	case DISP_CMD_HDMI_GET_HPD_STATUS:
    //printk(KERN_ERR "SEB_DEBUG DISP_CMD_HDMI_GET_HPD_STATUS DATA %lX \n", ubuffer[0]);
    	    if(suspend_status != 0)
    	    {
    	        ret = 0;
    	    }
    	    else
    	    {
    	        ret = BSP_disp_hdmi_get_hpd_status(ubuffer[0]);
    	    }
    //printk(KERN_ERR "SEB_DEBUG RET= %lX \n", ret);
    		break;

    	case DISP_CMD_HDMI_SUPPORT_MODE:
    printk(KERN_ERR "SEB_DEBUG DISP_CMD_HDMI_SUPPORT_MODE DATA %lX %lX\n", ubuffer[0], ubuffer[1]);
    		ret = BSP_disp_hdmi_check_support_mode(ubuffer[0], ubuffer[1]);
    printk(KERN_ERR "SEB_DEBUG RET= %lX \n", ret);
    		break;

    	case DISP_CMD_HDMI_SET_SRC:
    printk(KERN_ERR "SEB_DEBUG DISP_CMD_HDMI_SET_SRC DATA %lX %lX\n", ubuffer[0], ubuffer[1]);
    		ret = BSP_disp_hdmi_set_src(ubuffer[0], (__disp_lcdc_src_t)ubuffer[1]);
    		break;

    //----vga----
    	case DISP_CMD_VGA_ON:
    		ret = BSP_disp_vga_open(ubuffer[0]);
    		break;

    	case DISP_CMD_VGA_OFF:
    		ret = BSP_disp_vga_close(ubuffer[0]);
    		break;

    	case DISP_CMD_VGA_SET_MODE:
    		ret = BSP_disp_vga_set_mode(ubuffer[0], (__disp_vga_mode_t)ubuffer[1]);
    		break;

    	case DISP_CMD_VGA_GET_MODE:
    		ret = BSP_disp_vga_get_mode(ubuffer[0]);
    		break;

    	case DISP_CMD_VGA_SET_SRC:
    		ret = BSP_disp_vga_set_src(ubuffer[0], (__disp_lcdc_src_t)ubuffer[1]);
    		break;

    //----sprite----
    	case DISP_CMD_SPRITE_OPEN:
    		ret = BSP_disp_sprite_open(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_CLOSE:
    		ret = BSP_disp_sprite_close(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_SET_FORMAT:
    		ret = BSP_disp_sprite_set_format(ubuffer[0], (__disp_pixel_fmt_t)ubuffer[1], (__disp_pixel_seq_t)ubuffer[2]);
    		break;

    	case DISP_CMD_SPRITE_GLOBAL_ALPHA_ENABLE:
    		ret = BSP_disp_sprite_alpha_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_GLOBAL_ALPHA_DISABLE:
    		ret = BSP_disp_sprite_alpha_disable(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_ENABLE:
    		ret = BSP_disp_sprite_get_alpha_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_SET_GLOBAL_ALPHA_VALUE:
    		ret = BSP_disp_sprite_set_alpha_vale(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_VALUE:
    		ret = BSP_disp_sprite_get_alpha_value(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_SET_ORDER:
    		ret = BSP_disp_sprite_set_order(ubuffer[0], ubuffer[1],ubuffer[2]);
    		break;

    	case DISP_CMD_SPRITE_GET_TOP_BLOCK:
    		ret = BSP_disp_sprite_get_top_block(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_GET_BOTTOM_BLOCK:
    		ret = BSP_disp_sprite_get_bottom_block(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_SET_PALETTE_TBL:
            if((ubuffer[1] == 0) || ((int)ubuffer[3] <= 0))
            {
                __wrn("para invalid in display ioctrl DISP_CMD_SPRITE_SET_PALETTE_TBL,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[3]);
                return -1;
            }
    		if(copy_from_user(gbuffer, (void __user *)ubuffer[1],ubuffer[3]))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret =  BSP_disp_sprite_set_palette_table(ubuffer[0], (__u32 * )gbuffer,ubuffer[2],ubuffer[3]);
    		break;

    	case DISP_CMD_SPRITE_GET_BLOCK_NUM:
    		ret = BSP_disp_sprite_get_block_number(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_REQUEST:
    	{
    	    __disp_sprite_block_para_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_sprite_block_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_request(ubuffer[0], &para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_RELEASE:
    		ret = BSP_disp_sprite_block_release(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_SET_SCREEN_WINDOW:
    	{
    	    __disp_rect_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_set_screen_win(ubuffer[0], ubuffer[1],&para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_GET_SCREEN_WINDOW:
    	{
    	    __disp_rect_t para;

    		ret = BSP_disp_sprite_block_get_srceen_win(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_SET_SOURCE_WINDOW:
    	{
    	    __disp_rect_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_set_src_win(ubuffer[0], ubuffer[1],&para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_GET_SOURCE_WINDOW:
    	{
    	    __disp_rect_t para;

    		ret = BSP_disp_sprite_block_get_src_win(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_SET_FB:
    	{
    	    __disp_fb_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_set_framebuffer(ubuffer[0], ubuffer[1],&para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_GET_FB:
    	{
    	    __disp_fb_t para;

    		ret = BSP_disp_sprite_block_get_framebufer(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_SET_TOP:
    		ret = BSP_disp_sprite_block_set_top(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_SET_BOTTOM:
    		ret = BSP_disp_sprite_block_set_bottom(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_GET_PREV_BLOCK:
    		ret = BSP_disp_sprite_block_get_pre_block(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_GET_NEXT_BLOCK:
    		ret = BSP_disp_sprite_block_get_next_block(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_GET_PRIO:
    		ret = BSP_disp_sprite_block_get_prio(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_OPEN:
    		ret = BSP_disp_sprite_block_open(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_CLOSE:
    		ret = BSP_disp_sprite_block_close(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_SET_PARA:
    	{
    	    __disp_sprite_block_para_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_sprite_block_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_set_para(ubuffer[0], ubuffer[1],&para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_GET_PARA:
    	{
    	    __disp_sprite_block_para_t para;

    		ret = BSP_disp_sprite_block_get_para(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_sprite_block_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

	//----framebuffer----
    	case DISP_CMD_FB_REQUEST:
    	{
    	    __disp_fb_create_para_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_fb_create_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
			ret = Display_Fb_Request(ubuffer[0], &para);
         printk(KERN_ERR "SEB_DEBUG DISP_CMD_FB_REQUEST DATA %X Type __disp_fb_create_para_t\n", ubuffer[0]);
         printk(KERN_ERR "SEB_DEBUG fb_mode %x\n", para.fb_mode);
         printk(KERN_ERR "SEB_DEBUG mode %x\n", para.mode);
         printk(KERN_ERR "SEB_DEBUG buffer_num %lx\n", para.buffer_num);
         printk(KERN_ERR "SEB_DEBUG width %lx\n", para.width);
         printk(KERN_ERR "SEB_DEBUG height %lx\n", para.height);
         printk(KERN_ERR "SEB_DEBUG output_width %lx\n", para.output_width);
         printk(KERN_ERR "SEB_DEBUG output_height %lx\n", para.output_height);
         printk(KERN_ERR "SEB_DEBUG primary_screen_id %lx\n", para.primary_screen_id);
         printk(KERN_ERR "SEB_DEBUG aux_output_width %lx\n", para.aux_output_width);
         printk(KERN_ERR "SEB_DEBUG aux_output_height %lx\n", para.aux_output_height);
         printk(KERN_ERR "SEB_DEBUG line_length %lx\n", para.line_length);
         printk(KERN_ERR "SEB_DEBUG smem_len %lx\n", para.smem_len);
         printk(KERN_ERR "SEB_DEBUG ch1_offset %lx\n", para.ch1_offset);
         printk(KERN_ERR "SEB_DEBUG ch2_offset %lx\n", para.ch2_offset);

			break;
        }

		case DISP_CMD_FB_RELEASE:
			ret = Display_Fb_Release(ubuffer[0]);
			break;

	    case DISP_CMD_FB_GET_PARA:
	    {
    	    __disp_fb_create_para_t para;

			ret = Display_Fb_get_para(ubuffer[0], &para);
    		if(copy_to_user((void __user *)ubuffer[1],&para, sizeof(__disp_fb_create_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
			break;
        }

	    case DISP_CMD_GET_DISP_INIT_PARA:
	    {
    	    __disp_init_t para;

			ret = Display_get_disp_init_para(&para);
    		if(copy_to_user((void __user *)ubuffer[0],&para, sizeof(__disp_init_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
			break;
        }

		case DISP_CMD_MEM_REQUEST:
			ret =  disp_mem_request(ubuffer[0],ubuffer[1]);
			break;

	//----for test----
		case DISP_CMD_MEM_RELASE:
			ret =  disp_mem_release(ubuffer[0]);
			break;

		case DISP_CMD_MEM_SELIDX:
			g_disp_mm_sel = ubuffer[0];
			break;

		case DISP_CMD_MEM_GETADR:
			ret = g_disp_mm[ubuffer[0]].mem_start;
			break;

		case DISP_CMD_SUSPEND:
		{
		    pm_message_t state;

			ret = disp_suspend(0, state);
			break;
        }

		case DISP_CMD_RESUME:
			ret = disp_resume(0);
			break;

        case DISP_CMD_PRINT_REG:
            ret = BSP_disp_print_reg(1, ubuffer[0]);
            break;

		default:
		    break;
    }

	return ret;
}

static const struct file_operations disp_fops =
{
	.owner		= THIS_MODULE,
	.open		= disp_open,
	.release    = disp_release,
	.write      = disp_write,
	.read		= disp_read,
	.unlocked_ioctl	= disp_ioctl,
	.mmap       = disp_mmap,
};

static struct platform_driver disp_driver =
{
	.probe		= disp_probe,
	.remove		= disp_remove,
	.suspend    = disp_suspend,
	.resume    = disp_resume,
	.shutdown   = disp_shutdown,
	.driver		=
	{
		.name	= "disp",
		.owner	= THIS_MODULE,
	},
};


struct platform_device disp_device =
{
	.name           = "disp",
	.id		        = -1,
	.num_resources  = ARRAY_SIZE(disp_resource),
	.resource	    = disp_resource,
	.dev            = {}
};

int __init disp_module_init(void)
{
    int ret, err;

    __inf("disp_module_init\n");

    atomic_set(&cmd_index, 0);
    alloc_chrdev_region(&devid, 0, 1, "disp");
    my_cdev = cdev_alloc();
    cdev_init(my_cdev, &disp_fops);
    my_cdev->owner = THIS_MODULE;
    err = cdev_add(my_cdev, devid, 1);
    if (err)
    {
        __wrn("cdev_add fail\n");
        return -1;
    }

    disp_class = class_create(THIS_MODULE, "disp");
    if (IS_ERR(disp_class))
    {
        __wrn("class_create fail\n");
        return -1;
    }

    device_create(disp_class, NULL, devid, NULL, "disp");

	ret = platform_device_register(&disp_device);

	if (ret == 0)
	{
		ret = platform_driver_register(&disp_driver);
	}
#ifdef CONFIG_HAS_EARLYSUSPEND
    register_early_suspend(&backlight_early_suspend_handler);
#endif

	return ret;
}

static void __exit disp_module_exit(void)
{
	__inf("disp_module_exit\n");

#ifdef CONFIG_HAS_EARLYSUSPEND
    unregister_early_suspend(&backlight_early_suspend_handler);
#endif
    DRV_DISP_Exit();

	platform_driver_unregister(&disp_driver);
	platform_device_unregister(&disp_device);

    device_destroy(disp_class,  devid);
    class_destroy(disp_class);

    cdev_del(my_cdev);
}

EXPORT_SYMBOL(disp_set_hdmi_func);
EXPORT_SYMBOL(DRV_DISP_Init);


late_initcall(disp_module_init);
//module_init(disp_module_init);
module_exit(disp_module_exit);

MODULE_AUTHOR("danling_xiao");
MODULE_DESCRIPTION("display driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:disp");

