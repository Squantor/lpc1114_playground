/*
 * Board init definitions
 */
#ifndef BOARD_INIT_H
#define BOARD_INIT_H

#include <ring_buffer.h>

extern RINGBUFF_T txring, rxring;

void board_init(void);

#endif
