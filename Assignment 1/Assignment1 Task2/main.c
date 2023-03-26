#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdbool.h>
//#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"


xTaskHandle task2_handle;

void PortF_init()
{
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2);
  GPIOUnlockPin(GPIOF_BASE,GPIO_PIN_1|GPIO_PIN_2);
  
  
}




void red(void *pvParameters)
{

	for(;;){
	GPIOPinWrite(GPIOF_BASE,GPIO_PIN_1,GPIOPinRead(GPIOF_BASE,GPIO_PIN_1)?0:GPIO_PIN_1);
		
	vTaskDelay(pdMS_TO_TICKS(5));
	}
	
}
void blue(void *pvParameters)
{

	for(;;){
	GPIOPinWrite(GPIOF_BASE,GPIO_PIN_2,GPIOPinRead(GPIOF_BASE,GPIO_PIN_2)?0:GPIO_PIN_2);
		
	vTaskDelay(pdMS_TO_TICKS(5));
	
	}

}



int main()
{
	PortF_init();

	xTaskCreate(red,"red",256,NULL,1,NULL);
	xTaskCreate(blue,"blue",256,NULL,2,&task2_handle);

	// Startup of the FreeRTOS scheduler.  The program should block here.  
	vTaskStartScheduler();
	
	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}
void vApplicationIdleHook(void)
{
	
	vTaskDelete(task2_handle);
}
