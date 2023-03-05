#ifndef _SCHED_

#define _SCHED_

#include <stdbool.h>
#include <stdint.h>


typedef struct {
  uint32_t period;
  
  void (*ptr)();
    
}task;
void createTask(task T);
void scheduling(void);
#endif 