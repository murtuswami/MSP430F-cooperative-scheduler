#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
typedef void (*task_fn_t)(uint16_t); // task function type 

typedef struct {
    task_fn_t fn; 
    uint16_t period_ticks;
    uint16_t next_run;
} task_t;

void scheduler_run(task_t arr[],const uint8_t,uint16_t);

void run_task(task_t * task, uint16_t );


#endif 
