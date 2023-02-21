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
  while(1)
  {
  blink_red();
  blink_blue();
 
  }
}


