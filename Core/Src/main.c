#include "main.h"

void main( void )
{
    P_SW2 |= 0x80;
    
    GPIO_Init();

    /*  调试使用 printf  */
    Uart3_Init();
    
    /*  温度控制  */
    ADC_Init();
    temp.temp_scan_allow_flag = 1;

    /*  485控制  */
    Uart2_Init();
    Uart2_Send_Statu_Init();
    Timer0_Init();

    /*  4路220输出控制  */
    Power_Statu_Init();
    INT2_Init();
    Timer1_Init();
    Timer4_Init();
    

    /*  PWM控制  */
    PWM_Init();

    EA = 1;

    eeprom_statu_judge();
    A1_DHT11 = 1;
    printf("========== code start ========== \r\n");
    //printf("========== code start ========== \r\n");
    
    while (1)
    {
        Modbus_Event();
        temp_scan();
        //  DHT11_TEST();
        // delay_ms(1000);
        //slave_scan();
    }  
}