#include "core_char.h"



#define DEMO_MAJOR      236
#define DEMO_MINOR      0

#define COMMAND_A '1'
#define COMMAND_B '2'
#define COMMAND_C '3'
#define COMMAND_D '4'

#define COMMAND_E '5'
#define COMMAND_F '6'
#define COMMAND_G '7'
#define COMMAND_H '8'

#define COMMAND_I 'a'
#define COMMAND_J 's'
#define COMMAND_K 'd'
#define COMMAND_L 'f'



static long demo_ioctl(struct file *filp,unsigned int cmd,unsigned long arg){
    // printk(KERN_ERR "DEMO: Line %d,function %s() has been invoked!\n",__LINE__,__func__);
    switch(cmd){
        case COMMAND_A:
            digitalSet(&dev.work_state,((dev.work_state & 0xf0) | FIRST_BEGIN));
            break;
        case COMMAND_B:
            digitalSet(&dev.work_state,((dev.work_state & 0xf0) | FIRST_CONTINUE));           
            break;
        case COMMAND_C:
            digitalSet(&dev.work_state,((dev.work_state & 0xf0) | FIRST_SUSPEND));           
            break;
        case COMMAND_D:
            digitalSet(&dev.work_state,((dev.work_state & 0xf0) | FIRST_STOP));           
            break;


        case COMMAND_E:
            digitalSet(&dev.work_state,((dev.work_state & 0x0f) | SECOND_BEGIN));
            break;
        case COMMAND_F:
            digitalSet(&dev.work_state,((dev.work_state & 0x0f) | SECOND_CONTINUE));
            break;
        case COMMAND_G:
            digitalSet(&dev.work_state,((dev.work_state & 0x0f) | SECOND_SUSPEND));
            break;
        case COMMAND_H:
            digitalSet(&dev.work_state,((dev.work_state & 0x0f) | SECOND_STOP));
            break;


        case COMMAND_I:
            digitalSet(&dev.work_state,(FIRST_BEGIN | SECOND_BEGIN));
            break;
        case COMMAND_J:
            digitalSet(&dev.work_state,(FIRST_CONTINUE | SECOND_CONTINUE));
            break;
        case COMMAND_K:
            digitalSet(&dev.work_state,(FIRST_SUSPEND | SECOND_SUSPEND));
            break;
        case COMMAND_L:
            digitalSet(&dev.work_state,(FIRST_STOP | SECOND_STOP));
            break;
        
        default:
            printk("Ioctl Error...");
            return -EINVAL;
    }
    printk("Ioctl Command:%c\n",cmd);
    return 0;
}


static struct file_operations demo_fops = {
    .owner          = THIS_MODULE,
    // .llseek         = demo_llseek,
    // .read           = demo_read,
    // .write          = demo_write,
    .unlocked_ioctl = demo_ioctl,
    // .open           = demo_open,
    // .release        = demo_release,
};

int core_char_init(){
    int retval;

    dev_t devno = 0;

    devno = MKDEV(DEMO_MAJOR,DEMO_MINOR);

    retval = register_chrdev_region(devno,1,"demo_char");
    if(retval){
        printk("Character Device Register Failed!\n");
        goto out;
    }
    cdev_init(&dev.cdev,&demo_fops);

    dev.cdev.owner = THIS_MODULE;
    dev.cdev.ops = &demo_fops;

    retval = cdev_add(&dev.cdev,devno,1);
    if(retval){
        printk("Character Device Add Failed!\n");
        goto err;
    }
    printk("Character Device Add Successfully!\n");
out:
    return 1;

err:
    cdev_del(&dev.cdev);
    unregister_chrdev_region(devno,1);
    return 0;    
}

void core_char_exit(){
    dev_t devno = MKDEV(DEMO_MAJOR,DEMO_MINOR);
    cdev_del(&dev.cdev);
    unregister_chrdev_region(devno,1);
}