#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init m_init(void)
{
	pr_info("my_module\n");
	return 0;
}

static void __exit m_exit(void)
{
	pr_info("my_module\n");
}

// Declare special functions
module_init(m_init);
module_exit(m_exit);

// These uppercase macro will be added to informative section of module (.modinfo)
MODULE_AUTHOR("Jérôme Pouiller");
MODULE_DESCRIPTION("A pedagogic Hello World");
MODULE_LICENSE("Proprietary");
MODULE_VERSION("1.1");
