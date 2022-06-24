#include "core_thread.h"


#define CLONE_KERNEL    (CLONE_FS | CLONE_FILES | CLONE_SIGHAND)


struct task_struct *task1_handle = NULL;
struct task_struct *task2_handle = NULL;
struct task_struct *control_handle = NULL;

void sema_control(){
    switch(dev.work_state & 0x0f){
        case FIRST_BEGIN:{
            dev.num1 = 1;
            digitalSet(&dev.work_state,((dev.work_state & 0xf0) | FIRST_CONTINUE));
            break;
        }
        case FIRST_CONTINUE:{
            up(&dev.timer_sema1);
            break;
        }
        case FIRST_SUSPEND:{
            break;
        }
        case FIRST_STOP:{
            dev.num1 = 1;
            break;
        }
    }
    switch (dev.work_state &0xf0){
        case SECOND_BEGIN:{
            dev.num2 = 1;
            digitalSet(&dev.work_state,((dev.work_state & 0x0f) | SECOND_CONTINUE));
            break;
        }
        case SECOND_CONTINUE:{
            up(&dev.timer_sema2);
            break;
        }
        case SECOND_SUSPEND:{
            break;
        }
        case SECOND_STOP:{
            dev.num2 = 1;
            break;
        }
    }
}


int kernel_count1(void *arg){
    dev.num1 = 1;
    while(!kthread_should_stop()){
        if(dev.thread_state){
            down(&dev.timer_sema1);
            printk("Thread1,num1:%d\n",dev.num1++);
            if(dev.num1 > 100){
                dev.num1 = 1;
            }
        }else{
            printk("Waiting Thread1 Exit\n");
        }
    }
    return 0;
}
int kernel_count2(void *arg){
    int times = 0;
    dev.num2 = 1;
    while(!kthread_should_stop()){
        if(dev.thread_state){
            down(&dev.timer_sema2);
            if(!(times % 2)){
                printk("Thread2,num2:%d\n",dev.num2++);
            }
            if(dev.num2 > 100){
                dev.num2 = 1;
            }
            if(times > 99){
                times = 0;
            }else{
                times ++;
            }
        }else{
            printk("Waiting Thread2 Exit\n");
        }
    }
    return 0;
}

int control_thread(void *arg){
    while(!kthread_should_stop()){
        if(dev.thread_state){
            down(&dev.control_sema);
            sema_control();
        }else{
            up(&dev.timer_sema1);
            up(&dev.timer_sema2);
            printk("Waiting Control Thread Exit\n");
        }
    }
    return 0;
}


int count_thread_init()
{
    printk("Thread Init...\n");
    task1_handle = kthread_run(kernel_count1,NULL,"count1 task");  
    task2_handle = kthread_run(kernel_count2,NULL,"count2 task");  
    control_handle = kthread_run(control_thread,NULL,"control task");  
    if(IS_ERR(task1_handle) || IS_ERR(task2_handle) || IS_ERR(control_handle)){
        dev.thread_state = 0;
        kthread_stop(task1_handle);
        kthread_stop(task2_handle);
        kthread_stop(control_handle);
    }
    return 0;
}

void count_thread_exit()
{   
    printk("Thread Exit...\n");
    kthread_stop(task1_handle);
    kthread_stop(task2_handle);
    kthread_stop(control_handle);
}
