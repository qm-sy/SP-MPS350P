#ifndef __CC6903_H_
#define __CC6903_H_

#include "adc.h"
#include "stdio.h"

#define I_OUT1      10
#define I_OUT2      11
#define I_OUT3      12

uint16_t get_current( uint8_t channel_x );

#endif