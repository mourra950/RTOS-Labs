#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdbool.h>
//#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"





void PortF_init()
{
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2);
  GPIOUnlockPin(GPIOF_BASE,GPIO_PIN_1|GPIO_PIN_2);
  
  
}


uint32_t red_parameters[1]={GPIO_PIN_1};
uint32_t blue_parameters[1]={GPIO_PIN_2};



void toggle_led(void *pvParameters)
{

	uint32_t *params= (uint32_t*) pvParameters;
	for(;;){
	GPIOPinWrite(GPIOF_BASE,params[0],GPIOPinRead(GPIOF_BASE,params[0])?0:params[0]);
	}
	
}



int main()
{
	PortF_init();

	xTaskCreate(toggle_led,"red",256,(void*)red_parameters,1,NULL);
	//xTaskCreate(toggle_led,"blue",256,(void*)blue_parameters,1,NULL);
	xTaskCreate(toggle_led,"blue",256,(void*)blue_parameters,2,NULL);

	// Startup of the FreeRTOS scheduler.  The program should block here.  
	vTaskStartScheduler();
	
	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}
