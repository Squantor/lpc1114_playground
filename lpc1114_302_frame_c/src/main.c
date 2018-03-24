#include <stdint.h>
#include "chip.h"
#include <board.h>
#include <board_init.h>
#include <print.h>

const char str_ready[] = "ready!\r\n";
const char str_crlf[] = "\r\n";
const char str_space[] = " ";
const char str_separator[] = ";";

volatile static uint16_t ticks = 0;

void SysTick_Handler(void)
{
	ticks++;
}

void UART_IRQHandler(void)
{
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}

void I2C_IRQHandler(void)
{
	Chip_I2C_MasterStateHandler(I2C0);
}

int main(void)
{
	uint16_t currentticks = 0;

	board_init();

	__enable_irq();

	Chip_UART_SendRB(LPC_USART, &txring, str_ready, sizeof(str_ready) - 1);

    while(1)
    {
    	if(currentticks != ticks)
    	{
    		currentticks = ticks;
			Chip_GPIO_SetPinToggle(LPC_GPIO, LED_PORT, LED_PIN);
			print_dec_u16(currentticks);
			Chip_UART_SendRB(LPC_USART, &txring, str_crlf, sizeof(str_crlf) - 1);
    	}
    	;
    }
    return 0 ;
}
