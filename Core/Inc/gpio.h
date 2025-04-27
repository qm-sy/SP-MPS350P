#ifndef _GPIO_H_
#define _GPIO_H_

#include "sys.h"

sbit I_Out1    = P0^2;
sbit I_Out2    = P0^3;
sbit I_Out3    = P0^4;
sbit Temp_Hum  = P0^5;
sbit A2        = P0^6;

sbit RX2       = P1^0;
sbit TX2       = P1^1;
sbit NTC1      = P1^4;
sbit NTC2      = P1^5;
sbit NTC3      = P1^6;
sbit NTC4      = P1^7;

sbit DC_24V    = P2^1;
sbit PWM7      = P2^2;
sbit PWM8      = P2^3;

sbit RX1       = P3^0;
sbit TX1       = P3^1;
sbit zero_flag = P3^6;

sbit AC_Out1   = P4^1;
sbit AC_Out2   = P4^2;
sbit AC_Out3   = P4^3;
sbit AC_Out4   = P4^4;
sbit DR2       = P4^7;

void GPIO_Init( void );

#endif