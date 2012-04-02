#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

struct my_node_t {
	struct list_head node;
	char content;
};

LIST_HEAD(my_list);

static int __init m_init(void)
{
	struct my_node_t *new_node;
	char c;

	for (c = 'a'; c != 'f'; c++) {
		new_node = kmalloc(sizeof(new_node), GFP_KERNEL);
		new_node->content = c;
		list_add(&new_node->node, &my_list);
	}
	return 0;
}

static void __exit m_exit(void)
{
	struct my_node_t *i;
	struct my_node_t *tmp;
	list_for_each_entry(i, &my_list, node) {
		pr_info("my_module: %c\n", i->content);
	}
	list_for_each_entry_safe(i, tmp, &my_list, node) {
		list_del(&(i->node));
		kfree(i);
	}
}

// Declare special functions
module_init(m_init);
module_exit(m_exit);

// These uppercase macro will be added to informative section of module (.modinfo)
MODULE_AUTHOR("Jérôme Pouiller");
MODULE_DESCRIPTION("A pedagogic Hello World");
MODULE_LICENSE("Proprietary");
MODULE_VERSION("1.1");
