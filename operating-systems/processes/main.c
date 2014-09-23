#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>

void print_processes_dfs(void);
void print_children_recursive(struct task_struct *task);


static int __init process_init( void ) {

    print_processes_dfs();

    return 0;
}



static void __exit process_exit( void ) {
    printk(KERN_INFO "Removing module\n");
}



void print_processes_dfs(void) {
    printk(KERN_INFO "%6s  %6s  %-12s\n", "ID", "STATE", "NAME");
    print_children_recursive(&init_task);
}


void print_children_recursive(struct task_struct *task) {

    struct task_struct *child_task;
    struct list_head *list;

    printk(KERN_INFO "%6d  %6ld  %-12s\n", task->pid, task->state, task->comm);


    list_for_each(list, &(task->children)) {
        child_task = list_entry(list, struct task_struct, sibling);
        print_children_recursive(child_task);
    }
}


module_init(process_init);
module_exit(process_exit);

MODULE_LICENSE("GPL");
