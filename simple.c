#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/* this function is called when the module is loaded */

int simple_init (void){
  printk(KERN_INFO "loading Module\n");
  return 0;
}

/* this function is called when module is removed */

void simple_exit(void){
  printk(KERN_INFO "removing module\n");
}

/* macros for registering module entry and exit points */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENCE("GPL"),
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
