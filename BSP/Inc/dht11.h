#ifndef __DHT11_H_
#define __DHT11_H_

#include "delay.h"
#include "sys.h"
#include "gpio.h"
#include "communication.h"
typedef struct 
{
    uint8_t u8T_data_H_temp;
    uint8_t u8T_data_L_temp;
    uint8_t u8RH_data_H_temp;
    uint8_t uRH_data_L_temp;
    uint8_t u8checkdata_temp;
}DHT11;

typedef struct 
{
    uint8_t  tempH;  //温度高字节（十位）
    uint8_t  tempL;  //温度低位节（个位）
    uint8_t  humyH;  //湿度高字节（十位）
    uint8_t  humyL;  //湿度高字节（个位）

}DHT11_VALUE;

extern DHT11 dht11;
extern DHT11_VALUE dht11_value;

void Read_DHT11(void);   //温湿传感启动
void delay_10us(uint8_t x);
void DHT11_TEST(void);

#endif