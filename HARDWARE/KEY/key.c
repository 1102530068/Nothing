#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32F103������V1
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/10
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
//PC2 PA4 PA0    
//������ʼ������
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
	GPIOA->CRL&=0XFFF0FFF0;	//PA0���ó����룬Ĭ������	  
	GPIOA->CRL|=0X00080008; 
	GPIOA->ODR|=1<<4;	
  
	GPIOC->CRL&=0XFFFFF0FF;	//PC2���ó�����	  
	GPIOC->CRL|=0X00000800; 				   
	GPIOC->ODR|=1<<2;	   	//PC2 ����
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ:
//0��û���κΰ�������
//1��KEY0����
//2��KEY1���� 
//3��KEY_UP���� ��WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2; 
		else if(WK_UP==1)return 3;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}




















