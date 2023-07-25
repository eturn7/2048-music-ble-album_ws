/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : show_bmp.h
-  Author  :  xiongzh		Date:  2019-8-1
-  Description  :  
				  1、对show_bmp.c文件内的函数进行声明
-  Others  :	none
-  Function List  :  
   1.  input_mask() ：获取密码
   2.  black_lcd()  : 清屏
   3.  show_bmp_no_effect() : 没有特效的显示图片
   4.  show_bmp()  : 附带特效的显示图片
*************************************************************/

#ifndef __SHOW_BMP_H__
#define __SHOW_BMP_H__

void show_bmp(int x,int y,char *file);
void show_bmp_no_effect(int x,int y,char *file);
void black_lcd();
void input_mask(int *l,int *x1,int *x2,int *x,int *y,int *y1,int *y2,char *mask);

#endif