/*
|P0^0|          |P1^0|RX2       |P2^0|          |P3^0|RX1       |P4^0|          |P5^0|     
|P0^1|          |P1^1|TX2       |P2^1|DC_24V    |P3^1|TX1       |P4^1|AC_Out1   |P5^1|     
|P0^2|I_Out1    |P1^2|          |P2^2|PWM7      |P3^2|zero_flag |P4^2|AC_Out2   |P5^2|     
|P0^3|I_Out2    |P1^3|          |P2^3|PWM8      |P3^3|          |P4^3|AC_Out4   |P5^3|     
|P0^4|I_Out3    |P1^4|NTC1      |P2^4|          |P3^4|          |P4^4|AC_Out3   |P5^4|
|P0^5|DHT11     |P1^5|NTC2      |P2^5|          |P3^5|          |P4^5|          |P5^5|             
|P0^6|A2        |P1^6|NTC3      |P2^6|          |P3^6|          |P4^6|          |P5^6|     
|P0^7|          |P1^7|NTC4      |P2^7|          |P3^7|          |P4^7|DR2       |P5^7|     
*/ 
#include "gpio.h"

/**
 * @brief	GPIO初始化
 *
 * @param   void
 *
 * @return  void
 */
void GPIO_Init( void )
{
    P0M0 = 0x02;    P0M1 = 0x1c;        //P0.1为推挽输出，P0.2、P0.3、P0.4为高阻输入
    P1M0 = 0x02;    P1M1 = 0xf0;        //1.1为推挽输出,P1.4、P1.5、P1.6、P1.7为高阻输入
    P2M0 = 0x0e;    P2M1 = 0x00;        //P2.1为推挽输出
    P3M0 = 0X00;    P3M1 = 0X00;   
    P4M0 = 0x9e;    P4M1 = 0x00;        //P4.7为推挽输出       
    P5M0 = 0X04;    P5M1 = 0X00;    
}