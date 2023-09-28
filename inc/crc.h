/*
 * crc.h
 *
 *  Created on: 09 nov 2016
 *      Author: MattiaMobile
 */

#ifndef INC_CRC_H_
#define INC_CRC_H_

#include "flash_if.h"

#define CRC16_START_VAL  0x1D0F /* start value which is used by SRecord tool for -CRC16_Big_Endian */

#define FAILED	0
#define PASSED	1

#define CRC_RANGE_START_ADDR    APPLICATION_ADDRESS /* start addr */
#define CRC_RANGE_END_ADDR      (USER_FLASH_END_ADDRESS-2) /* end addr, this one will not be counted */
#define CRC_VALUE_ADDR          (USER_FLASH_END_ADDRESS-2) /* address of CRC (16bits) */

unsigned short crc16_ccitt(const void *buf, int len, unsigned short start);
unsigned char CheckCRC(void);


#endif /* INC_CRC_H_ */
