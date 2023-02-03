/*******************************************
���ƣ�51����С��
���ߣ�������
ʱ�䣺2020��11��18��21:36:47
����������Ŀ��Ƶ��https://www.bilibili.com/video/BV1Wy4y1z7a5
csdn���ͽ̳̣�https://blog.csdn.net/mbs520/article/details/109775964
����:2809786963@qq.com
*******************************************/

#include "reg52.h"


#define uchar unsigned char
#define uint unsigned char


/***С�����ƶ���**/
#define DIR P1
#define QIAN 0xaa 
#define HOU 0x55 
#define ZUO 0x5a 
#define YOU 0xa5 
#define STOP 0x00 


void uart_init();
void uart_tx_string(uchar *str);
void uart_tx_byte(uchar str);
void Delayms(unsigned int n);
void Delayus(unsigned int n);
uchar rec;

/*********************************************************
��������������
*********************************************************/
void main()
{
				int v=4;
        uart_init();
				DIR = STOP;
        Delayms(1);
        uart_tx_string("hello buletooch car!\n");
        while(1)
        {
					rec  = 1;
                switch(rec)
								{
									case 0:v++; rec=10;    																	break;
									case 1:DIR = QIAN;Delayus(3);DIR = STOP;Delayus(10); 	break;
									case 2:DIR = HOU;	Delayus(v);DIR = STOP;Delayus(100);		break;
									case 3:DIR = ZUO;	Delayus(v);DIR = STOP;Delayus(40);		break;
									case 4:DIR = YOU;	Delayus(v);DIR = STOP;Delayus(40);		break;
									case 5:DIR = STOP;Delayus(v);DIR = STOP;Delayus(100);		break;
								}
        }
}

/*********************************************************
�������������ж�
*********************************************************/
void uart_timer() interrupt 4
{
        if(RI)
        {
                RI=0;
                rec=SBUF;
                uart_tx_byte('&');
                Delayms(2);
                uart_tx_byte(rec);
                Delayms(2);
                uart_tx_byte('&');
        }
}

/*********************************************************
�����������ڳ�ʼ��
�����ʣ�9600
����11.059M
*********************************************************/
void uart_init()
{
				TMOD=TMOD&0x0F;
				TMOD=0x20;
				TH1=0xFD;
				TL1=0xFD;
				TR1=1;
				SCON=SCON&0x0F;
				SCON=0x50;
				EA=1;
				ES=1;
}
 
/*********************************************************
�����������ڷ���һ���ֽ�
*********************************************************/
void uart_tx_byte(uchar str)
{
        SBUF=str;
        while(!TI);
}

/*********************************************************
�����������ڷ���һ���ַ���
*********************************************************/
void uart_tx_string(uchar *str)
{
        while(*str!='\0')
        {
                uart_tx_byte(*str++);
                Delayms(2);
        }
}

/*********************************************************
����������ʱ����
*********************************************************/
void Delayms(unsigned int n)
{
        unsigned int i,j;
        for(j=n;j>0;j--)
                for(i=112;i>0;i--);
}
void Delayus(unsigned int n)
{
				while(n--);
}




