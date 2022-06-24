#include "core_semaphore.h"


void thread_sema_init(){
    sema_init(&dev.timer_sema1,0);
    sema_init(&dev.timer_sema2,0);
    sema_init(&dev.control_sema,0);
}