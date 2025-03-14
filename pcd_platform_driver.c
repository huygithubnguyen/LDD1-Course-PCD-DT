#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>

#include "platform_data.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__

#define NO_OF_DEVICES       4

#define MEM_SIZE_MAX_PCDEV1 1024
#define MEM_SIZE_MAX_PCDEV2 512
#define MEM_SIZE_MAX_PCDEV3 1024
#define MEM_SIZE_MAX_PCDEV4 512

/* pseudo device's memory */
char device_buffer_pcdev1[MEM_SIZE_MAX_PCDEV1];
char device_buffer_pcdev2[MEM_SIZE_MAX_PCDEV2];
char device_buffer_pcdev3[MEM_SIZE_MAX_PCDEV3];
char device_buffer_pcdev4[MEM_SIZE_MAX_PCDEV4];

/* Device private data structure */
struct pcdev_private_data {
    char *buffer;
    dev_t dev_num;
    struct pcdev_platform_data pdata;
    struct cdev cdev;

    struct device *pcdev_device;
};

/* Driver private data stucture */
struct pcdrv_private_data {
    int total_devices;
    dev_t device_number;
    struct class *class_pcd;
    struct device *device_pcd;
    struct pcdev_private_data pcdev_data[NO_OF_DEVICES];
};

#define RDONLY  0x01
#define WRONLY  0x10
#define RDWR    0x11

static int check_permission(int dev_perm, int acc_mode) {


    return 0;
}

struct pcdrv_private_data pcdrv_data =
{
    .total_devices = NO_OF_DEVICES,
    .pcdev_data = {
        [0] = {
            .buffer = device_buffer_pcdev1,
        },
        [1] = {
            .buffer = device_buffer_pcdev2,
        },
        [2] = {
            .buffer = device_buffer_pcdev3,
        },
        [3] = {
            .buffer = device_buffer_pcdev4,
        },
    }
};

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
    return 0;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    return count;
}

ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    return count;
}

int pcd_open(struct inode *inode, struct file *filp)
{
    int ret;

    return ret;
}

int pcd_release(struct inode *inode, struct file *filp)
{
    pr_info("Release was successful\n");
    return 0;
}

/* file operations of driver */
struct file_operations pcd_fops = {
    .open = pcd_open,
    .release = pcd_release,
    .read = pcd_read,
    .write = pcd_write,
    .owner = THIS_MODULE,
};

int pcd_platform_driver_probe(struct platform_device *pdev)
{
    int ret;
    struct pcdev_private_data *pcdev_data;
    struct pcdev_platform_data *pdata;

    pr_info("A device is detected\n");

#if 0
    /* 1. Get the platform data */
    pdata = (struct pcdev_platform_data *)dev_get_platdata(&pdev->dev);
    if (!pdata) {
        pr_info("No platform data available\n");
        return -EINVAL;
    }

    /* 2. Dynamically allocate memory for the device private data */
    pcdev_data = devm_kzalloc(&pdev->dev, sizeof(*pcdev_data), GFP_KERNEL);
    if (!pcdev_data) {
        pr_info("Cannot allocate memory\n");
        return -ENOMEM;
    }
    dev_set_drvdata(&pdev->dev, pcdev_data);
    pcdev_data->pdata.size = pdata->size;
    pcdev_data->pdata.perm = pdata->perm;
    pcdev_data->pdata.serial_number = pdata->serial_number;

    pr_info("Device serial number %s\n", pcdev_data->pdata.serial_number);
    pr_info("Device size %d\n", pcdev_data->pdata.size);
    pr_info("Device permission %d\n", pcdev_data->pdata.perm);

    /* 3. Dynamically allocate memory for the device buffer using size
    information from the platform data */
    pcdev_data->buffer = devm_kzalloc(&pdev->dev, pcdev_data->pdata.size, GFP_KERNEL);
    if (!pcdev_data->buffer) {
        devm_kfree(&pdev->dev, pcdev_data);
        pr_info("Cannot allocate memory\n");
        return -ENOMEM;
    }

    /* 4. Get the device number */
    pcdev_data->dev_num = pcdrv_data.device_number + pdev->id;
    pr_info("Device number %d\n", pcdev_data->dev_num);

    /* 5. Do cdev init and cdev add */
    cdev_init(&pcdev_data->cdev, &pcd_fops);
    pcdev_data->cdev.owner = THIS_MODULE;
    ret = cdev_add(&pcdev_data->cdev, pcdev_data->dev_num, 1);
    if (ret < 0) {
        devm_kfree(&pdev->dev, pcdev_data->buffer);
        devm_kfree(&pdev->dev, pcdev_data);
        pr_info("Cdev add failed\n");
        return ret;
    }

    /* 6. Create device file for the detected platform device */
    pcdev_data->pcdev_device = device_create(pcdrv_data.class_pcd, NULL, pcdev_data->dev_num, NULL, "pcdev-%d", pdev->id);
    if (IS_ERR(pcdev_data->pcdev_device)) {
        cdev_del(&pcdev_data->cdev);
        devm_kfree(&pdev->dev, pcdev_data->buffer);
        devm_kfree(&pdev->dev, pcdev_data);
        pr_info("Device create failed\n");
        return PTR_ERR(pcdev_data->pcdev_device);
    }

    pcdrv_data.total_devices++;

    /* 7. Error handling */

    pr_info("The probe was successful\n");
#endif
    return 0;
}

int pcd_platform_driver_remove(struct platform_device *pdev)
{
#if 0
    struct pcdev_private_data *pcdev_data = dev_get_drvdata(&pdev->dev);

    /* 1. Remove a device that was created with device create() */
    device_destroy(pcdrv_data.class_pcd, pcdev_data->dev_num);

    /* 2. Remove a cdev entry from the system */
    cdev_del(&pcdev_data->cdev);

    pcdrv_data.total_devices--;
#endif

    pr_info("A device is removed\n");
    return 0;
}

struct platform_device_id pcdev_ids[] = {
    [0] = {
        .name = "pcdev-A1x",
        .driver_data = 100,
    },
    [1] = {
        .name = "pcdev-B1x",
        .driver_data = 200,
    },
    [2] = {
        .name = "pcdev-C1x",
        .driver_data = 300,
    },
    [3] = {
        .name = "pcdev-D1x",
        .driver_data = 400,
    },
    {}
};

struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .id_table = pcdev_ids,
    .driver = {
        .name = "pseudo-char-device"
    }
};

static int __init pcd_driver_init(void)
{
    #if 0
    int ret;
    /* 1. Dynamically allocate a device number for MAX_DEVICES */
    ret = alloc_chrdev_region(&pcdrv_data.device_number, 0, NO_OF_DEVICES, "pcdevs");
    if (ret < 0) {
        pr_err("Alloc chrdev failed\n");
        return ret;
    }

    /* 2. Create a device class under /sys/class */
    pcdrv_data.class_pcd = class_create("pcd_class");
    if (IS_ERR(pcdrv_data.class_pcd)) {
        pr_err("Class creation failed\n");
        ret = PTR_ERR(pcdrv_data.class_pcd);
        unregister_chrdev_region(pcdrv_data.device_number, NO_OF_DEVICES);
        return ret;
    }

    /* 3. Register a platform driver */
    platform_driver_register(&pcd_platform_driver);
    pr_info("Pcd Platform Driver Loaded\n");
#endif
    return 0;
}

static void __exit pcd_driver_cleanup(void)
{
    #if 0
    /* 1. Unregister platform driver */
    platform_driver_unregister(&pcd_platform_driver);

    /* 2. Class destroy */
    class_destroy(pcdrv_data.class_pcd);

    /* 3. Unregister device number */
    unregister_chrdev_region(pcdrv_data.device_number, NO_OF_DEVICES);
#endif
    pr_info("Pcd Platform Driver Unoaded\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huy Nguyen");
MODULE_DESCRIPTION("A pseudo character driver which handles multiple devices");
