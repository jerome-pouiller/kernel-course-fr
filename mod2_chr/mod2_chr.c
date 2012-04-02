#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

static int major = 0;
static int buf_size = 255;
static char *global_buf;
static int idx = 0;
static struct mutex mutex;

/*
 * File operations
 */
static ssize_t m_read(struct file *file, char *user_buf, size_t count, loff_t *ppos) {
    int ret;
    mutex_lock(&mutex);
    if (count > idx)
        count = idx;
    ret = copy_to_user(user_buf, global_buf, count);
    memmove(global_buf, global_buf + count, buf_size - count);
    idx -= count;
    mutex_unlock(&mutex);
    return count;
}

static ssize_t m_write(struct file *file, const char *user_buf, size_t count, loff_t *ppos) {
    int ret;

    mutex_lock(&mutex);
    if (count > buf_size - idx)
        count = buf_size - idx;
    ret = copy_from_user(global_buf + idx, user_buf, count);
    idx += count;
    mutex_unlock(&mutex);
    return count;
}

// FIXME: is usefull?
static int m_open(struct inode *inode, struct file *file) {
  return 0;
}

static int m_release(struct inode *inode, struct file *file) {
  return 0;
}

static struct file_operations m_fops = {
  .owner   = THIS_MODULE,
  .read    = m_read,
  .write   = m_write,
//  .ioctl   = m_ioctl,
  .open    = m_open,
  .release = m_release,
};

static int __init m_init(void) {
  int ret;

  mutex_init(&mutex);
  // Create a oops
  //*((int *) 0) = 1;

  global_buf = kcalloc(1, buf_size, GFP_KERNEL);
  if (!global_buf) {
      ret = -EAGAIN;
      goto err_mutex;
  }

  // Process acces to driver as soon as it is registered, so it last
  major = register_chrdev(major, "m_chrdev", &m_fops);
  if (major < 0) {
      ret = major;
      goto err_alloc;
  }

  pr_info("my_chardev: registered between <%d, 0> and <%d, 255>\n", major, major);
  return 0;

err_mutex:
  mutex_destroy(&mutex);
err_alloc:
  kfree(global_buf);
  return ret;
}

static void __exit m_exit(void) {
  //unregister_chrdev_region(MKDEV(major, 0), 256);
  unregister_chrdev(MKDEV(major, 0), "m_chrdev");
  mutex_destroy(&mutex);
  kfree(global_buf);
}

module_init(m_init);
module_exit(m_exit);
module_param(major, int, 0644);
module_param(buf_size, int, 0644);

MODULE_AUTHOR("Jérôme Pouiller");
MODULE_DESCRIPTION("A pedagogic character device");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.24");
MODULE_PARM_DESC(major, "Fix major to use (default: auto)");
MODULE_PARM_DESC(buf_size, "Size of buffer (default = 255)");
