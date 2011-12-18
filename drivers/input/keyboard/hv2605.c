/*
 * drivers/input/keyboard/hv2605_keypad.c
 *
 * HV2605 KEYPAD driver.
 *
 * Copyright (c) 2010  Focal tech Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 *	note: only support mulititouch	Wenfs 2010-10-01
 */

#include <linux/i2c.h>
#include <linux/input.h>
#include "hv2605.h"
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach/script_v2.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
    #include <linux/pm.h>
    #include <linux/earlysuspend.h>
#endif
//#define KEY_DEBUG
//#define PRINT_STATUS_INFO
#define  PRINT_SUSPEND_INFO

static struct i2c_client *this_client;
static struct hv_keypad_data *hv_keypad;


struct key_event {
	int	key_val;
	int key_last;
	int	key_status;
};

struct hv_keypad_data {
	struct input_dev     *input_dev;
	struct key_event     event;
 	struct delayed_work  work;
  	struct workqueue_struct *queue;
};

#ifdef PRINT_STATUS_INFO
#define print_status_info(fmt, args...)   \
        do{                              \
                printk(fmt, ##args);     \
        }while(0)
#else
#define print_status_info(fmt, args...)   //
#endif

static struct i2c_msg tx_msg[] = {{0},};
static 	struct i2c_msg rx_msgs[] = {{0},{0},};

#ifdef CONFIG_HAS_EARLYSUSPEND
struct hv2605_keyboard_data {
    struct early_suspend early_suspend;
};
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
static struct hv2605_keyboard_data *keyboard_data;
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
static void hv2605_keyboard_suspend(struct early_suspend *h)
{
    #ifdef PRINT_SUSPEND_INFO
        printk("enter earlysuspend: hv2605_keyboard_suspend. \n");
    #endif
    cancel_delayed_work_sync(&hv_keypad->work);
    return ;
}
static void hv2605_keyboard_resume(struct early_suspend *h)
{
    #ifdef PRINT_SUSPEND_INFO
        printk("enter laterresume: hv2605_keyboard_resume. \n");
    #endif
    queue_delayed_work(hv_keypad->queue, &hv_keypad->work, DELAY_PERIOD);
    return ;
}
#else
#endif

static int hv_i2c_rxdata(char *rxdata, int length)
{
	int ret;

    rx_msgs[0].addr	= this_client->addr;
	rx_msgs[0].flags	= I2C_M_RD;
	rx_msgs[0].len	= 1;
	rx_msgs[0].buf	= rxdata;

    //msleep(1);
	ret = i2c_transfer(this_client->adapter, rx_msgs, 1);
	//printk("msg i2c read: 0x%x\n", this_client->addr);
	//printk("HV IIC read data\n");
	if (ret < 0){
		//pr_info("msg %s i2c read error: 0x%x\n", __func__, this_client->addr);
	}

	return ret;
}

static int hv_i2c_txdata(char *txdata, int length)
{
	int ret;
    tx_msg[0].addr   = this_client->addr;
    tx_msg[0].flags  = 0;
    tx_msg[0].len    = length;
    tx_msg[0].buf    = txdata;

   	//msleep(1);
	ret = i2c_transfer(this_client->adapter, tx_msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}

static int hv_init(void)
{
    u8 buf[12] = {0x88,0x05,0x40,0x40,0x40,0x40,0x40,0xf8,0x00,0x05,0x00,0x42};
    //u8 buf[12] =   {0x88,0x05,0x90,0x90,0x90,0x90,0x90,0xf8,0x00,0x40,0x00,0x0d};
    //u8 buf[12] = {0x88,0x05,0x40,0x40,0x40,0x40,0xff,0xf8,0x00,0x00,0x00,0xfc};
    int ret = -1;
    struct hv_keypad_data *data = i2c_get_clientdata(this_client);
    struct key_event *event = &data->event;
    event->key_status = 2;
    event->key_last = 0;
    ret = hv_i2c_txdata(buf, 12);
    if (ret > 1) {
        printk("write reg failed! %#x ret: %d", buf[0], ret);
        return -1;
    }

    return 0;
}

static void hv_keypad_release(void)
{
	struct hv_keypad_data *data = i2c_get_clientdata(this_client);
	struct key_event *event = &data->event;
	input_report_key(data->input_dev,event->key_val, 0);
	#ifdef KEY_DEBUG
		printk("======Release the key %d=====\n",event->key_val);
	#endif
	input_sync(data->input_dev);
	event->key_status = 2;
	event->key_last = 0;
}


static int hv_read_data(void)
{
	struct hv_keypad_data *data = i2c_get_clientdata(this_client);
	struct key_event *event = &data->event;
	u8 buf[2] = {0};
	int ret = -1;
	//memset(event, 0, sizeof(struct key_event));
	ret = hv_i2c_rxdata(buf, 1);
    if(ret==1){
    	if((0xff != buf[0])&& (0 != buf[0])){
            event->key_val    = buf[0];
    	}else if(0xff == buf[0]){
            //event->key_val = event->key_last;
            event->key_val    = buf[0];
            //printk("receive 0xff. \n");
    	}
    	print_status_info("157 event->key_status = %d\n",event->key_status);

    }else{
    	if(event->key_status ==1){
            event->key_status = 0;
    	}
        print_status_info("163 event->key_status = %d\n",event->key_status);

    }

	return ret;
}

static void hv_report_value(void)
{
	struct hv_keypad_data *data = i2c_get_clientdata(this_client);
	struct key_event *event = &data->event;

	input_report_key(data->input_dev, event->key_val, 1);
	#ifdef KEY_DEBUG
		printk("Press the key %d\n",event->key_val);
	#endif
	input_sync(data->input_dev);
	event->key_status = 1;
}

static void hv_read_loop(struct work_struct *work)
{
	int ret = -1;
	struct hv_keypad_data *data = i2c_get_clientdata(this_client);
	struct key_event *event = &data->event;
	//printk("==========Begin Read Data============\n");
    ret = hv_read_data();
    if(ret==1)
    {
        	switch(event->key_val)
        	{
        		case 1:
        		case 2:
        		case 3:
        		case 4:
        		case 5:
        		    if(event->key_last != event->key_val){
                        event->key_last = event->key_val;
                        hv_report_value();
                    }
        		    break;

        		case 0xff:
            	    if(event->key_status == 1){
                        event->key_val = event->key_last;
                        hv_keypad_release();
            	    }
            		break;

        		default :
        		    //hv_keypad_release();
        		    break;

        	}

    	    print_status_info("225 event->key_status = %d\n",event->key_status);
    }else if(event->key_status ==0){
            hv_keypad_release();
    		print_status_info("233 event->key_status = %d\n",event->key_status);
    }

	queue_delayed_work(hv_keypad->queue, &hv_keypad->work, DELAY_PERIOD);

}

static int hv_keypad_probe(struct i2c_client *client, const struct i2c_device_id *id)
{

	struct input_dev *input_dev;
	int err = 0;
	int i;

	printk("======================================hv_keypad_probe Begin=============================================\n");
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	hv_keypad = kzalloc(sizeof(*hv_keypad), GFP_KERNEL);
	if (!hv_keypad)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}

	this_client = client;
	i2c_set_clientdata(client, hv_keypad);


	INIT_DELAYED_WORK(&hv_keypad->work, hv_read_loop);
	hv_keypad->queue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!hv_keypad->queue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}

	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}

	hv_keypad->input_dev = input_dev;

	input_dev->name = HV_NAME;
	input_dev->phys = "sun4ikbd/inputx";
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0100;

	input_dev->evbit[0] = BIT_MASK(EV_KEY);

	for (i = 1; i < 6; i++)
		set_bit(i, input_dev->keybit);

	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
		"hv_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}

    hv_init();

	queue_delayed_work(hv_keypad->queue, &hv_keypad->work, DELAY_PERIOD);
#ifdef CONFIG_HAS_EARLYSUSPEND
    printk("==register_early_suspend =\n");
    keyboard_data = kzalloc(sizeof(*keyboard_data), GFP_KERNEL);
	if (keyboard_data == NULL) {
		err = -ENOMEM;
		goto err_alloc_data_failed;
	}
    keyboard_data->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
    keyboard_data->early_suspend.suspend = hv2605_keyboard_suspend;
    keyboard_data->early_suspend.resume	= hv2605_keyboard_resume;
    register_early_suspend(&keyboard_data->early_suspend);
#endif
	printk("==probe ======over =\n");

    return 0;

exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
exit_create_singlethread:
	printk("==singlethread error =\n");
	i2c_set_clientdata(client, NULL);
	kfree(hv_keypad);
exit_alloc_data_failed:
exit_check_functionality_failed:
#ifdef CONFIG_HAS_EARLYSUSPEND
 err_alloc_data_failed:
#endif
	return err;
}

static int __devexit hv_keypad_remove(struct i2c_client *client)
{

	struct hv_keypad_data *hv_keypadc = i2c_get_clientdata(client);
	input_unregister_device(hv_keypadc->input_dev);
	kfree(hv_keypad);
	printk("==hv_keypad_remove=\n");
	//cancel_work_sync(&zt_ts->work);
	//destroy_workqueue(zt_ts->queue);
	cancel_delayed_work_sync(&hv_keypad->work);
	destroy_workqueue(hv_keypad->queue);
	i2c_set_clientdata(client, NULL);
	return 0;
}

static const struct i2c_device_id hv_keypad_id[] = {
	{ HV_NAME, 0 },{ }
};
MODULE_DEVICE_TABLE(i2c, hv_keypad_id);

static struct i2c_driver hv_keypad_driver = {
	.probe		= hv_keypad_probe,
	.remove		= __devexit_p(hv_keypad_remove),
	.id_table	= hv_keypad_id,
	.driver	= {
		.name	=    HV_NAME,
		.owner	=    THIS_MODULE,
	},
};

static int __init hv_keypad_init(void)
{
    int ret = -1;
    int hv_keypad_used = -1;

	printk("========HV Inital ===================\n");

	if(SCRIPT_PARSER_OK != script_parser_fetch("tkey_para", "tkey_used", &hv_keypad_used, 1)){
        pr_err("hv_keypad: script_parser_fetch err. \n");
        goto script_parser_fetch_err;
	}
	if(1 != hv_keypad_used){
        pr_err("hv_keypad: hv_keypad_unused. \n");
        return 0;
	}
	ret = i2c_add_driver(&hv_keypad_driver);
script_parser_fetch_err:
    return ret;

}

static void __exit hv_keypad_exit(void)
{
	i2c_del_driver(&hv_keypad_driver);
}
module_init(hv_keypad_init);
module_exit(hv_keypad_exit);

MODULE_AUTHOR("<zhengdixu@allwinnertech.com>");
MODULE_DESCRIPTION("hv keypad driver");
MODULE_LICENSE("GPL");

