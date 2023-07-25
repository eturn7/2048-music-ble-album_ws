/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : square.h
-  Author  :  xiongzh		Date:  2019-8-1
-  Description  :  
				  1、square.c文件内的函数进行声明
				  2、定义了方阵结构体、中间链表结构体、还有进度结构体
				  3、把方阵单个节点定义为全局变量
-  Others  :	none
-  Function List  :  
   1.  init_temp_list())  : 整个个中间链表节点初始化
   2.  square_init()  : 整个方阵初始化
*************************************************************/

#ifndef __SQUARE_H__
#define __SQUARE_H__

/* 方阵定义 */
typedef struct square 
{
	int num;
	int num_y;
	int x;
	int y;
	char *name;
	struct square *up;
	struct square *down;
	struct square *next;
	struct square *bef;
}SQ,*SQ_P;

/* 中间变量结构体定义 */
typedef struct temp_square 
{
	int num_y;
	char *name;
	struct temp_square *next;
}TEMP,*TEMP_P;

/* 进度结构体定义 */
typedef struct write_square 
{
	int num;
	int num_y;
	char name[20];
}WR,*WR_P;

/* 定义全局方阵节点 */
SQ_P sq_1;
SQ_P sq_2;
SQ_P sq_3;
SQ_P sq_4;
SQ_P sq_5;
SQ_P sq_6;
SQ_P sq_7;
SQ_P sq_8;
SQ_P sq_9;
SQ_P sq_10;
SQ_P sq_11;
SQ_P sq_12;
SQ_P sq_13;
SQ_P sq_14;
SQ_P sq_15;
SQ_P sq_16;

SQ_P square_init();
TEMP_P init_temp_list();

#endif