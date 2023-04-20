#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

static int major = 0;
static char sdata[64] = {0};
static int sdatalen = 0;
static struct class *class_for_hello;

static ssize_t hello_read(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    copy_to_user(buf, sdata, sdatalen);
    return sdatalen;
}

static ssize_t hello_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    sdatalen = size;
    copy_from_user(sdata, buf, sdatalen);
    return sdatalen;
}

static struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .read = hello_read,
    .write = hello_write,
};

int __init hello_init(void)
{
    printk("hello drv init\n");
    major = register_chrdev(0, "hello_drv", &hello_fops);

    class_for_hello = class_create(THIS_MODULE, "helloclass");
    device_create(class_for_hello, NULL, MKDEV(major, 0), NULL, "hellodev"); /* /dev/hellodev*/

    return 0;
}

void __exit hello_exit(void)
{
    printk("hello drv exit\n");
    device_destroy(class_for_hello, MKDEV(major, 0));
    class_destroy(class_for_hello);
    unregister_chrdev(major, "hello_drv");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
