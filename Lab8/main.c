#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "queue.h"
#include "semphr.h"
//#include "stdio.h"

void delay(void)
{
	for(int i =0;i<1200;i++)
	for(int j =0;j<1200;j++);

}
xSemaphoreHandle semaphore;
SemaphoreHandle_t mutex;

void GPIOF_Handler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(semaphore,&xHigherPriorityTaskWoken);
	GPIOIntClear(GPIOF_BASE,GPIO_INT_PIN_0);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	
}
void initTask(void * params)
{
	for(;;)
	{
		//PortF Init
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
  GPIOUnlockPin(GPIOF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIOF_BASE,GPIO_PIN_0);
	GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		//Interrupt
	IntMasterEnable();
	GPIOIntEnable(GPIOF_BASE, GPIO_INT_PIN_0); 
  GPIOIntTypeSet(GPIOF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE); 
  GPIOIntRegister(GPIOF_BASE, GPIOF_Handler);
	IntPrioritySet(INT_GPIOF, 0xE0);
	
		//UART
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






void CounterTask(void *params)
{
	char s[] = "This is the CounterTask";
	for(;;)
	{
		xSemaphoreTake(mutex,portMAX_DELAY);
		for(int i=0;i<strlen(s);i++)
		{
			UARTCharPut(UART0_BASE,s[i]);
		}
		UARTCharPut(UART0_BASE,'\n');
		UARTCharPut(UART0_BASE,'\r');
		for(int i=0;i<10;i++)
		{
			UARTCharPut(UART0_BASE,i+'0');
			UARTCharPut(UART0_BASE,'\n');
			UARTCharPut(UART0_BASE,'\r');
			
		}
		UARTCharPut(UART0_BASE,1+'0');
		UARTCharPut(UART0_BASE,0+'0');
		UARTCharPut(UART0_BASE,'\n');
		UARTCharPut(UART0_BASE,'\r');
		delay();
		xSemaphoreGive(mutex);
		
	}
	
}
void  LedTogglerTask(void* params)
{
	xSemaphoreTake(semaphore,0);
	for(;;)
	{
		xSemaphoreTake(semaphore,portMAX_DELAY);
		GPIOPinWrite(GPIOF_BASE,GPIO_PIN_1,GPIOPinRead(GPIOF_BASE,GPIO_PIN_1)?0:GPIO_PIN_1);
	char s[] = "This is the LedTogglerTask ";
		xSemaphoreTake(mutex,portMAX_DELAY);
		for(int i=0;i<strlen(s);i++)
		{
			UARTCharPut(UART0_BASE,s[i]);
		}
		UARTCharPut(UART0_BASE,'\n');
		UARTCharPut(UART0_BASE,'\r');
		vTaskDelay(pdMS_TO_TICKS(3000));
		xSemaphoreGive(mutex);
		
	
}


}



int main()
{
	
	
	
	
	vSemaphoreCreateBinary(semaphore);
	mutex = xSemaphoreCreateMutex();
	
	xTaskCreate( initTask, "init", 240, NULL, 4, NULL );
	xTaskCreate( CounterTask, "init", 240, NULL, 2, NULL );
	xTaskCreate( LedTogglerTask, "init", 240, NULL, 3, NULL );
	

	vTaskStartScheduler();
	
	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
}

