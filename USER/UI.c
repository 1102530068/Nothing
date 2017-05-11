//480*320mm
#include "sys.h" 
#include "lcd.h" 
#include "image2lcd.h"
#include "stdlib.h"
#include "UI.h"

u16 color;
extern const u8 gImage_image1[];//ͼƬ����(������Ϣͷ),�洢��image1.c����.
extern const u8 gImage_imagekaiji[];

void  display_dong(int zv,float cv,float s)
 { 
		u8 S=30,VZ=30,VY=42;
		u32 i=0; 
		u16 R=0,G;
		S=s;
	
/********��ԭʼ���ݴ���ɷ�����״ͼ��ʾ�����ݣ������ƣ�********/

			VZ=272-4*((zv+50)/100);
			VY=272-4*(int)(cv+0.5);
			S=s;
	 
/**********************************************************/			
			R=0;
			for(i=272;i>VZ;i=i-4)		//���ת��
			{	
				
				if(i<=152)         //���Ϸ���ɫ����
				{
					if(i<=32)
					{
						LCD_Fill(60,i,120,i+3,RED);	
					}
					else
					{	
					R=31;
					G=G-1;
					}
				}
				else							//���·���ɫ���� 
				{
					G=31;
					R=R+1;
				}
				color=R*2048+G*64;
				LCD_Fill(60,i,120,i+3,color);	
			}
			LCD_Fill(60,25,120,VZ,LGRAY);
			
			
			R=0;
			for(i=272;i>VY;i=i-4)		//�Ҳ೵��
			{	
				
				if(i<=152)         //���Ϸ���ɫ����
				{
					if(i<=32)
					{
						LCD_Fill(360,i,420,i+3,RED);	
					}
					else
					{	
					R=31;
					G=G-1;
					}	
				}
				else							//���·���ɫ���� 
				{
					G=31;
					R=R+1;
				}
				color=R*2048+G*64;
				LCD_Fill(360,i,420,i+3,color);	
			}
			LCD_Fill(360,25,420,VY,LGRAY);	
			
			LCD_Fill(20,290,S,305,GREEN);			//�·�·����	
			LCD_Fill(S,290,420,305,GRAY);			
			
}	
/****************************************************************************************/ 
void display_jing()															//����ͼƬ�̶ȵȵľ�̬ͼ�����ʾ
{
		u32 y,ZK=6000,SK=60;
		HEADCOLOR *imginfo;
		imginfo=(HEADCOLOR*)gImage_image1;	//�õ��ļ���Ϣ 
		LCD_Init(); 	    //LCD��ʼ��	
		LCD_Display_Dir(1);	//����
		srand(imginfo->h*imginfo->w);
		image_display(190,35,(u8*)gImage_image1);//��ָ����ַ(X,Y)��ʾͼƬ
		LCD_DrawLine(55,22,55,275);
		LCD_DrawLine(425,22,425,275);

		for(y=27;y<=275;y=y+4)						//���ת�ٿ̶�
		{
			if(y%40==35)
			{	
				POINT_COLOR=RED;
				LCD_DrawLine(30,y,55,y);
				if(ZK>0)
				{LCD_ShowxNum( 0,y-8,ZK,4,16,1);}
				if(ZK==0)
				{
					LCD_ShowxNum(0 ,y-8,ZK,4,16,1);
					ZK=7000;
				}	
				ZK=ZK-1000;

				
				
				
			}
			else
			{
				POINT_COLOR=BLACK;
				LCD_DrawLine(40,y,55,y);
			}
			
			if(y%40==15)
			{	
				LCD_DrawLine(35,y,55,y);
			}	
		}
		
		
		
		for(y=27;y<=275;y=y+4)						//�Ҳ೵�ٿ̶�
		{
			if(y%40==35)
			{	
				POINT_COLOR=RED;
				LCD_DrawLine(425,y,450,y);
				if(SK>0)
				{LCD_ShowxNum( 458,y-8,SK,2,16,1);}
				if(SK==0)
				{
					LCD_ShowxNum( 458,y-8,SK,1,16,1);
					SK=70;
				}	
				SK=SK-10;
			}
			else
			{
				POINT_COLOR=BLACK;
				LCD_DrawLine(425,y,440,y);
			}
			
			if(y%40==15)
			{	
				LCD_DrawLine(425,y,445,y);
			}	
		}
   /****************************************************/
		POINT_COLOR=RED;
		LCD_DrawRectangle(185,222,305,260);
		//LCD_DrawRectangle(245,220,350,260);
		
		POINT_COLOR=BLACK;	
		//LCD_ShowString(200,240,420,48,16,"km/h"); 
		LCD_ShowString(265,225,420,48,32,"km");
	
		LCD_ShowString(10,0,150,48,16,"r/min");
		LCD_ShowString(430,0,150,48,24,"km/h");
}
