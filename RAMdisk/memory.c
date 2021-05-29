#define LICENSE "Dual BSD/GPL"
#define VERSION "0.01"
#define AUTHOR "Robin Steidle"

#include <linux/init.h>
#include <linux/types.h> /* size_t */
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/seq_file.h> // seq_printf()
#include <asm/uaccess.h> /* copy_from/to_user */

#define BUFF_SIZE 64

MODULE_LICENSE(LICENSE);
MODULE_VERSION(VERSION);
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION("RAM disk driver");

/* Declaration of memory.c functions */
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t memory_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void memory_show_fdinfo(struct seq_file *m, struct file *f);

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


int memory_major = 60;  // major number
char *memory_buffer;  // buffer for storing data

static int Device_Open = 0;  // prevent multiple access to device

int memory_init(void) {
    int result;

    result = register_chrdev(memory_major, "memory", &memory_fops);  // registering device
    if (result < 0) {
        printk("<1>memory: cannot obtain major number %d\n", memory_major);
        return result;
    }

    memory_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL);  // allocating memory
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
    unregister_chrdev(memory_major, "memory");  // freeing major number

    if (memory_buffer) {
        kfree(memory_buffer);  // freeing allocated memory
    }
    printk("<1>Removing memory module\n");
}

int memory_open(struct inode *inode, struct file *filp) {
    if (Device_Open)
        return -EBUSY;  // device already opened

    Device_Open++;
    try_module_get(THIS_MODULE);

    return 0;
}

int memory_release(struct inode *inode, struct file *filp) {
    Device_Open--;  // close device
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
    copy_from_user(memory_buffer,buff,count);  // copy user input string into the buffer
    return count;
}