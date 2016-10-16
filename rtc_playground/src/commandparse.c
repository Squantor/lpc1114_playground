#include <stdint.h>
#include <commandparse.h>
#include <ringbuffers.h>
#include <board.h>
#include <strings.h>
#include <print.h>
#include <DS3231.h>

void commandread(char *s, int size)
{
	switch(s[1])
	{
	case 'r':	// read out rtc date and time, format YYMMDD HHMMSS
		{
			uint8_t rtc_data[6];
			// read out first register
			Chip_I2C_MasterCmdRead(I2C0, DS3231_SLAVE_ADDR, 0x00, rtc_data, sizeof(rtc_data));
			print_hex_u8(rtc_data[6]);
			print_hex_u8(rtc_data[5] & 0x7F);
			print_hex_u8(rtc_data[4]);
			Chip_UART_SendRB(LPC_USART, &txring, str_space, sizeof(str_space));
			print_hex_u8(rtc_data[2]);
			print_hex_u8(rtc_data[1]);
			print_hex_u8(rtc_data[0]);
		}
		break;
	case 't':
		// read out temperature register
		{
			uint8_t rtc_data[2];
			Chip_I2C_MasterCmdRead(I2C0, DS3231_SLAVE_ADDR, DS3231_TEMP, rtc_data, sizeof(rtc_data));
			print_dec_u16(rtc_data[0]);
			Chip_UART_SendRB(LPC_USART, &txring, str_dot, sizeof(str_dot));
			print_dec_u16((rtc_data[1] >> 6) * 25);

		}
		break;
	case 'c':
		// read out calibration register
		break;
	}
}

void commandwrite(char *s, int size)
{

}

void commandStart(char *s, int size)
{
	switch(s[1])
	{
		case 't':
			{
				uint8_t rtc_startconv[] = {DS3231_CTRL, 0x20};
				Chip_I2C_MasterSend(I2C0, DS3231_SLAVE_ADDR, rtc_startconv, sizeof(rtc_startconv));
				Chip_UART_SendRB(LPC_USART, &txring, str_okay, sizeof(str_okay));
			}
		break;
	}
}

void commandparse(char *s, int size)
{
	if(size > 0)
	{
		Chip_UART_SendRB(LPC_USART, &txring, str_crlf, sizeof(str_crlf));
		switch(s[0])
		{
		case 'r':
			commandread(s, size);
		break;
		case 'w':
			commandwrite(s, size);
		break;
		case 's':
			commandStart(s, size);
			break;
		}
		Chip_UART_SendRB(LPC_USART, &txring, str_crlf, sizeof(str_crlf));
	}
}
