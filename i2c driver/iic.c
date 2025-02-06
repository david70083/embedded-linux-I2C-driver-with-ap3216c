#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "linux/ioctl.h"
#include "linux/of_irq.h"
#include <linux/input.h>
#include <linux/i2c.h>

#define char_name "iic_dev"

#define AP3216C_SYSTEMCONG	0x00	
#define AP3216C_INTSTATUS	0X01	
#define AP3216C_INTCLEAR	0X02	
#define AP3216C_IRDATALOW	0x0A	
#define AP3216C_IRDATAHIGH	0x0B	
#define AP3216C_ALSDATALOW	0x0C	
#define AP3216C_ALSDATAHIGH	0X0D	
#define AP3216C_PSDATALOW	0X0E	
#define AP3216C_PSDATAHIGH	0X0F	

struct newleddev
{
    struct cdev cdev;
    struct class *class;
    struct device *device;
    dev_t devid;
    int major;
    int minor;
    void *private_data;
};

struct newleddev leddev;

static s32 ap3216c_write_regs(struct newleddev *dev, u8 reg, u8 *buf, u8 len)
{
	u8 b[256];
	struct i2c_msg msg;
	struct i2c_client *client = (struct i2c_client *)dev->private_data;
	//// transfer struct 
    msg.addr = client->addr;	// i2c addr		 
	msg.flags = 0;				// write: 0   read: I2C_M_RD
	msg.buf = &reg;			    // data
	msg.len = 1;	            // data len
    i2c_transfer(client->adapter, &msg, 1);

    msg.addr = client->addr;			
	msg.flags = 0;				
	msg.buf = &buf;			
	msg.len = len;			

	return i2c_transfer(client->adapter, &msg, 1);
}

static void ap3216c_write_reg(struct newleddev *dev, u8 reg, u8 data)
{
	u8 buf = 0;
	buf = data;
	ap3216c_write_regs(dev, reg, &buf, 1);
}


static int ap3216c_read_regs(struct newleddev *dev, u8 reg, void *val, int len)
{ 
    int ret;
	struct i2c_msg msg[2];
	struct i2c_client *client = (struct i2c_client *)dev->private_data;

	// msg[0]write data addr
	msg[0].addr = client->addr;			
	msg[0].flags = 0;					//write: 0   read: I2C_M_RD
	msg[0].buf = &reg;					// data
	msg[0].len = 1;						// data len

	// msg[1]read data addr
	msg[1].addr = client->addr;			
	msg[1].flags = I2C_M_RD;			
	msg[1].buf = val;					
	msg[1].len = len;					

	ret = i2c_transfer(client->adapter, msg, 2);
	if(ret == 2) {
		ret = 0;
	} else {
		printk("i2c rd failed=%d reg=%06x len=%d\n",ret, reg, len);
		ret = -EREMOTEIO;
	}
	return ret;
}

static unsigned char ap3216c_read_reg(struct newleddev *dev, u8 reg)
{
	u8 data = 0;

	ap3216c_read_regs(dev, reg, &data, 1);
	return data;
}
static const struct of_device_id ap3216c_match[] = {
	{ .compatible = "david,ap3216c", },
	{ },
};

static const struct i2c_device_id ap3216c_id[] = {
	{ "david,ap3216c", },
	{},
};

static int ap3216c_remove(struct i2c_client *client){
    
    return 0;
}

static int ap3216c_open(struct file *file)
{
    printk("open\r\n");
    file->private_data = &leddev;
    ap3216c_write_reg(&leddev, AP3216C_SYSTEMCONG, 0x04);
    mdelay(60);
    ap3216c_write_reg(&leddev, AP3216C_SYSTEMCONG, 0x03);
    mdelay(60);
    u8 a;
    //ap3216c_read_regs(&leddev, AP3216C_SYSTEMCONG, &a, 1, 1);
    printk("%#x\r\n", a);
    return 0;
}
static int ap3216c_release(struct file *file)
{
    printk("release\r\n");
    return 0;
}
static int ap3216c_read(struct file *filp, char __user *buf, size_t size)
{
    printk("read\r\n");
    int i = 0;
    unsigned char buff[6];
    for(i = 0; i < 6; i++)	
    {
        buff[i] = ap3216c_read_reg(&leddev, AP3216C_IRDATALOW + i);	
    }
    short ps;
    ps = ((unsigned short)(buff[5] & 0X3F) << 4) | (buff[4] & 0X0F); 
    printk("PS: %d\r\n", ps);
    int err;
    err = copy_to_user(buf, &ps, sizeof(ps));
    mdelay(1000);
    return 0;
}

const struct file_operations ap3216fop = {
    .owner = THIS_MODULE,
	.open = ap3216c_open,
	.release = ap3216c_release,
    .read = ap3216c_read
};

static int ap3216c_probe(struct i2c_client *client, const struct i2c_device_id *id){
    printk("match ap3216c\r\n");
    int ret;
    if(leddev.major)
    {
        leddev.devid = MKDEV(leddev.major, 0);
        ret = register_chrdev_region(leddev.devid, 1, char_name);
    }
    else
    {
        ret = alloc_chrdev_region(&leddev.devid, 0, 1, char_name);
        leddev.major = MAJOR(leddev.devid);
        leddev.minor = MINOR(leddev.devid);
    }
    printk("major = %d,  minor = %d \r\n", leddev.major, leddev.minor);
    leddev.cdev.owner = THIS_MODULE;
    cdev_init(&leddev.cdev, &ap3216fop);
    cdev_add(&leddev.cdev, leddev.devid, 1);
    leddev.class = class_create(ap3216fop.owner, char_name);
    leddev.device = device_create(leddev.class, NULL, leddev.devid, NULL, char_name);
    leddev.private_data = client;
    return 0;
}

static struct i2c_driver ap3216c_driver = {
	.driver = {
		.name = "david_ap3216",
		.owner = THIS_MODULE,
		.of_match_table = ap3216c_match,
	},
	.probe = ap3216c_probe,
	.remove = ap3216c_remove,
	.id_table = ap3216c_id,
};

static void __exit newledexit(void)
{
    //void device_destroy(struct class *class, dev_t devt)
    device_destroy(leddev.class, leddev.devid);
    //void class_destroy(struct class *cls);
    class_destroy(leddev.class);
    i2c_del_driver(&ap3216c_driver);
}

static int __init newledinit(void)
{
   int ret = 0;
   ret = i2c_add_driver(&ap3216c_driver);
   return ret;
}



module_init(newledinit);
module_exit(newledexit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("David");