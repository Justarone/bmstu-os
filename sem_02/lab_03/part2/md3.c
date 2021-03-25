#include "md.h"
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Perestoronin Pavel");

static int __init md_init(void)
{
    printk("+ module md3 start!\n");
    printk("+ data string exported from md1 : %s\n", md1_data);
    printk("+ string returned md1_proc() is : %s\n", md1_proc());
    printk("+ data number exported from md1 : %d\n", md1_number);
    printk("+    md1_fact(5) call returns   : %d\n", md1_fact(5));
    return -1;
}

module_init(md_init);
