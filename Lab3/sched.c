#include "sched.h"


extern volatile uint32_t  tick_ctr;
task Tasks[40];
uint8_t index=0;
void createTask(task T)
{
  Tasks[index]=T;
  index++;
}

void scheduling(void)
{
  
  int prev=0;
  while(1){
    if (prev==tick_ctr)
        continue;
    else
      prev=tick_ctr;
    for(int i =0;i<index;i++)
    {
      //check for condition
      if(tick_ctr % Tasks[i].period==0)
      {
        Tasks[i].ptr();
      }
    }
  }
}