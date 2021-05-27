#include <linux/init.h>
#include <linux/types.h> /* size_t */
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/uaccess.h> /* copy_from/to_user */

#define BUFF_SIZE 64

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Robin Steidle");
MODULE_DESCRIPTION("RAM disk driver");

/* Declaration of memory.c functions */
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t memory_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void memory_exit(void);
int memory_init(void);

/* Structure that declares the usual file */
/* access functions */
struct file_operations memory_fops = {
    read: memory_read,
    write: memory_write,
    open: memory_open,
    release: memory_release
};

/* Declaration of the init and exit functions */
module_init(memory_init);
module_exit(memory_exit);
/* Global variables of the driver */
/* Major number */
int memory_major = 60;
/* Buffer to store data */
char *memory_buffer;

static int Device_Open = 0;  // prevent multiple access to device

int memory_init(void) {
    int result;
    /* Registering device */
    result = register_chrdev(memory_major, "memory", &memory_fops);
    if (result < 0) {
        printk("<1>memory: cannot obtain major number %d\n", memory_major);
        return result;
    }

    /* Allocating memory for the buffer */
    memory_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL);
    if (!memory_buffer) {
        result = -ENOMEM;
        goto fail;
    }
    memset(memory_buffer, 0, BUFF_SIZE);
    printk("<1>Inserting memory module\n");
    return 0;
    fail:
    memory_exit();
    return result;
}

void memory_exit(void) {
    /* Freeing the major number */
    unregister_chrdev(memory_major, "memory");
    /* Freeing buffer memory */
    if (memory_buffer) {
        kfree(memory_buffer);
    }
    printk("<1>Removing memory module\n");
}

int memory_open(struct inode *inode, struct file *filp) {
    if (Device_Open)
        return -EBUSY;

    Device_Open++;
    try_module_get(THIS_MODULE);

    return 0;
}

int memory_release(struct inode *inode, struct file *filp) {
    Device_Open--;
    module_put(THIS_MODULE);
    return 0;
}

ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
    size_t length = BUFF_SIZE;

    if (*f_pos >= length)
        return 0;

    length -= (size_t)(*f_pos);
    memory_buffer += (size_t)(*f_pos);

    if(count > length)
        count = length;

    if (copy_to_user(buf, memory_buffer, count))
        return -EFAULT;

    *f_pos += (loff_t)count;

    return (ssize_t)count;
}

ssize_t memory_write(struct file *filp, const char *buff, size_t count, loff_t *f_pos) {
    copy_from_user(memory_buffer,buff,count);
    return count;
}


