/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : show_bmp.c
-  Author  :  xiongzh		Date  :  2019-7-31
-  Description  : 
				  1、封装了获取登录密码的函数
				  2、封装了几个显示图片的函数
				  3、还有一个清屏的函数
-  Others  :  包含了一些所需的头文件
-  Function List  :  
   1.  input_mask() ：获取密码
   2.  black_lcd()  : 清屏
   3.  show_bmp_no_effect() : 没有特效的显示图片
   4.  show_bmp()  : 附带特效的显示图片
*************************************************************/
#include "show_bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <fcntl.h>	
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <strings.h>
#include "bit_bmp.h"
#include "effect.h"
#include "get_xy.h"

#define wh (bmp_info.width * bmp_info.height) // 定义图片宽和高的乘积的宏


/***********************************************************
-  Description  :  点击屏幕的按键，获取密码 每点击一次显示一个
-  Calls  :  get_xy.c->get_xy()、show_bmp_no_effect()
-  Iuput  :  1、密码字符数组的下标地址 2~6、坐标地址 7、密码字符数组的起始地址
-  Return  :  none
-  Others  :  none
*************************************************************/
void input_mask(int *l,int *x1,int *x2,int *x,int *y,int *y1,int *y2,char *mask)
{
	while(*l < 9)
		{
			/* 点击数字九键然后把数字对应的字符赋值给字符数组 */
			get_xy(x,y);
			if(*x <69 && *y > 273 && *y < 320)
			{
				mask[*l] = '0';
			}
			else if(*x > 69 && *x < 145 && *y > 273 && *y < 320)
			{
				mask[*l] = '1';
			}
			else if(*x > 145 && *x < 207 && *y > 273 && *y < 320)
			{
				mask[*l] = '2';
			}
			else if(*x <69 && *y > 320 && *y < 368)
			{
				mask[*l] = '3';
			}
			else if(*x > 69 && *x < 145 && *y > 320 && *y < 368)
			{
				mask[*l] = '4';
			}
			else if(*x > 145 && *x < 207 && *y > 320 && *y < 368)
			{
				mask[*l] = '5';
			}
			else if(*x <69 && *y > 368 && *y < 413)
			{
				mask[*l] = '6';
			}
			else if(*x > 69 && *x < 145 && *y > 368 && *y < 413)
			{
				mask[*l] = '7';
			}
			else if(*x > 145 && *x < 207 && *y > 368 && *y < 413)
			{
				mask[*l] = '8';
			}
			else if(*x < 69 && *y > 413)
			{
				mask[*l] = '9';
			}
			else if(*x > 69 && *x < 207 && *y > 413)
			{
				mask[*l] = ' ';
			}
			else if(*x > 333 && *x < 503 && *y > 265 && *y < 288)
			{
				break;
			}
			
			if(*x < 208 && *y > 272)
			{
				show_bmp_no_effect(332+(*l)*20,198,"./mask.bmp");
				(*l)++;
			}
		}
}

/***********************************************************
-  Description  :  清屏成黑色
-  Calls  :  open()、mmap()、munmap()
-  Iuput :  none
-  Return  :  none
-  Others  :  mmap()这里第四个参数这里不能用MAP_PRIVATE，不然存储和读取游戏进度会有问题
*************************************************************/
void black_lcd()
{
	unsigned int black = 0x00000000; 
	int i=0;
	int lcd_fd = open("/dev/fb0",O_RDWR);
	/* 映射lcd */
	unsigned int *m_fd =(unsigned int *)mmap(NULL, 800*480*4, PROT_WRITE | PROT_READ, MAP_SHARED
			,lcd_fd,0);
	for(i=0 ;i<800*480;i++)
	{
		*(m_fd+i) = black;
	}
	munmap(m_fd,800*480*4);
	exit(0);
}

/***********************************************************
-  Description  :  无特效的显示图片
-  Calls  :  open()、mmap()、munmap()、bzero()、read()、close()
-  Iuput :  1~2、显示图片的其实坐标 3、显示的图片位置
-  Return  :  none
-  Others  :  需要注意不规则图片而产生的地址对齐问题
*************************************************************/
void show_bmp_no_effect(int x,int y,char *file)
{
	/* 打开图片文件和lcd设备文件 */
	int bmp_fd = open(file,O_RDONLY);
	int lcd_fd = open("/dev/fb0",O_RDWR);
	
	/* 映射lcd */
	unsigned int *m_fd =(unsigned int *)mmap(NULL, 800*480*4, PROT_WRITE | PROT_READ, MAP_SHARED
			,lcd_fd,0);
	
	/* 定义存储文件信息的结构体，并读取文件信息存入结构体 */
	struct bitmap_header bmp_head;
	struct bitmap_info bmp_info;
	bzero(&bmp_head, 14);
	bzero(&bmp_info, 40);
	
	read(bmp_fd, &bmp_head, 14);
	read(bmp_fd, &bmp_info, 40);
	
	unsigned int lcd_buf[wh]; //定义存储图片像素点的缓冲区
	bzero(lcd_buf,wh*4);  
	
	int ret = (4-(bmp_info.width*3)%4)%4; // 计算出不是4的整数倍宽度的图片因地质对其，多读取的字节数
	char bmp_buf[3*wh];
	/* 把像素信息读取到缓冲区 */
	read(bmp_fd,bmp_buf,wh*3);
	close(bmp_fd);
	
	int i,j=0;
	/* 把3为像素点转化为4位的 */
	for(i=0; i<wh; i++)
	{
		if((i%(bmp_info.width) == 0) && (i != 0))
			j++;
		lcd_buf[i] = bmp_buf[i*3+j*ret]<<0|bmp_buf[i*3+1+j*ret]<<8|bmp_buf[i*3+2+j*ret]<<16|0x00<<24;
	}
	i = 0;
	j = 0;
	/* 回正像素点并写入lcd */
	for(i=y; i<bmp_info.height+y; i++)
	{
		for(j=x; j<bmp_info.width+x; j++)
		{
				*(m_fd + i*800+j) = lcd_buf[(bmp_info.height-1+y-i)*bmp_info.width+j-x];
		}
	}
	
	munmap(m_fd,800*480*4);
	close(lcd_fd);

}

/***********************************************************
-  Description  :  有特效的显示图片
-  Calls  :  open()、mmap()、munmap()、bzero()、
			 read()、close()、srand()、rand()、
			 effect.c->effect1~4()
-  Iuput :  1~2、显示图片的其实坐标 3、显示的图片位置
-  Return  :  none
-  Others  :  需要注意不规则图片而产生的地址对齐问题
			  这里的特效是因为调用了封装了显示特效的函数
*************************************************************/
void show_bmp(int x,int y,char *file)
{
	/* 把四种特效函数先放入函数指针数组 */
	void (*fun[4])(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h) = {effect1,effect2,effect3,effect4};
	
	int num =0;
	srand(time(0));
	num = rand()%4;
	
	int bmp_fd = open(file,O_RDONLY);
	int lcd_fd = open("/dev/fb0",O_RDWR);

	unsigned int *m_fd =(unsigned int *)mmap(NULL, 800*480*4, PROT_WRITE | PROT_READ, MAP_SHARED
			,lcd_fd,0);

	struct bitmap_header bmp_head;
	struct bitmap_info bmp_info;
	bzero(&bmp_head, 14);
	bzero(&bmp_info, 40);
	
	read(bmp_fd, &bmp_head, 14);
	read(bmp_fd, &bmp_info, 40);

	
	unsigned int lcd_buf[wh];
	
	bzero(lcd_buf,wh*4);  
	int ret = (4-(bmp_info.width*3)%4)%4;
	char bmp_buf[3*wh+ret*bmp_info.height];
	//lseek();
	read(bmp_fd,bmp_buf,wh*3+ret*bmp_info.height);
	close(bmp_fd);
	

	int i,j=0;
	for(i=0; i<wh; i++)
	{
		if((i%(bmp_info.width) == 0) && (i != 0))
			j++;
		lcd_buf[i] = bmp_buf[i*3+j*ret]<<0|bmp_buf[i*3+1+j*ret]<<8|bmp_buf[i*3+2+j*ret]<<16|0x00<<24;
	}
	/* 通过随机值作函数指针数组的下标，随机调用特效函数 */
	fun[num](x,y,m_fd,lcd_buf,bmp_info.width,bmp_info.height);

	munmap(m_fd,800*480*4);
	close(lcd_fd);

}