#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "tm4c123gh6pm.h"

uint32_t start;
volatile uint32_t tick_ctr=0;
void SysTickHandler(void);

uint32_t stack_RedBlinky[40];
uint32_t *sp_RedBlinky = &stack_RedBlinky[40];

uint32_t stack_BlueBlinky[40];
uint32_t *sp_BlueBlinky = &stack_BlueBlinky[40];

void SysTickHandler(void)
{
  tick_ctr++;
  
  
}

void PortF_init()
{
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2);
  GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2);
  
  
}
void Systick_init()
{
  IntMasterDisable();
  SysTickDisable();
  SysTickIntDisable();
  
  SysTickPeriodSet(100*16000-1);
  SysTickIntRegister(SysTickHandler);
  
  SysTickEnable();
  SysTickIntEnable();
  
  IntMasterEnable();
}


void delay(uint32_t u)
{
  IntMasterDisable();
  start=tick_ctr;
  IntMasterEnable();
  while(tick_ctr-start<u);
  
}

void blink_red()
{
  while(1){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
    delay(5);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0x00);
    delay(5);
    
  }
  
  
}

void blink_blue()
{
  while(1){
    
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
    delay(10);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0x00);
    delay(10);
  }
  
  
  
}
int main()
{
  PortF_init();
  Systick_init();
  //Red
  *(--sp_RedBlinky)=1<<24;
  *(--sp_RedBlinky)=(uint32_t)&blink_red;
  
  *(--sp_RedBlinky)=0xE;
  *(--sp_RedBlinky)=0xC;
  *(--sp_RedBlinky)=0x3;
  *(--sp_RedBlinky)=0x2;
  *(--sp_RedBlinky)=0x1;
  *(--sp_RedBlinky)=0x0;
  
  //Blue
  
  
  *(--sp_BlueBlinky)=1<<24;
  *(--sp_BlueBlinky)=(uint32_t)&blink_blue;
  *(--sp_BlueBlinky)=0xEE;
  *(--sp_BlueBlinky)=0xCC;
  *(--sp_BlueBlinky)=0x33;
  *(--sp_BlueBlinky)=0x22;
  *(--sp_BlueBlinky)=0x11;
  *(--sp_BlueBlinky)=0x00;
  
  while(1)
  {
    
  }
}


