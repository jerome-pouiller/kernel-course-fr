#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <mach/gpio.h>
#include <mach/at91_pio.h>
#include <mach/at91sam9260.h>

void __iomem *reg;
#define mask (1 << (AT91_PIN_PB21 % 32))

static int __init m_init(void) {
	// would return AT91SAM9260_BASE_PIOB:
	//void __iomem    *reg = pin_to_controller(pin);
	// would return 1 << 21:
	//unsigned        mask = pin_to_mask(pin);
	reg = ioremap(AT91SAM9260_BASE_PIOB, 0xff);
	
	writel(mask, reg + PIO_SODR);
	writel(mask, reg + PIO_OER);
	writel(mask, reg + PIO_PER);
	
	return 0;
}

static void __exit m_exit(void) {
	writel(mask, reg + PIO_CODR);
	writel(mask, reg + PIO_OER);
	writel(mask, reg + PIO_PER);
	iounmap(reg);
}

module_init(m_init);
module_exit(m_exit);

MODULE_AUTHOR("Jérôme Pouiller");
MODULE_DESCRIPTION("A pedagogic example of MMIO access");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.24");
