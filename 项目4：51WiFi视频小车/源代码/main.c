/*******************************************
���ƣ�51WIFI��ƵС��
���ߣ�������
ʱ�䣺2020��11��20��11��27��
����������Ŀ��Ƶ��https://www.bilibili.com/video/BV1gf4y1v7qa
csdn���ͽ̳̣�https://blog.csdn.net/mbs520/article/details/109843972
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

uchar rec;//���յ����ֽ�
uchar buff[9]="00000000";//���յ������ݰ�
uchar flag;//���ݰ���ʼ���ձ�־
uchar num; //�����±�
uchar buf_ready;//���յ����ݰ���1

/*********************************************************
��������������
*********************************************************/
void main()
{
        uart_init();
				DIR = STOP;
        Delayms(1);
				buff[3] = 0x80;
				buff[4] = 0x80;
        uart_tx_string("hello buletooch car!\n");
        while(1)
        {
								if(buf_ready == 1)//����һ��������ɱ�־
								{
											buf_ready = 0;
								}
								if(buff[3]>0xb0){DIR = QIAN;Delayus(buff[3]-0xa0);DIR = STOP;Delayus(0x20);}//buff[3]����ǰ��
								else if(buff[3]<0x40){DIR = HOU;Delayus(0x60-buff[3]);DIR = STOP;Delayus(0x20);}//ͨ����ʱ�����ٶ�
								
								else if(buff[4]>0xd0){DIR = YOU;Delayus(20);DIR = STOP;Delayus(5);}//buff[4]��������
								else if(buff[4]<0x20){DIR = ZUO;Delayus(20);DIR = STOP;Delayus(5);}
								else DIR = STOP;
        }
}

/*********************************************************
�������������ж�
*********************************************************/
void uart_timer() interrupt 4
{
        if(RI)
        {
								RI = 0;
                rec=SBUF;
								
								if(rec==0x66 && flag==0)//����ͷ
								{
											flag = 1;  
											num=0;
											buff[0] = rec;
								}
								else if(flag == 1)//��ʼ�������ݰ�buff[8]
								{
									
											num++;
											buff[num] = rec;
											if(num==7 && buff[7]==0x99)//���յ�����β
											{
													buf_ready = 1;
													flag = 0;
													num = 0;
											}
											else if(num == 7)//���մ���
											{
													flag = 0;
													num = 0;
											}
								}
        }
}

/*********************************************************
�����������ڳ�ʼ��
�����ʣ�9600
����11.059M
*********************************************************/
void uart_init()
{
				TMOD=0x20;
				TH1=0xfd; //9600
				TL1=0xfd;
				PCON=0x80;//9600
				SCON=0x50;
				TR1=1; //start Timer1
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
				Delayms(2);
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





