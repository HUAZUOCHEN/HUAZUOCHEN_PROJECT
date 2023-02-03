/*******************************************
名称：51WIFI视频小车
作者：化作尘
时间：2020年11月20日11点27分
哔哩哔哩项目视频：https://www.bilibili.com/video/BV1gf4y1v7qa
csdn博客教程：https://blog.csdn.net/mbs520/article/details/109843972
邮箱:2809786963@qq.com
*******************************************/

#include "reg52.h"


#define uchar unsigned char
#define uint unsigned char


/***小车控制定义**/
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

uchar rec;//接收到的字节
uchar buff[9]="00000000";//接收到的数据包
uchar flag;//数据包开始接收标志
uchar num; //数组下标
uchar buf_ready;//接收到数据包置1

/*********************************************************
函数名：主函数
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
								if(buf_ready == 1)//接收一组数据完成标志
								{
											buf_ready = 0;
								}
								if(buff[3]>0xb0){DIR = QIAN;Delayus(buff[3]-0xa0);DIR = STOP;Delayus(0x20);}//buff[3]控制前后
								else if(buff[3]<0x40){DIR = HOU;Delayus(0x60-buff[3]);DIR = STOP;Delayus(0x20);}//通过延时控制速度
								
								else if(buff[4]>0xd0){DIR = YOU;Delayus(20);DIR = STOP;Delayus(5);}//buff[4]控制左右
								else if(buff[4]<0x20){DIR = ZUO;Delayus(20);DIR = STOP;Delayus(5);}
								else DIR = STOP;
        }
}

/*********************************************************
函数名：串口中断
*********************************************************/
void uart_timer() interrupt 4
{
        if(RI)
        {
								RI = 0;
                rec=SBUF;
								
								if(rec==0x66 && flag==0)//数据头
								{
											flag = 1;  
											num=0;
											buff[0] = rec;
								}
								else if(flag == 1)//开始接收数据包buff[8]
								{
									
											num++;
											buff[num] = rec;
											if(num==7 && buff[7]==0x99)//接收到数据尾
											{
													buf_ready = 1;
													flag = 0;
													num = 0;
											}
											else if(num == 7)//接收错误
											{
													flag = 0;
													num = 0;
											}
								}
        }
}

/*********************************************************
函数名：串口初始化
波特率：9600
晶振：11.059M
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
函数名：串口发送一个字节
*********************************************************/
void uart_tx_byte(uchar str)
{
        SBUF=str;
        while(!TI);
				Delayms(2);
}

/*********************************************************
函数名：串口发送一个字符串
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
函数名：延时函数
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





