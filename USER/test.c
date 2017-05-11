#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "exti.h"
#include "key.h"
#include "rtc.h"
#include "timer.h"
#include "stdio.h"
#include "usmart.h"
#include "UI.h"

/**********************************************************************************/
/*												接口																		                */
/*  测速模块：外部计算中断 PC2车速	PA4转速								 											  */ 
/*	TIM3：500ms计时器																															*/
/*	TIM4：USMART																																	*/
/*	车轮周长：150cm																																*/
/**********************************************************************************/

extern float V;
extern u16 V_RPM;
extern u16 quanshu_tenfold;		//十倍的圈数
  int main(void)
 {
	 u8 t=0;	
	 u16 Time=0,dis_pre=0;
	 float V_ave;	//平均速度
	 char code_ave[5],code_v[5],code_vr[5], mileage[5];//信息储存数组
	 
	delay_init(72);	    	 			//延时函数初始化	  
	Stm32_Clock_Init(9);		  //系统时钟设置
	uart_init(72,9600);	 	 		  //串口初始化为9600
	usmart_dev.init(72);			//初始化USMART	
	EXTIX_Init();							//外部中断初始化
	TIM3_Int_Init(5000,7199); //10Khz的计数频率，计数到5000为500ms 
	LCD_Init();				 				//  PE.ALL,PD1~5  
	LCD_Display_Dir(1);				//设置显示方向为横屏显示
	LED_Init();		  					//初始化与LED连接的硬件接口
	KEY_Init();								//初始化按键

		while(RTC_Init())
		{	//等待RTC初始化
			delay_ms(200);
			printf("rtc waiting```\r\n");
		} 
		printf("finish\n");
		display_jing();
		//初始化完成
   while(1)
	{	
		POINT_COLOR=RED;
		if(t!=calendar.sec)		//时间更新后显示新时间
		{
				t=calendar.sec;	
				LCD_ShowNum(200,135,calendar.min,2,32);		
				LCD_ShowString(230,135,200,32,32,":");			
				LCD_ShowNum(248,135,calendar.sec,2,32);
			
				Time = 60*calendar.min+calendar.sec;//比赛用时
				if(dis_pre != quanshu_tenfold)			//有位移变化
				{
					V_ave=(0.154*quanshu_tenfold)/Time;
					dis_pre = quanshu_tenfold;
					sprintf(code_ave,"%.2f",V_ave);	
					LCD_ShowString(100,200,200,32,32,code_ave);//修改一下位置
					
				}
		}
			
		sprintf(code_v,"%.2f",V);					//float 转 字符串  
		LCD_ShowString(365,275,200,32,32,code_v);		//速度
		
		sprintf(code_vr,"%d",V_RPM);					//float 转 字符串  
		LCD_ShowString(365,275,200,32,32,code_vr);		//转速
 
		sprintf(mileage,"%.2f",0.154*quanshu_tenfold);
		LCD_ShowString(250,225,200,24,32,mileage);	//里程
		
	  display_dong(V_RPM,V,50);
		delay_ms(300);
	}
 }
