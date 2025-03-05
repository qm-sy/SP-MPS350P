#include  "dht11.h" 

DHT11 dht11;
DHT11_VALUE dht11_value;

/**************************************************************************************
 * 描  述 : 延时函数
 * 入  参 : 10us的整数倍
 * 返回值 : 无
 **************************************************************************************/
void delay_10us(uint8_t x)
{
  uint8_t i;

  for (i = 0; i < x; i++) 
	{
        delay_us(10);
    }               
}

 /**************************************************************************************
 * 描  述 : 从DHT11读取一个字节，MSB先行
 * 入  参 : 无
 * 返回值 : uint8_t
 **************************************************************************************/
static uint8_t Read_Byte(void)  
{       
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	   
		while(!A1_DHT11);  //每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束

		/*DHT11 以23~27us的高电平表示“0”，以68~74us的高电平表示“1”  */
		delay_10us(4); //延时约40us,这个延时需要大于数据0持续的时间即可	   	  

		if(A1_DHT11)   //约40us后仍为高电平表示数据“1”
		{
			while(A1_DHT11);  //等待数据1的高电平结束
			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 //40us后为低电平表示数据“0”
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return temp;	
}

 /**************************************************************************************
 * 描  述 : 一次完整的数据传输为40bit，高位先出
 * 入  参 : 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
 * 返回值 : 无
 **************************************************************************************/
void Read_DHT11(void)   //温湿传感启动
{
    uint8_t u8T_data_H,u8T_data_L,u8RH_data_H,u8RH_data_L,u8checkdata,uchartemp;

    A1_DHT11=0;         //主机拉低
    delay_ms(20);       //延时20ms，需大于18ms
    A1_DHT11=1;         //总线拉高 主机延时30us
    delay_10us(3);      //延时约30us
	
    if(!A1_DHT11)       //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行   
    {
			  while(!A1_DHT11);   //轮询直到从机发出的83us 低电平 响应信号结束
			  while(A1_DHT11);    //轮询直到从机发出的87us 高电平 标置信号结束
			
			  /*开始接收数据*/ 
        dht11.u8RH_data_H_temp= Read_Byte();
        dht11.uRH_data_L_temp= Read_Byte();
        dht11.u8T_data_H_temp= Read_Byte();
        dht11.u8T_data_L_temp= Read_Byte();
        dht11.u8checkdata_temp= Read_Byte();
			
        A1_DHT11=1;                   //读取结束，主机拉高
			
			  /*检查读取的数据是否正确*/
        uchartemp=(dht11.u8T_data_H_temp+dht11.u8T_data_L_temp+dht11.u8RH_data_H_temp+dht11.uRH_data_L_temp);
        if(uchartemp==dht11.u8checkdata_temp)   //校验
        {
            u8RH_data_H=dht11.u8RH_data_H_temp;
            u8RH_data_L=dht11.uRH_data_L_temp;
            u8T_data_H=dht11.u8T_data_H_temp;
            u8T_data_L=dht11.u8T_data_L_temp;
            u8checkdata=dht11.u8checkdata_temp;
        }				
        dht11_value.tempH = u8T_data_H/10; 
        dht11_value.tempL = u8T_data_H%10;
        
        dht11_value.humyH = u8RH_data_H/10; 
        dht11_value.humyL = u8RH_data_H%10; 					
    } 
    else      //没能成功读取，返回0
    {
        dht11_value.tempH = 0; 
        dht11_value.tempL = 0;        
        dht11_value.humyH = 0; 
        dht11_value.humyL = 0;  	
    }  
}

/**************************************************************************************
 * 描  述 : 串口实时显示温湿度
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void DHT11_TEST(void)
{
    static uint8_t temp[3]; 
    static uint8_t humidity[3];   
    uint16_t tmep_value;
    uint16_t humidity_value;

    memset(temp, 0, 3);                             //将temp数组初始化（清零）
    memset(humidity, 0, 3);                         //将humidity数组初始化（清零）
		
	  Read_DHT11();                                   //获取温湿度值
		
	  //将温湿度值转换为字符串
    temp[0] = dht11_value.tempH + 0x30;
    temp[1] = dht11_value.tempL + 0x30;
    tmep_value = temp[0]<<8 | temp[1];
    humidity[0] = dht11_value.humyH + 0x30;
    humidity[1] = dht11_value.humyL + 0x30;
    humidity_value = humidity[0]<<8 | humidity[1];
		
		//串口打印温湿度值
    printf("Temperature: 0x%04x ,Humidity is 0x%04x \r\n",tmep_value,humidity_value);
}


/*********************************END FILE*************************************/