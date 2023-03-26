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
  GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
  GPIOUnlockPin(GPIOF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
  
  
}


uint32_t red_parameters[2]={GPIO_PIN_1,1000};
uint32_t blue_parameters[2]={GPIO_PIN_2,2000};
uint32_t green_parameters[2]={GPIO_PIN_3,3000};



void toggle_led(void *pvParameters)
{
	uint32_t *params= (uint32_t*) pvParameters;
	for(;;){
	GPIOPinWrite(GPIOF_BASE,params[0],GPIOPinRead(GPIOF_BASE,params[0])?0:params[0]);
	vTaskDelay( pdMS_TO_TICKS( params[1] ));
	}
	
}

void red(void *pvParameters)
{
	TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();

	
	for (;;) {
		
		GPIOPinWrite(GPIOF_BASE,GPIO_PIN_1,GPIOPinRead(GPIOF_BASE,GPIO_PIN_1)?0:GPIO_PIN_1);
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 1000 ) );
		
		
		//vTaskDelay( pdMS_TO_TICKS( 1000 ));
		

	}

}

void blue(void *pvParameters)
{
	
	TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		
		GPIOPinWrite(GPIOF_BASE,GPIO_PIN_2,GPIOPinRead(GPIOF_BASE,GPIO_PIN_2)?0:GPIO_PIN_2);
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 2000 ) );
		
		//vTaskDelay( pdMS_TO_TICKS( 2000 ));
		

	}

}	
void green(void *pvParameters)
{
	
	TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		
		GPIOPinWrite(GPIOF_BASE,GPIO_PIN_3,GPIOPinRead(GPIOF_BASE,GPIO_PIN_3)?0:GPIO_PIN_3);
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 3000 ) );
		//vTaskDelay( pdMS_TO_TICKS( 3000 ));
		

	}

}	


int main()
{
	PortF_init();
/*	xTaskCreate(red, "red", 256, NULL, 1, NULL);
	
	xTaskCreate(blue, "blue", 256, NULL, 2, NULL);
	
	xTaskCreate(green, "green", 256, NULL, 3, NULL);*/
	xTaskCreate(toggle_led,"red",256,(void*)red_parameters,1,NULL);
	xTaskCreate(toggle_led,"blue",256,(void*)blue_parameters,1,NULL);
	xTaskCreate(toggle_led,"green",256,(void*)green_parameters,1,NULL);

	// Startup of the FreeRTOS scheduler.  The program should block here.  
	vTaskStartScheduler();
	
	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}
