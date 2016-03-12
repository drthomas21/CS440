#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

struct list_head birthdayList;

int simple_init(void)
{
	struct birthday *ptr;
	int i = 5;	
	
	INIT_LIST_HEAD(&birthdayList);
	printk(KERN_INFO "Loading Module\n");	
	
	do {
		struct birthday *person;
		person = kmalloc(sizeof(*person), GFP_KERNEL);
		
		person->day = i * 2;
		person->month = i;
		person->year = 1999 + (i * 3);
		INIT_LIST_HEAD(&person->list);
		list_add_tail(&person->list,&birthdayList);
		i--;
	} while(i > 0);
	
	list_for_each_entry(ptr, &birthdayList, list) {
		printk(KERN_INFO "Day: %d Month: %d Year: %d\n",ptr->day,ptr->month,ptr->year);
	}
	return 0;
}

void simple_exit(void)
{
	struct birthday *ptr, *next;
	list_for_each_entry_safe(ptr,next, &birthdayList, list) {
		list_del(&ptr->list);
		printk("Removing item from list\n");
		kfree(ptr);
	}
	
	list_for_each_entry(ptr, &birthdayList, list) {
		printk(KERN_INFO "Day: %d Month: %d Year: %d\n",ptr->day,ptr->month,ptr->year);
	}
	
	printk(KERN_INFO "Removing Module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");