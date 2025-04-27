#include  "dht11.h" 

DHT11 dht11;

void DHT11_start()
{
    Temp_Hum = 1;
    delay_us(2);

    Temp_Hum = 0;
    delay_ms(25);   //拉低延时18ms以上

    Temp_Hum = 1;
    delay_us(30);   //拉高 延时 20~40us，取中间值 30us
}

 /**************************************************************************************
 * 描  述 : 从DHT11读取一个字节，MSB先行
 * 入  参 : 无
 * 返回值 : uint8_t
 **************************************************************************************/
static uint8_t Read_Byte(void)  
{       
	uint8_t i, dat = 0;

	for( i = 0; i < 8; i++ )    
	{	   
		while(!Temp_Hum); //每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束

		delay_us(40); //延时约40us,这个延时需要大于数据0持续的时间即可	   	  

        if(Temp_Hum)   //约40us后仍为高电平表示数据“1”
		{
			while(Temp_Hum);  //等待数据1的高电平结束
			dat|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 //40us后为低电平表示数据“0”
		{			   
			dat&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return dat;	
}

 /**************************************************************************************
 * 描  述 : 一次完整的数据传输为40bit，高位先出
 * 入  参 : 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
 * 返回值 : 无
 **************************************************************************************/
void Read_DHT11(void)   //温湿传感启动
{
    uint8_t R_H,R_L,T_H,T_L,revise; 

    DHT11_start();
	
    if(!Temp_Hum)      //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行   
    {
        while(!Temp_Hum);   //轮询直到从机发出的83us 低电平 响应信号结束
        while(Temp_Hum);    //轮询直到从机发出的87us 高电平 标置信号结束
			
			  /*开始接收数据*/ 
        R_H= Read_Byte();
        R_L= Read_Byte();
        T_H= Read_Byte();
        T_L= Read_Byte();

        revise = Read_Byte();

        Temp_Hum =1 ;                   //读取结束，主机拉高

        if((R_H+R_L+T_H+T_L)==revise)      //最后一字节为校验位，校验是否正确
        {
            dht11.dht11_temp  = T_H;
            dht11.dht11_humidity = R_H;
        }
    }
}
