#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <mach/gpio.h>
#include <mach/at91_pio.h>
#include <mach/at91sam9260.h>

static int major = 0;
static int status = 0;
void __iomem *reg;

/*
 * File operations
 */
static ssize_t m_read(struct file *file, char *user_buf, size_t count, loff_t *ppos) {
	int ret;
    char buf = status + '0';
    ret = copy_to_user(user_buf, &buf, 1);
    return 1;
}

static ssize_t m_write(struct file *file, const char *user_buf, size_t count, loff_t *ppos) {
	int ret;
    char buf;
    ret = copy_from_user(&buf, user_buf, 1);
    status = (buf - '0') ? 1 : 0;
    // cf. include/mach/at91_pio.h#78
    if (status)
	    writel(1 << 21, reg + PIO_SODR);
    else 
	    writel(1 << 21, reg + PIO_CODR);
    writel(1 << 21, reg + PIO_OER);
    writel(1 << 21, reg + PIO_PER);
    return 1;
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
  // cf. include/mach/at91sam9260.h#L92
  reg = ioremap(0xFFFFF600, 0xff);

  // Process acces to driver as soon as it is registered, so it last
  major = register_chrdev(major, "m_mmio", &m_fops);
  if (major < 0) {
      ret = major;
      goto err_register;
  }
  writel(1 << 21, reg + PIO_SODR);
  writel(1 << 21, reg + PIO_OER);
  writel(1 << 21, reg + PIO_PER);
   
  pr_info("my_chardev: registered between <%d, 0> and <%d, 255>\n", major, major);
  return 0;

err_register:
  return ret;
}

static void __exit m_exit(void) {
  writel(1 << 21, reg + PIO_CODR);
  writel(1 << 21, reg + PIO_OER);
  writel(1 << 21, reg + PIO_PER);
  iounmap(reg);
}

module_init(m_init);
module_exit(m_exit);

MODULE_AUTHOR("Jérôme Pouiller");
MODULE_DESCRIPTION("A pedagogic example of MMIO access");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.24");
