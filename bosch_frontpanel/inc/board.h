/*
 * Board definitions
 */
#ifndef BOARD_H
#define BOARD_H

#include <chip.h>

#define SYSTICKS_PER_S		6

/* pin defines */
#define	LED_PORT			0
#define	LED_PIN				7

/* uart settings*/
// Transmit and receive ring buffer sizes
#define UART_SRB_SIZE 	128	/* Send */
#define UART_RRB_SIZE 	32	/* Receive */
// uart speeds
#define UART_SPEED		115200

/* i2c settings */
#define	I2C_SPEED		100000

/* SPI settings*/
#define	SSP_SPEED		50000
#define	SSP_DATA_BITS	SSP_BITS_8

// PCF8574 address
#define I2C_PCF8574_1	0x27
#define	ROT_DIR			(1 << 0)
#define	ROT_PULSE		(1 << 1)
#define	ROT_SW			(1 << 2)
#define	BL_ON			(1 << 5)
#define	BL_DIM			(1 << 6)

#define INT_PORT		0
#define	INT_PIN			3

// PCF2119 controller
#define	LCD_ADDR		0x3B


#endif
