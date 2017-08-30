#include <stdint.h>
#include "chip.h"
#include <board.h>
#include <board_init.h>
#include <cr_section_macros.h>
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

const uint8_t ioexp_init[] = {0x0F};

//const uint8_t lcd_init[] = {0x00, LCD_FSET_NORM, 0x0E, };
const uint8_t lcd_init[] = {LCD_I_WR, LCD_FSET_NORM, 0x0C, 0x06, LCD_FSET_EXT, 0x04, 0x10, 0x42, 0x9f, LCD_FSET_NORM, 0x80, 0x02};
const uint8_t lcd_test[] = {LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE};
const uint8_t lcd_upline[] = {LCD_I_WR, LCD_FSET_NORM, 0x80};
const uint8_t lcd_lowline[] = {LCD_I_WR, LCD_FSET_NORM, 0xC0};

const uint8_t lcd_text[8][17] =
	{
					{LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE, 'a'},
					{LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE, 'b'},
					{LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE, 'c'},
					{LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE, 'd'},
					{LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE, 'e'},
					{LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE, 'f'},
					{LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE, 'g'},
					{LCD_D_WR, LCD_UPCASE('h'), 'e', 'l','l','o',LCD_SPACE,'w','o','r','l','d', LCD_SPACE, LCD_SPACE, LCD_SPACE, LCD_SPACE, 'h'},
	};


int main(void)
{
	uint16_t currentticks = 0;

	board_init();

	__enable_irq();

	// delay a moment
	while(ticks < 4)
		;


	// enable backlight on PCF

	Chip_I2C_MasterSend(I2C0, I2C_PCF8574_1, ioexp_init, sizeof(ioexp_init));
	Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_init, sizeof(lcd_init));

	uint8_t i = 0;
    while(1)
    {

    	if(currentticks != ticks)
    	{
    		currentticks = ticks;
			Chip_GPIO_SetPinToggle(LPC_GPIO, LED_PORT, LED_PIN);

    	}
    	// check if we timed out, turn off backlight
    	// did the rotary get pressed?
    	if(Chip_GPIO_GetPinState(LPC_GPIO, INT_PORT, INT_PIN) == false)
    	{
    		uint8_t pinstate[1];
    		// what changed?
    		Chip_I2C_MasterRead(I2C0, I2C_PCF8574_1, pinstate, sizeof(pinstate));
    		// its the push button
			Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_upline, sizeof(lcd_upline));
			Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_text[i], sizeof(lcd_text[i]));
			Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_lowline, sizeof(lcd_lowline));
			Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_text[i+1], sizeof(lcd_text[i+1]));
			i++;
			if( i > 6 )
				i = 0;

    	}

    }
    return 0 ;
}
