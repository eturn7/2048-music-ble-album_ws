/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : effect.h
-  Author  :  xiongzh		Date:  2019-8-1
-  Description  :  
				  1、对effect.c文件内的函数进行声明
-  Others  :	none
-  Function List  :  
   1.  effect1~4() : 四种不同的刷图特效
*************************************************************/
#ifndef __EFFECT_H__
#define __EFFECT_H__

void effect1(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h);
void effect2(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h);
void effect3(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h);
void effect4(int x,int y,unsigned int *m_fd,unsigned int *lcd_buf,int w,int h);


#endif