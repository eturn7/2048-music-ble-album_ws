/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : effect.c
-  Author  :  xiongzh		Date  :  2019-7-31
-  Description  : 
				  1、封装了图片显示特效的函数
-  Others  :  none
-  Function List  :  
   1.  effect1() ：百叶窗显示
   2.  effect2() : 方格显示
   3.  effect3() ：中间扩散
   4.  effect4() ：中间向两边扩散
*************************************************************/
#include "effect.h"
#include <unistd.h>
#include <stdio.h>

/***********************************************************
-  Description  :  百叶窗
-  Calls  :  sleep()
-  Iuput :  1~2、显示图片起始坐标 3、lcd映射地址 4、图片像素点缓冲区起始地址 5~6、图片宽，高
-  Return  :  none
-  Others  :  none
*************************************************************/
void effect1(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h)
{
	int i,i1,i2,i3,i4,j;
	for(i=30,i1=112,i2=194,i3=276,i4=358; i<112,i1<194,i2<276,i3<358,i4<440; i++,i1++,i2++,i3++,i4++)
	{
		for(j=0; j<800; j++)
		{     //   
			if((i >= y && i < h+y) && (j >= x && j < x+w))
			{
				*(m_fd+i*800+j) = lcd_buf[(h-1-i+30)*w+j-65];
				*(m_fd+i1*800+j) = lcd_buf[(h-1-i1+30)*w+j-65];
				*(m_fd+i2*800+j) = lcd_buf[(h-1-i2+30)*w+j-65];
				*(m_fd+i3*800+j) = lcd_buf[(h-1-i3+30)*w+j-65];
				*(m_fd+i4*800+j) = lcd_buf[(h-1-i4+30)*w+j-65];
			}
		}
		usleep(10000);
	}
}

/***********************************************************
-  Description  :  分四块，先扫两块，后扫一块
-  Calls  :  sleep()
-  Iuput :  1~2、显示图片起始坐标 3、lcd映射地址 4、图片像素点缓冲区起始地址 5~6、图片宽，高
-  Return  :  none
-  Others  :  none
*************************************************************/
void effect2(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h)
{
	int i,j,k,l;
	for(i=235,k=235; i<=0,k<440; i--,k++)
	{
		for(j=317,l=317; j>=0,l<570; j--,l++)
		{     //   
			if((i >= y && i < h+y) && (j >= x && j < x+w))
			{
				*(m_fd+i*800+j) = lcd_buf[(h-1-i+30)*w+j-65];
				*(m_fd+k*800+l) = lcd_buf[(h-1-k+30)*w+l-65];
			}
		}
		usleep(100);
	}
	usleep(500000);
	for(i=235,k=235; i<=0,k<440; i--,k++)
	{
		for(j=317,l=317; j<570,l>=0; j++,l--)
		{     //   
			if((i >= y && i < h+y) && (j >= x && j < x+w))
			{
				*(m_fd+i*800+j) = lcd_buf[(h-1-i+30)*w+j-65];
				*(m_fd+k*800+l) = lcd_buf[(h-1-k+30)*w+l-65];
			}
		}
		usleep(100);
	}
		
}

/***********************************************************
-  Description  :  从中间开始，向两边扩散
-  Calls  :  sleep()
-  Iuput :  1~2、显示图片起始坐标 3、lcd映射地址 4、图片像素点缓冲区起始地址 5~6、图片宽，高
-  Return  :  none
-  Others  :  none
*************************************************************/
void effect3(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h)
{
	int i,j,k;
	for(j=318,k=318;j>=65,k<570;j--,k++)
	{
		for(i=479;i>=0;i--)
		{
			if((i >= y && i < h+y) && (j >= x && j < x+w))
			{
				*(m_fd+i*800+j) = lcd_buf[(h-1-i+30)*w+j-65];
				*(m_fd+i*800+k) = lcd_buf[(h-1-i+30)*w+k-65];
			}
		}
		usleep(100);
	}
}

/***********************************************************
-  Description  :  从中间开始，向周围扩散扩散
-  Calls  :  sleep()
-  Iuput :  1~2、显示图片起始坐标 3、lcd映射地址 4、图片像素点缓冲区起始地址 5~6、图片宽，高
-  Return  :  none
-  Others  :  none
*************************************************************/
void effect4(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h)
{
	int i,j;
	for(i=194;i<276;i++)
	{
		for(j=267;j<367;j++)
		{
				//	 -41 		+41 		-50 		 +50
			if((i >= 194 && i <276 ) && (j >= 267 && j < 367))
				*(m_fd+i*800+j) = lcd_buf[(h-1-i+30)*w+j-65];
		}
	}
	usleep(300000);
	for(i=153;i<317;i++)
	{
		for(j=217;j<417;j++)
		{
			if((i >= 153 && i <317 ) && (j >= 217 && j < 417))
				*(m_fd+i*800+j) = lcd_buf[(h-1-i+30)*w+j-65];
		}
	}
	usleep(300000);
	for(i=112;i<358;i++)
	{
		for(j=167;j<467;j++)
		{
			if((i >= 112 && i <358 ) && (j >= 167 && j < 467))
				*(m_fd+i*800+j) = lcd_buf[(h-1-i+30)*w+j-65];
		}
	}
	usleep(300000);
	for(i=30;i<440;i++)
	{
		for(j=67;j<567;j++)
		{
			if((i >= 30 && i <440 ) && (j >= 67 && j < 567))
				*(m_fd+i*800+j) = lcd_buf[(h-1-i+30)*w+j-65];
		}
	}
}
