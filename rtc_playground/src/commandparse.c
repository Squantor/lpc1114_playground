#include <stdint.h>
#include <commandparse.h>
#include <board.h>

void commandparse(char *s, int size)
{
	Chip_UART_SendRB(LPC_USART, &txring, &s, size);
}
