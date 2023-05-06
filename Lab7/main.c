#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdbool.h>
//#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "queue.h"
#include "semphr.h"
//#include "stdio.h"

QueueHandle_t xQueue;
SemaphoreHandle_t xSemaphore;

void initTask(void * params)
{
	for(;;)
	{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
  GPIOUnlockPin(GPIOF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIOF_BASE,GPIO_PIN_0);
	GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
 while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
GPIOPinTypeUART(GPIOA_BASE,GPIO_PIN_0|GPIO_PIN_1);
GPIOUnlockPin(GPIOA_BASE,GPIO_PIN_0|GPIO_PIN_1);
		UARTDisable(UART0_BASE);
	UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(),9600,UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE);
	UARTEnable(UART0_BASE);
vTaskSuspend(NULL);
	}
}



void PortF_init()
{
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
  GPIOUnlockPin(GPIOF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
  
  
}



void button_task(void *params)
{
	static uint8_t IncrementingCounter=0;
	for(;;)
	{
		if(!(GPIOPinRead(GPIOF_BASE,GPIO_PIN_0)))
		{
			vTaskDelay(pdMS_TO_TICKS(20));
			if(!(GPIOPinRead(GPIOF_BASE,GPIO_PIN_0)))
			{
				IncrementingCounter++;
				xQueueSendToBack(xQueue,&IncrementingCounter,0);
				
				xSemaphoreGive(xSemaphore);
				while(!(GPIOPinRead(GPIOF_BASE,GPIO_PIN_0)));
				
				
				
			}
			
			
			
		}
		
		
	}
	
	
	
}
void uart_task(void * params)
{
	xSemaphoreTake(xSemaphore,0);
	uint8_t buffer=0;
	for(;;)
	{
		xSemaphoreTake(xSemaphore,portMAX_DELAY);
		xQueueReceive(xQueue,&buffer,0);
		int arr[5];
		int cnt = 0;
		while(buffer!=0)
		{
			int digit = buffer%10;
			buffer/=10;
			arr[cnt] = digit;
			cnt++;
			
		}
		for(int i=cnt-1 ; i>=0 ; i--){
			UARTCharPut(UART0_BASE,arr[i]+'0');
		}
		UARTCharPut(UART0_BASE,' ');
		
		
		
	}
	
	
}







int main()
{
	
	xQueue = xQueueCreate( 20, sizeof(int) );
	
	
	xSemaphore = xSemaphoreCreateCounting( 20, 0 );
	xTaskCreate( initTask, "init", 240, NULL, 4, NULL );
	xTaskCreate( button_task, "button", 240, NULL, 1, NULL );
	xTaskCreate( uart_task, "uart", 240, NULL, 2, NULL );

	vTaskStartScheduler();
	
	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}

