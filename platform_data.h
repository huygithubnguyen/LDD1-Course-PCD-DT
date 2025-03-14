#ifndef PLATFORM_DATA_H
#define PLATFORM_DATA_H

/* Permission flags */
#define RDONLY  0x01  /* Read only */
#define WRONLY  0x02  /* Write only */
#define RDWR    0x03  /* Read and write */

/* Platform data structure for pseudo character device */
struct pcdev_platform_data {
    int size;
    int perm;
    const char *serial_number;
};

#endif /* PLATFORM_DATA_H */

