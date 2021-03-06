#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <asm/paravirt.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Asiagmaing");
MODULE_DESCRIPTION("proc create test and exploitation");

/*
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
*/

static ssize_t my_read(struct file *filp, char __user *buf, loff_t *f_pos){
	printk(KERN_INFO"file_operations read is hooked...\n");
	return count;
}

static ssize_t my_write(struct file *filp, char __user *buf, loff_t *f_pos){
	printk(KERN_INFO"file_operations write is hooked...\n");
	return count;
}

static struct file_operations my_fops = {
	.read = (void *)my_read,
	.write = (void *)my_write
};


static int my_proc_create(void){

	struct proc_dir_entry *my_proc_entry = proc_create("my_proc", 0666, NULL, &my_fops);
	printk(KERN_INFO"my_proc_entry pointer : %p\n", my_proc_entry);
/*	my_proc_entry->read_proc = my_proc_read;		->		this is old version  */
	return 0;
}

static void my_proc_exit(void){

	printk(KERN_INFO"module unloaded : my_proc\n");
	remove_proc_entry("my_proc", 0);

}

module_init(my_proc_create);
module_exit(my_proc_exit);
