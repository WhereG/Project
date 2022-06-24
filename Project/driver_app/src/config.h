#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/semaphore.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>



#define BUF_SIZE        256

#define digitalSet(x,value)   *x = value 


enum work_state_e{
    FIRST_BEGIN = 0x01,
    FIRST_CONTINUE = 0x02,
    FIRST_SUSPEND = 0x04,
    FIRST_STOP = 0x08,

    SECOND_BEGIN = 0x10,
    SECOND_CONTINUE = 0x20,
    SECOND_SUSPEND = 0x40,
    SECOND_STOP = 0x80,
};


typedef struct {
    //character device
    struct cdev cdev;
    uint8_t demo_buf[BUF_SIZE];

    //semaphore
    struct semaphore timer_sema1;
    struct semaphore timer_sema2;
    struct semaphore control_sema;

    //count numbers
    int num1;
    int num2;

    //thread work state
    uint8_t work_state;
    int thread_state;
}core_dev_t;

extern core_dev_t dev;
#endif 