#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <mach/gpio.h>
#include <mach/at91_pio.h>
#include <mach/at91sam9260.h>

struct my_dev_s {

};

struct my_dev_s my_dev;

static irqreturn_t handler(int irq, void *dev) {
   pr_info("Interrupt\n");
   return IRQ_HANDLED;
}

static int __init m_init(void) {
  int ret;
 
  ret = request_irq(gpio_to_irq(AT91_PIN_PB10), handler, IRQF_SHARED, "my interruption handler", &my_dev);
  //enable_irq(gpio_to_irq(AT91_PIN_PB10));

  pr_info("my_chardev: registered\n");
  return 0;
}

static void __exit m_exit(void) {
  free_irq(gpio_to_irq(AT91_PIN_PB10), &my_dev);
}

module_init(m_init);
module_exit(m_exit);

MODULE_AUTHOR("Jérôme Pouiller");
MODULE_DESCRIPTION("A pedagogic character device");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.24");
