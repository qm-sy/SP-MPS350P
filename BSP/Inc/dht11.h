#ifndef __DHT11_H_
#define __DHT11_H_

#include "delay.h"
#include "sys.h"
#include "gpio.h"
#include "communication.h"
#include "ntc.h"

typedef struct 
{
    uint8_t  dht11_temp;
    uint8_t  dht11_humidity;
}DHT11;

extern DHT11 dht11;

void DHT11_start();
void Read_DHT11(void);   //温湿传感启动

#endif