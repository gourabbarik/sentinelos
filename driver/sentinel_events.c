#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define DEVICE_NAME "sentinel_events"
#define CLASS_NAME "sentinel"

#define EVENT_BUFFER_SIZE 256

static dev_t device_number;
static struct cdev sentinel_cdev;
static struct class *sentinel_class;
static struct device *sentinel_device;

static char event_buffer[EVENT_BUFFER_SIZE];
static size_t event_length;

static DEFINE_MUTEX(event_mutex);

static int sentinel_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "sentinel_events: device opened\n");
    return 0;
}

static int sentinel_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "sentinel_events: device closed\n");
    return 0;
}

static ssize_t sentinel_write(
    struct file *file,
    const char __user *buffer,
    size_t count,
    loff_t *offset)
{
    size_t bytes_to_copy;

    if (count == 0)
        return 0;

    bytes_to_copy = count;

    if (bytes_to_copy >= EVENT_BUFFER_SIZE)
        bytes_to_copy = EVENT_BUFFER_SIZE - 1;

    if (mutex_lock_interruptible(&event_mutex))
        return -ERESTARTSYS;

    if (copy_from_user(event_buffer, buffer, bytes_to_copy)) {
        mutex_unlock(&event_mutex);
        return -EFAULT;
    }

    event_buffer[bytes_to_copy] = '\0';
    event_length = bytes_to_copy;

    printk(
        KERN_INFO
        "sentinel_events: event received: %s\n",
        event_buffer
    );

    mutex_unlock(&event_mutex);

    return bytes_to_copy;
}

static ssize_t sentinel_read(
    struct file *file,
    char __user *buffer,
    size_t count,
    loff_t *offset)
{
    size_t bytes_to_copy;

    if (mutex_lock_interruptible(&event_mutex))
        return -ERESTARTSYS;

    if (event_length == 0) {
        mutex_unlock(&event_mutex);
        return 0;
    }

    bytes_to_copy = event_length;

    if (count < bytes_to_copy)
        bytes_to_copy = count;

    if (copy_to_user(buffer, event_buffer, bytes_to_copy)) {
        mutex_unlock(&event_mutex);
        return -EFAULT;
    }

    event_length = 0;

    mutex_unlock(&event_mutex);

    return bytes_to_copy;
}

static struct file_operations sentinel_fops = {
    .owner = THIS_MODULE,
    .open = sentinel_open,
    .release = sentinel_release,
    .read = sentinel_read,
    .write = sentinel_write,
};

static int __init sentinel_init(void)
{
    int result;

    result = alloc_chrdev_region(
        &device_number,
        0,
        1,
        DEVICE_NAME
    );

    if (result < 0) {
        printk(KERN_ERR
               "sentinel_events: failed to allocate device number\n");
        return result;
    }

    cdev_init(&sentinel_cdev, &sentinel_fops);

    result = cdev_add(
        &sentinel_cdev,
        device_number,
        1
    );

    if (result < 0) {
        printk(KERN_ERR
               "sentinel_events: failed to add cdev\n");

        unregister_chrdev_region(
            device_number,
            1
        );

        return result;
    }

    sentinel_class = class_create(CLASS_NAME);

    if (IS_ERR(sentinel_class)) {
        cdev_del(&sentinel_cdev);

        unregister_chrdev_region(
            device_number,
            1
        );

        return PTR_ERR(sentinel_class);
    }

    sentinel_device = device_create(
        sentinel_class,
        NULL,
        device_number,
        NULL,
        DEVICE_NAME
    );






    if (IS_ERR(sentinel_device)) {
        class_destroy(sentinel_class);
        cdev_del(&sentinel_cdev);

        unregister_chrdev_region(
            device_number,
            1
        );

        return PTR_ERR(sentinel_device);
    }

    printk(KERN_INFO
           "sentinel_events: module loaded\n");

    printk(KERN_INFO
           "sentinel_events: device created at /dev/%s\n",
           DEVICE_NAME);

    return 0;
}

static void __exit sentinel_exit(void)
{
    device_destroy(
        sentinel_class,
        device_number
    );

    class_destroy(sentinel_class);

    cdev_del(&sentinel_cdev);

    unregister_chrdev_region(
        device_number,
        1
    );

    printk(KERN_INFO
           "sentinel_events: module unloaded\n");
}

module_init(sentinel_init);
module_exit(sentinel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SentinelOS");
MODULE_DESCRIPTION(
    "SentinelOS Linux character device diagnostic event channel"
);
