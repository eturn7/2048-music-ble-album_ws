/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : main.c
-  Author  :  xiongzh		Date  :  2019-7-31
-  Description  :  主函数的实现文件
				   几个LED灯的宏定义
				   还有全局变量的定义	
				   
-  Others  :  包含了一些所需的头文件
-  Function List  :  
   1. main.c :  整个项目的实现
*************************************************************/
#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "get_xy.h"
#include "list.h"
#include "game_func.h"
#include "dirve.h"
#include "xiangce.h"
#include "music_play.h"

int x, y;
int flag_home;
int main()
{
	while (1)
	{
		show_bmp_no_effect(0, 0, "./picture/welcome.bmp");
		get_xy(&x, &y);
		//
		show_bmp_no_effect(0, 0, "./choose.bmp");
		//显示三个选择功能的图片
		printf("choose app");

		get_xy(&x, &y);

		if (533 < x && x < 800)
		{
			printf("getin picture");
			xiangce(&flag_home);
			printf("%d",&flag_home);
		}
		if (266 < x && x <500)
		{
			printf("music!!");
			musicplay();
		}
		if (0 < x && x < 266)
		{
			printf("getin 2048");
			game_play();
		}
		return 0;
	}
}