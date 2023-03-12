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

void vPeriodicTask(void *pvParameters)
{
	
	
	for (;;) {
		GPIOPinWrite(GPIOF_BASE,GPIO_PIN_1,GPIOPinRead(GPIOF_BASE,GPIO_PIN_1)?0:GPIO_PIN_1);
		vTaskDelay( pdMS_TO_TICKS( 1000 ));

	}

}	



int main()
{
	PortF_init();
	xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);

	// Startup of the FreeRTOS scheduler.  The program should block here.  
	vTaskStartScheduler();
	
	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}
