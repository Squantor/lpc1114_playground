/*
 * Board definitions
 */
#ifndef BOARD_H
#define BOARD_H

#include <chip.h>

#define SYSTICKS_PER_S		6

/* pin defines */
#define	LED_PORT			0
#define	LED_PIN				1

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

#endif
