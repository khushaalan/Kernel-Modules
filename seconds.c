#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

//Define the Macros
#define BUFFER_SIZE 128
#define PROC_NAME "seconds"
static unsigned long load_jiffies;
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

int proc_init(void){
    load_jiffies = jiffies;
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    return 0;
}

void proc_exit(void){
    remove_proc_entry(PROC_NAME, NULL);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos){
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    unsigned long elapsed_jiffies = jiffies - load_jiffies;
    unsigned long elapsed_seconds = elapsed_jiffies / HZ;
    unsigned long not_copied;
    if (completed) {completed = 0;return 0;}

    completed = 1;
    rv = sprintf(buffer, "The Number of Seconds: %lu\n", elapsed_seconds);
    printk(KERN_INFO "Printing Number of Seconds...\n");
    not_copied = copy_to_user(usr_buf, buffer, rv);

    if (not_copied) {return -EFAULT;}
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("Khushaalan Arjunan - A190409");
