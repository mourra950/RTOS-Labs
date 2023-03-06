#include "sched.h"


extern volatile uint32_t  tick_ctr;
extern int flag;
task Tasks[40];
uint8_t index=0;
void createTask(task T)
{
  Tasks[index]=T;
  index++;
  task temp;
  
  for(int i =0;i<index;i++)
  {
    
    for(int j =0;j<index-1;j++)
    {
      if (Tasks[j].priority>Tasks[j+1].priority)
      {
        temp=Tasks[j];
        Tasks[j]=Tasks[j+1];
        Tasks[j+1]=temp;
      }
    }
  }
}

void scheduling(void)
{
  
  if(flag==1)
    return;
  
  for(int i =0;i<index;i++)
  {
    //check for condition
    if(tick_ctr % Tasks[i].period==0)
    {
      flag=1;
      Tasks[i].ptr();
    }
  }
}
