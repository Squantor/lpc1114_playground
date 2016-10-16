/*
 * Board definitions
 */
#ifndef DS3231_H
#define DS3231_H

// For more information, check out the DS3231 datasheet

#define	DS3231_SEC			0x00	// seconds
#define DS3231_MIN			0x01	// minutes
#define	DS3231_CTRL			0x0E	// control register
#define	DS3231_CTRLSTATE	0x0F	// control and status
#define	DS3231_AGING		0x10	// aging
#define	DS3231_TEMP			0x11	// temperature
#define	DS3231_TEMP_SIZE	2

#endif
