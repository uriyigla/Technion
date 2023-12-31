#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>

asmlinkage long sys_hello(void) {
    printk("Hello, World!\n");
    return 0;
}

asmlinkage long sys_set_weight(int weight) {
    if (weight < 0)
        return -EINVAL;
    current->weight = weight;
    return 0;
}

asmlinkage long sys_get_weight(void) {
    return current->weight;
}

asmlinkage long sys_get_ancestor_sum(void) {
    struct task_struct* it = current;
    int sum = it->weight;
    it = it->parent;
    while (it != NULL && it->pid >= 0) {
        sum += it->weight;
        if (it->pid == 0) {
            break;
        }
        it = it->parent;
    }
    return sum;
}

static struct task_struct * dfs_get_heaviest_descendant(struct task_struct *task) {
    if( task == NULL)
        return NULL;
    if (list_empty(&task->children))
        return task;
    struct task_struct* max_task = task;
    int max_weight = task->weight;
    struct task_struct* child= NULL;
    struct list_head* list= NULL;
    struct task_struct* child_task= NULL;

    list_for_each(list, &task->children){
        child = list_entry(list, struct task_struct, sibling);
        child_task = dfs_get_heaviest_descendant(child); //recursive call
        if (child_task != NULL) {
            if (child_task->weight > max_weight ||
                (child_task->weight == max_weight && child_task->pid < max_task->pid)) {
                max_task = child_task;
                max_weight = child_task->weight;
            }
        }
    }
    return max_task;
}

asmlinkage long sys_get_heaviest_descendant(void) {
    if (list_empty(&current->children))
        return -ECHILD;
    int max_weight = -1;
    struct task_struct *child = NULL;
    struct list_head *list = NULL;
    struct task_struct *child_task = NULL;
    struct task_struct *max_task = NULL;
    list_for_each(list, &current->children){
        child = list_entry(list, struct task_struct, sibling);
        child_task = dfs_get_heaviest_descendant(child);
        if (child_task != NULL) {
            if (child_task->weight > max_weight ||
                (child_task->weight == max_weight && child_task->pid < max_task->pid)) {
                max_task = child_task;
                max_weight = child_task->weight;
            }
        }
    }
    if(max_task == NULL)
        return -ECHILD;
    return max_task->pid;
}
