#include "core_timer.h"

static struct timer_list timer;

void timer_callback(struct timer_list *t){
    up(&dev.control_sema);
    mod_timer(&timer,jiffies + msecs_to_jiffies(1000));
}

int count_timer_init(void){
    timer_setup(&timer,timer_callback,0);
    add_timer(&timer);
    printk("Timer Init...\n");
    return 0;
}

void count_timer_exit(void){
    printk("Timer Exit...\n");
    del_timer(&timer);
}
