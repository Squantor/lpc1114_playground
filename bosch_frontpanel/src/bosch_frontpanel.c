#include <stdint.h>
#include "chip.h"
#include <board.h>
#include <board_init.h>
#include <cr_section_macros.h>
#include <print.h>
#include <lyrics.h>
#include <PCF2119.h>

const char str_ready[] = "ready!\r\n";
const char str_crlf[] = "\r\n";
const char str_space[] = " ";
const char str_separator[] = ";";

#define	SCREENOFF_TIMEOUT	120

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
const uint8_t ioexp_bl_off[] = {0x6F};

//const uint8_t lcd_init[] = {0x00, LCD_FSET_NORM, 0x0E, };
const uint8_t lcd_init[] = {LCD_I_WR, LCD_FSET_NORM, 0x0C, 0x06, LCD_FSET_EXT, 0x04, 0x10, 0x42, 0x9f, LCD_FSET_NORM, 0x80, 0x02};
const uint8_t lcd_upline[] = {LCD_I_WR, LCD_FSET_NORM, 0x80};
const uint8_t lcd_lowline[] = {LCD_I_WR, LCD_FSET_NORM, 0xC0};
const uint8_t lcd_clear[17] = {LCD_D_WR
		,'t','e','s','t'
		,LCD_SPACE,LCD_SPACE,LCD_SPACE,LCD_SPACE
		,LCD_SPACE,LCD_SPACE,LCD_SPACE,LCD_SPACE
		,LCD_SPACE,LCD_SPACE,LCD_SPACE,LCD_SPACE};

int main(void)
{
	uint16_t currentticks = 0;

	board_init();

	__enable_irq();

	// enable backlight on PCF
	// delay a moment
	while(ticks < 2)
		;

	Chip_I2C_MasterSend(I2C0, I2C_PCF8574_1, ioexp_init, sizeof(ioexp_init));
	Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_init, sizeof(lcd_init));
	// delay a moment
	while(ticks < 3)
		;
	Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_upline, sizeof(lcd_upline));
	Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_clear, sizeof(lcd_clear));
	Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_lowline, sizeof(lcd_lowline));
	Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_clear, sizeof(lcd_clear));

	// dummy read
	uint8_t pinstate_prev;
	Chip_I2C_MasterRead(I2C0, I2C_PCF8574_1, &pinstate_prev, sizeof(pinstate_prev));

	uint8_t text_index = 0;
	uint8_t screenoff = 0;
    while(1)
    {

    	if(currentticks != ticks)
    	{
    		currentticks = ticks;
			Chip_GPIO_SetPinToggle(LPC_GPIO, LED_PORT, LED_PIN);
			if(screenoff > 0)
			{
				screenoff--;
			}
			else
			{
				text_index = 0;
				Chip_I2C_MasterSend(I2C0, I2C_PCF8574_1, ioexp_bl_off, sizeof(ioexp_bl_off));
			}
    	}
    	// check if we timed out, turn off backlight
    	// did the rotary get pressed?
    	if(Chip_GPIO_GetPinState(LPC_GPIO, INT_PORT, INT_PIN) == false)
    	{
    		uint8_t pinstate[1];
    		// what changed?
    		Chip_I2C_MasterRead(I2C0, I2C_PCF8574_1, pinstate, sizeof(pinstate));
    		// what happened? (only check rotary)
    		if((pinstate[0] & 3) != (pinstate_prev & 3))
    		{
    			// did we get pulsed?
    			if((pinstate[0] & ROT_PULSE) != (pinstate_prev & ROT_PULSE))
    			{
    				// where we off?
    				if(screenoff != 0)
    				{
						// what rotate direction?
						if((pinstate[0] & ROT_DIR) == 0)
						{
							if( text_index == 0 )
							{
								text_index = LYRICS_SIZE-2;
							}
							else
							{
								text_index--;
							}
						}
						else
						{
							if( text_index > LYRICS_SIZE-3 )
							{
								text_index = 0;
							}
							else
							{
								text_index++;
							}
						}
    				}
    				else
    				{
    					text_index = 0;
    				}
        			// rotary turned, lets update our text
        			Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_upline, sizeof(lcd_upline));
        			Chip_I2C_MasterSend(I2C0, LCD_ADDR, lyrics[text_index], sizeof(lyrics[text_index]));
        			Chip_I2C_MasterSend(I2C0, LCD_ADDR, lcd_lowline, sizeof(lcd_lowline));
        			Chip_I2C_MasterSend(I2C0, LCD_ADDR, lyrics[text_index+1], sizeof(lyrics[text_index+1]));
    			}
    			pinstate_prev = pinstate[0];
    			// reset screen off timer
    			screenoff = SCREENOFF_TIMEOUT;
    			Chip_I2C_MasterSend(I2C0, I2C_PCF8574_1, ioexp_init, sizeof(ioexp_init));
    		}
    	}

    }
    return 0 ;
}
