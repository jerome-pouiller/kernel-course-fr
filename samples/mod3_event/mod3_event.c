#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kfifo.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/poll.h>

static int major = 0;

DECLARE_WAIT_QUEUE_HEAD(my_queue);
DEFINE_KFIFO(my_fifo, char, 256);

static ssize_t m_read(struct file *file, char *user_buf, size_t count, loff_t *ppos) {
	int ret, copied;

//	pr_info("Get read: %d\n", kfifo_is_empty(&my_fifo));
	wait_event_interruptible(my_queue, !kfifo_is_empty(&my_fifo));
	ret = kfifo_to_user(&my_fifo, user_buf, count, &copied);
//	pr_info("Returning : %d\n", copied ? copied : ret);
	return copied ? copied : ret;
}

static ssize_t m_write(struct file *file, const char *user_buf, size_t count, loff_t *ppos) {
	int ret, copied;
	
	ret = kfifo_from_user(&my_fifo, user_buf, count, &copied);
	wake_up(&my_queue);
	return copied ? copied : ret;
}


static unsigned int m_poll(struct file *filp, poll_table *wait) {
	int mask = POLLOUT | POLLWRNORM;
	poll_wait(filp, &my_queue, wait);
	if (!kfifo_is_empty(&my_fifo))
		mask |= POLLIN | POLLRDNORM;
	// pr_info("Poll returning : %d\n", mask);
	return mask;
}

static struct file_operations m_fops = {
	.owner   = THIS_MODULE,
	.read    = m_read,
	.write   = m_write,
	.poll    = m_poll,
};

static int __init m_init(void) {
	// Process acces to driver as soon as it is registered, so it last
	major = register_chrdev(major, "m_chrdev", &m_fops);
	if (major <= 0) {
		return major;
	}

	pr_info("my_chardev: registered between <%d, 0> and <%d, 255>\n", major, major);
	return 0;
}

static void __exit m_exit(void) {
	unregister_chrdev(major, "m_chrdev");
}

module_init(m_init);
module_exit(m_exit);
module_param(major, int, 0644);

MODULE_AUTHOR("Jérôme Pouiller");
MODULE_DESCRIPTION("A pedagogic character device");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.24");
MODULE_PARM_DESC(major, "Fix major to use (default: auto)");
