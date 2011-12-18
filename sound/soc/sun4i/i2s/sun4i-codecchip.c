/*
********************************************************************************************************
*                          SUN4I----HDMI AUDIO
*                   (c) Copyright 2002-2004, All winners Co,Ld.
*                          All Right Reserved
*
* FileName: sun4i-codecchip.c   author:chenpailin  date:2011-07-19 
* Description: 
* Others: 
* History:
*   <author>      <time>      <version>   <desc> 
*   chenpailin   2011-07-19     1.0      modify this module 
********************************************************************************************************
*/

#include <linux/module.h>
#include <linux/clk.h>
#include <linux/mutex.h>
//#include <linux/gpio.h>

#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>
#include <sound/soc-dapm.h>
#include <mach/gpio_v2.h>
#include <mach/script_v2.h>
#include <linux/io.h>

#include "sun4i-i2s.h"
#include "sun4i-pcm.h"

#include "codecchip.h"


static struct clk *xtal;
static int i2s_used = 0;
static int clk_users;
static DEFINE_MUTEX(clk_lock);

#ifdef ENFORCE_RATES
static struct snd_pcm_hw_constraint_list hw_constraints_rates = {
	.count	= ARRAY_SIZE(rates),
	.list	= rates,
	.mask	= 0,
};
#endif

static struct platform_device *sun4i_codecchip_snd_device;

static int sun4i_codecchip_startup(struct snd_pcm_substream *substream)
{
	int ret = 0;
	#ifdef ENFORCE_RATES
		struct snd_pcm_runtime *runtime = substream->runtime;;
	#endif
	mutex_lock(&clk_lock);
	mutex_unlock(&clk_lock);
	if (!ret) {
	#ifdef ENFORCE_RATES
		ret = snd_pcm_hw_constraint_list(runtime, 0,
						 SNDRV_PCM_HW_PARAM_RATE,
						 &hw_constraints_rates);
		if (ret < 0)

	#endif
	}
	return ret;
}

static void sun4i_codecchip_shutdown(struct snd_pcm_substream *substream)
{
	mutex_lock(&clk_lock);
	clk_users -= 1;
	if (clk_users == 0) {
		clk_put(xtal);
		xtal = NULL;

	}
	mutex_unlock(&clk_lock);
}

typedef struct __MCLK_SET_INF
{
    __u32       samp_rate;      // sample rate
    __u16       mult_fs;        // multiply of smaple rate

    __u8        clk_div;        // mpll division
    __u8        mpll;           // select mpll, 0 - 24.576 Mhz, 1 - 22.5792 Mhz

} __mclk_set_inf;


typedef struct __BCLK_SET_INF
{
    __u8        bitpersamp;     // bits per sample
    __u8        clk_div;        // clock division
    __u16       mult_fs;        // multiplay of sample rate

} __bclk_set_inf;


static __bclk_set_inf BCLK_INF[] =
{
    // 16bits per sample
    {16,  4, 128}, {16,  6, 192}, {16,  8, 256},
    {16, 12, 384}, {16, 16, 512},

    //24 bits per sample
    {24,  4, 192}, {24,  8, 384}, {24, 16, 768},

    //32 bits per sample
    {32,  2, 128}, {32,  4, 256}, {32,  6, 384},
    {32,  8, 512}, {32, 12, 768},

    //end flag
    {0xff, 0, 0},
};

static __mclk_set_inf  MCLK_INF[] =
{
    // 8k bitrate
    {  8000, 128, 24, 0}, {  8000, 192, 16, 0}, {  8000, 256, 12, 0},
    {  8000, 384,  8, 0}, {  8000, 512,  6, 0}, {  8000, 768,  4, 0},

    // 16k bitrate
    { 16000, 128, 12, 0}, { 16000, 192,  8, 0}, { 16000, 256,  6, 0},
    { 16000, 384,  4, 0}, { 16000, 768,  2, 0},

    // 32k bitrate
    { 32000, 128,  6, 0}, { 32000, 192,  4, 0}, { 32000, 384,  2, 0},
    { 32000, 768,  1, 0},

    // 64k bitrate
    { 64000, 192,  2, 0}, { 64000, 384,  1, 0},

    //128k bitrate
    {128000, 192,  1, 0},

    // 12k bitrate
    { 12000, 128, 16, 0}, { 12000, 256, 8, 0}, { 12000, 512, 4, 0},

    // 24k bitrate
    { 24000, 128,  8, 0}, { 24000, 256, 4, 0}, { 24000, 512, 2, 0},

    // 48K bitrate
    { 48000, 128,  4, 0}, { 48000, 256,  2, 0}, { 48000, 512, 1, 0},

    // 96k bitrate
    { 96000, 128 , 2, 0}, { 96000, 256,  1, 0},

    //192k bitrate
    {192000, 128,  1, 0},

    //11.025k bitrate
    { 11025, 128, 16, 1}, { 11205, 256,  8, 1}, { 11205, 512,  4, 1},

    //22.05k bitrate
    { 22050, 128,  8, 1}, { 22050, 256,  4, 1},
    { 22050, 512,  2, 1}, 

    //44.1k bitrate
    { 44100, 128,  4, 1}, { 44100, 256,  2, 1}, { 44100, 512,  1, 1},

    //88.2k bitrate
    { 88200, 128,  2, 1}, { 88200, 256,  1, 1},

    //176.4k bitrate
    {176400, 128, 1, 1},

    //end flag 0xffffffff
    {0xffffffff, 0, 0, 0},
};

static s32 get_clock_divder(u32 sample_rate, u32 sample_width, u32 * mclk_div, u32* mpll, u32* bclk_div, u32* mult_fs)
{
	u32 i, j, ret = -EINVAL;

	for(i=0; i< 100; i++)
	{
		 if((MCLK_INF[i].samp_rate == sample_rate) && ((MCLK_INF[i].mult_fs == 256) || (MCLK_INF[i].mult_fs == 128)))
		 {
			  for(j=0; j<ARRAY_SIZE(BCLK_INF); j++)
			  {
					if((BCLK_INF[j].bitpersamp == sample_width) && (BCLK_INF[j].mult_fs == MCLK_INF[i].mult_fs))
					{
						 //set mclk and bclk division
						 *mclk_div = MCLK_INF[i].clk_div;
						 *mpll = MCLK_INF[i].mpll;
						 *bclk_div = BCLK_INF[j].clk_div;
						 *mult_fs = MCLK_INF[i].mult_fs;
						 ret = 0;
						 break;
					}
			  }
		 }
		 else if(MCLK_INF[i].samp_rate == 0xffffffff)
		 	break;
	}

	return ret;
}

static int sun4i_codecchip_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->dai->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	int ret = 0;
	unsigned long rate = params_rate(params);
	u32 mclk_div=0, mpll=0, bclk_div=0, mult_fs=0;

	get_clock_divder(rate, 32, &mclk_div, &mpll, &bclk_div, &mult_fs);
	
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0)
		return ret;

	ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0)
		return ret;

	ret = snd_soc_dai_set_sysclk(cpu_dai, 0 , mpll, 0);
	if (ret < 0)
		return ret;
		
	ret = snd_soc_dai_set_sysclk(codec_dai, 0 , mpll, 0);
	if (ret < 0)
		return ret;
		
	ret = snd_soc_dai_set_clkdiv(cpu_dai, SUN4I_DIV_MCLK, mclk_div);
	if (ret < 0)
		return ret;
		
	ret = snd_soc_dai_set_clkdiv(cpu_dai, SUN4I_DIV_BCLK, bclk_div);
	if (ret < 0)
		return ret;
		
	ret = snd_soc_dai_set_clkdiv(codec_dai, 0, mult_fs);
	if (ret < 0)
		return ret;

	return 0;
}

static struct snd_soc_ops sun4i_codecchip_ops = {
	.startup = sun4i_codecchip_startup,
	.shutdown = sun4i_codecchip_shutdown,
	.hw_params = sun4i_codecchip_hw_params,
};

static struct snd_soc_dai_link sun4i_codecchip_dai_link = {
	.name = "codecchip",
	.stream_name = "codecchip",
	.codec_dai = &codecchip_dai,
	.cpu_dai = &sun4i_iis_dai,
	.ops = &sun4i_codecchip_ops,
};

static struct snd_soc_card snd_soc_sun4i_codecchip = {
	.name = "SUN4I_codecchip",
	.platform = &sun4i_soc_platform_i2s,
	.dai_link = &sun4i_codecchip_dai_link,
	.num_links = 1,
};

static struct snd_soc_device sun4i_codecchip_snd_devdata = {
	.card = &snd_soc_sun4i_codecchip,
	.codec_dev = &soc_codec_dev_codecchip,
};

static int sun4i_codecchip_probe(struct platform_device *pdev)
{
	int ret;
	
	sun4i_codecchip_snd_device = platform_device_alloc("soc-audio", 2);
	if (!sun4i_codecchip_snd_device) {
		return -ENOMEM;
	}

	platform_set_drvdata(sun4i_codecchip_snd_device,
			     &sun4i_codecchip_snd_devdata);
	sun4i_codecchip_snd_devdata.dev = &sun4i_codecchip_snd_device->dev;

	ret = platform_device_add(sun4i_codecchip_snd_device);

	if (ret) {
		platform_device_put(sun4i_codecchip_snd_device);
	}

	return ret;
}

static int sun4i_codecchip_remove(struct platform_device *pdev)
{
	platform_device_unregister(sun4i_codecchip_snd_device);
	return 0;
}

static struct platform_driver sun4i_codecchip_driver = {
	.probe  = sun4i_codecchip_probe,
	.remove = sun4i_codecchip_remove,
	.driver = {
		.name = "sun4i_codecchip",
		.owner = THIS_MODULE,
	},
};

struct platform_device sun4i_codecchip_device = {
	.name		= "sun4i_codecchip",
};

static int __init sun4i_codecchip_init(void)
{
	int ret;
	int ret2;
	ret2 = script_parser_fetch("i2s_para","i2s_used", &i2s_used, sizeof(int));
	
	if (ret2)
    {
        printk("[I2S]sun4i_codecchip_init fetch i2s using configuration failed\n");
    } 
    
    if(i2s_used)
    {
		ret = platform_driver_register(&sun4i_codecchip_driver);
		if (ret != 0){
			goto err;
		}
	
		ret = platform_device_register(&sun4i_codecchip_device);
		if (ret != 0){
			platform_driver_unregister(&sun4i_codecchip_driver);
		}
	
		err:
			return ret;
	}
	else
	{
		printk("[I2S]sun4i_codecchip cannot find any using configuration for controllers, return directly!\n");
        return 0;
	}
}

static void __exit sun4i_codecchip_exit(void)
{
	if(i2s_used)
	{
		i2s_used = 0;
		platform_driver_unregister(&sun4i_codecchip_driver);
	}
}


module_init(sun4i_codecchip_init);
module_exit(sun4i_codecchip_exit);

MODULE_AUTHOR("ALL WINNER");
MODULE_DESCRIPTION("SUN4I_codecchip ALSA SoC audio driver");
MODULE_LICENSE("GPL");
