#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "tm4c123gh6pm.h"
#include "sched.h"


volatile uint32_t tick_ctr=0;
void SysTickHandler(void);


void SysTickHandler(void)
{
  tick_ctr++;
  
  
}

void PortF_init()
{
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
  GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
  
  
}
void Systick_init()
{
  IntMasterDisable();
  SysTickDisable();
  SysTickIntDisable();
  
  SysTickPeriodSet(1*16000-1);
  SysTickIntRegister(SysTickHandler);
  
  SysTickEnable();
  SysTickIntEnable();
  
  IntMasterEnable();
}



  
void task1(void){
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1)?0:GPIO_PIN_1);
}
void task2(void){
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2)?0:GPIO_PIN_2);
}
void task3(void){
GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_3)?0:GPIO_PIN_3);
}
  
  
  

int main()
{
  PortF_init();
  Systick_init();
  //Red
  task t1= {5000,task1};
  task t2= {10000,task2};
  task t3= {20000,task3};
  createTask(t1);
  createTask(t2);
  createTask(t3);
  
  
  scheduling();
  
}


