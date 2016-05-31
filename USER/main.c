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
/*	mpu��sda����PC11  scl����PC12				3.3V����																	*/																		
/*	��������		Echo����PA0  Trig����PA1		5V����																		*/
/*  ����ģ�飺�ⲿ�����ж� PC5					5V����   																		*/ 
/*	TIM3��1s��ʱ��																																	*/
/*	TIM2��������																																		*/	
/*	TIM4��USMART																																	*/
/**********************************************************************************/

extern u32	TIME;	//���벶��ֵ	
extern float v;
extern unsigned int quanshu_tenfold;
 int main(void)
 {
		u8 t,twice=0,i;	 
		float temp;	 //����ֵ
		char dis[4];
	  char code_v[5];
	  char angle[4];
	  float pitch,roll,yaw; 		//ŷ����
	 char mileage[5];
//		short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
//		short gyrox,gyroy,gyroz;	//������ԭʼ����
	//short temp;					//�¶�
	 
	delay_init();	    	 			//��ʱ������ʼ��	  
	NVIC_Configuration(); 	  //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Stm32_Clock_Init(9);		  //ϵͳʱ������
	uart_init(9600);	 	 		  //���ڳ�ʼ��Ϊ9600
	usmart_dev.init(72);			//��ʼ��USMART	
	EXTI_Init();							//�ⲿ�жϳ�ʼ��
	TIM3_Int_Init(5000,7199); //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	MPU_Init();								//��ʼ��MPU6050
	LCD_Init();				 				//PE.ALL,PD1~5
	LCD_Display_Dir(1);				//������ʾ����Ϊ������ʾ
 	ultrasonic_Init();				//��1Mhz��Ƶ�ʼ��� 
	LED_Init();		  					//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();								//��ʼ������

	 		while(mpu_dmp_init())
		{	
			delay_ms(20);
			twice++;
			if(twice==10)break;
			printf("mpu waiting```\r\n");//mpu6050��ʼ���ȴ�
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
				printf("���1��%.2f \r\n",pitch);
				printf("���2��%.2f \r\n",roll);
				printf("���3��%.2f \r\n",yaw);
				
				break;
			}
			else printf("%d",mpu_dmp_get_data(&pitch,&roll,&yaw));
		}
		if(t!=calendar.sec)		//ʱ����º���ʾ��ʱ��
		{
				t=calendar.sec;	
				LCD_ShowNum(280,300,calendar.min,2,32);		
				LCD_ShowString(320,300,200,32,32,":");			
				LCD_ShowNum(338,300,calendar.sec,2,32);
				LED0=!LED0;
		//	printf("Time: %d :%d\r\n",calendar.min,calendar.sec);
		}
			
		//printf("v=%.2f km/h\r\n",v);
		sprintf(code_v,"%.2f",v);					//float ת �ַ���
		LCD_ShowString(50,250,200,32,32,"Speed:");
		LCD_ShowString(160,250,200,32,32,code_v);
		LCD_ShowString(250,250,200,32,32,"km/h");
		
		temp=MiddleValueFilter(); //��ֵ�˲�
		//printf("distance=%f \n",temp);	
		
		sprintf(dis,"%.2f",temp);//float ת �ַ���
		LCD_ShowString(50,200,200,24,32,"Distence:");
		LCD_ShowString(200,200,200,24,32,dis);
		
		sprintf(angle,"%.2f",roll);
		LCD_ShowString(50,350,200,24,32,"Angle:");
		LCD_ShowString(150,350,200,24,32,angle);  //�Ƕ�
		
		sprintf(mileage,"%.2f",0.154*quanshu_tenfold);
		LCD_ShowString(50,400,200,24,32,"mileage:");
		LCD_ShowString(200,400,200,24,32,mileage);	//���
		LCD_ShowString(270,400,200,24,32,"Km");
		delay_ms(100);
	}
 }

