#include <linux/module.h>
#include <linux/platform_device.h>

#include "platform_data.h"

static void pcdev_release(struct device *dev)
{
    pr_info("pcdev_release\n");
}

//1. Create 2 platform data
struct pcdev_platform_data pcdev_pdata1 = {
    .size = 512,
    .perm = RDWR,
    .serial_number = "PCDEVABC123",
};

struct pcdev_platform_data pcdev_pdata2 = {
    .size = 1024,
    .perm = RDONLY,
    .serial_number = "PCDEVXYZ456",
};

//2. Create 2 platform devices
struct platform_device pcd_dev1 = {
    .name = "pseudo-A1x",
    .id = 0,
    .dev = {
        .platform_data = &pcdev_pdata1,
        .release = pcdev_release,
    },
};

struct platform_device pcd_dev2 = {
    .name = "pseudo-B1x",
    .id = 1,
    .dev = {
        .platform_data = &pcdev_pdata2,
        .release = pcdev_release,
    },
};

//
static int __init pcdev_platform_init(void)
{
    // Register the platform devices
    platform_device_register(&pcd_dev1);
    platform_device_register(&pcd_dev2);

    pr_info("pcd platform devices added\n");

    return 0;
}

static void __exit pcdev_platform_exit(void)
{
    platform_device_unregister(&pcd_dev1);
    platform_device_unregister(&pcd_dev2);

    printk("pcdev_platform_exit\n");
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sandeep S");
MODULE_DESCRIPTION("A simple platform driver");

