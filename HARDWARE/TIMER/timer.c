#include "timer.h"
#include "led.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
float V=0;
u16 V_RPM=0;
 u16 signal_sum=0;
 u16 quanshu_tenfold=0;		//Ȧ����ʮ��
 extern u16 signal;
 extern u16 signal_RPM;
 
//��ʱ��3�жϷ������	 0.5s��һ��  ���㳵�ٺ�ת��
void TIM3_IRQHandler(void)
{ 		 
	if(TIM3->SR&0X0001)//����ж�
	{		
		V=signal*0.9;					//�ٶ�ת�� Km/h   ��ʵ�����һ�� ��Ҫ/2
		V_RPM=signal_RPM*15;	//ת��ת�� r/min		��Ҫ*2   //W T F
		signal_sum+=signal;
		if(signal_sum>=120)			//ɲ����һȦ12���ź�
		{
			quanshu_tenfold++;
			signal_sum %= 120;	//���²���120�Ĳ���
		}
		printf("%d    V=%f    V_RPM=%d \n",signal,V,V_RPM);
		signal=0;
		signal_RPM=0;
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM3->PSC=psc;  	//Ԥ��Ƶ������
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2									 
}


