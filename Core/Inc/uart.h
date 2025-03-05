#ifndef _UART_H_
#define _UART_H_

#include "sys.h"

#define S2TI    0X02
#define S2RI    0X01

#define S3TI    0X02
#define S3RI    0X01

void Uart2_Init( void );
void Uart3_Init( void );

#endif