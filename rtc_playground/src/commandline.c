#include <stdint.h>
#include <ringbuffers.h>
#include <commandline.h>


void commandline(void)
{
	char c;
	// check for a char, add to buf
	int char_count = Chip_UART_ReadRB(LPC_USART, &rxring, &c, sizeof(c));
	if(char_count == 1)
	{
		Chip_UART_SendRB(LPC_USART, &txring, &c, sizeof(c));
	}
}
