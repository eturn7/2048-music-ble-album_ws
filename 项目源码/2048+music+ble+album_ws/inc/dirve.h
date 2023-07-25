/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : dirve.h
-  Author  :  xiongzh		Date:  2019-7-31
-  Description  :  
				  1、对dirve.c文件内的函数进行声明
				  2、包含一些dirve.c文件需要的头文件
				  3、定义dirve.c文件所需要的宏定义
-  Others  :	none
-  Function List  :  
   1. dirve_led_beff()  :  驱动led和beef
*************************************************************/
#ifndef __DIRVE_H__
#define __DIRVE_H__

#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include "show_bmp.h"
#include "get_xy.h"

/* 定义led的宏 */
#define LED1 _IO(TEST_MAGIC,0)
#define LED2 _IO(TEST_MAGIC,1)
#define LED3 _IO(TEST_MAGIC,2)
#define LED4 _IO(TEST_MAGIC,3)
#define TEST_MAGIC 'x'  //定义幻数 

void dirve_led_beff(int *x1,int *x2,int *x,int *y,int *y1,int *y2,int fd2,int fd3);

#endif