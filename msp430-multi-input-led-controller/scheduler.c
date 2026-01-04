
#include <stdint.h>
#include "scheduler.h"


void scheduler_run(task_t tasks[],const uint8_t num_of_tasks,uint16_t now) {
    for (uint8_t i = 0; i < num_of_tasks; ++i ) {
        run_task(&tasks[i],now);
    }
}

void run_task(task_t * task, uint16_t now) {
    if ((int16_t)(now - task->next_run) >= 0){ // need to enforce timer period to half of 2^16 due to signed integer conversion here 
        task->fn(now);
        task->next_run += task->period_ticks;
    }
}
