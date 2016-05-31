#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "ultrasonic.h"	   
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "stdio.h"
#include "rtc.h"
#include "timer.h"

/***********************************************************************************/
/*	mpu：sda——PC11  scl——PC12				3.3V供电																	*/																		
/*	超声波：		Echo——PA0  Trig——PA1		5V供电																		*/
/*  测速模块：外部计算中断 PC5					5V供电   																		*/ 
/*	TIM3：1s计时器																																	*/
/*	TIM2：超声波																																		*/	
/*	TIM4：USMART																																	*/
/**********************************************************************************/

extern u32	TIME;	//输入捕获值	
extern float v;
extern unsigned int quanshu_tenfold;
 int main(void)
 {
		u8 t,twice=0,i;	 
		float temp;	 //测量值
		char dis[4];
	  char code_v[5];
	  char angle[4];
	  float pitch,roll,yaw; 		//欧拉角
	 char mileage[5];
//		short aacx,aacy,aacz;		//加速度传感器原始数据
//		short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	//short temp;					//温度
	 
	delay_init();	    	 			//延时函数初始化	  
	NVIC_Configuration(); 	  //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Stm32_Clock_Init(9);		  //系统时钟设置
	uart_init(9600);	 	 		  //串口初始化为9600
	usmart_dev.init(72);			//初始化USMART	
	EXTI_Init();							//外部中断初始化
	TIM3_Int_Init(5000,7199); //10Khz的计数频率，计数到5000为500ms 
	MPU_Init();								//初始化MPU6050
	LCD_Init();				 				//PE.ALL,PD1~5
	LCD_Display_Dir(1);				//设置显示方向为横屏显示
 	ultrasonic_Init();				//以1Mhz的频率计数 
	LED_Init();		  					//初始化与LED连接的硬件接口
	KEY_Init();								//初始化按键

	 		while(mpu_dmp_init())
		{	
			delay_ms(20);
			twice++;
			if(twice==10)break;
			printf("mpu waiting```\r\n");//mpu6050初始化等待
		} 
		while(RTC_Init());
		{	
			delay_ms(200);
			printf("rtc waiting```\r\n");
		} 
		printf("finish\n");
   	while(1)
	{	
		for(i=0;i<6;i++)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{ 
				printf("倾角1：%.2f \r\n",pitch);
				printf("倾角2：%.2f \r\n",roll);
				printf("倾角3：%.2f \r\n",yaw);
				
				break;
			}
			else printf("%d",mpu_dmp_get_data(&pitch,&roll,&yaw));
		}
		if(t!=calendar.sec)		//时间更新后显示新时间
		{
				t=calendar.sec;	
				LCD_ShowNum(280,300,calendar.min,2,32);		
				LCD_ShowString(320,300,200,32,32,":");			
				LCD_ShowNum(338,300,calendar.sec,2,32);
				LED0=!LED0;
		//	printf("Time: %d :%d\r\n",calendar.min,calendar.sec);
		}
			
		//printf("v=%.2f km/h\r\n",v);
		sprintf(code_v,"%.2f",v);					//float 转 字符串
		LCD_ShowString(50,250,200,32,32,"Speed:");
		LCD_ShowString(160,250,200,32,32,code_v);
		LCD_ShowString(250,250,200,32,32,"km/h");
		
		temp=MiddleValueFilter(); //中值滤波
		//printf("distance=%f \n",temp);	
		
		sprintf(dis,"%.2f",temp);//float 转 字符串
		LCD_ShowString(50,200,200,24,32,"Distence:");
		LCD_ShowString(200,200,200,24,32,dis);
		
		sprintf(angle,"%.2f",roll);
		LCD_ShowString(50,350,200,24,32,"Angle:");
		LCD_ShowString(150,350,200,24,32,angle);  //角度
		
		sprintf(mileage,"%.2f",0.154*quanshu_tenfold);
		LCD_ShowString(50,400,200,24,32,"mileage:");
		LCD_ShowString(200,400,200,24,32,mileage);	//里程
		LCD_ShowString(270,400,200,24,32,"Km");
		delay_ms(100);
	}
 }

