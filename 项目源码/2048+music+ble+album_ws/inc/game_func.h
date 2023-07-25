/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : game_func.h
-  Author  :  xiongzh		Date:  2019-8-1
-  Description  :  
				  1、对game_func.c文件内的函数进行声明
				  2、包含一些game_func.c需要的头文件
				  3、外部声明和全局变量的定义
-  Others  :	none
-  Function List  :  
   1.  get_x_rand() ：随机获取1~16，也就是方阵对应的位置
   2.  get_y_rand() ：获取0~1，也就是随机显示的图片下标
   3.  read_sq_from_file() : 读取进度
   4.  safe_now_sq_to_file() ：保存进度
   5.  set_sq() ：清空方阵
   6.  game_win() ：判定是否win
   7.  sq_is_full() ：判定方阵是否已满
   8.  temp_sq_is_null() ：判定中间链表是否为空
   9.  sq_is_null() ：判定方阵是否为空
   10. sq_to_temp() ：把方阵数据复制给中间链表
   11. undo_one_step() ：撤回一步
   12. shift_to_down() ：下移
   13. shift_to_up() ：上移
   14. shift_to_right() ：右移
   15. shift_to_left() ：左移
   16. insert_sq() ：随机生成2或者4
   17. show_all_bmp() ：遍历显示所有图片
*************************************************************/

#ifndef __GAME_FUNC_H__
#define __GAME_FUNC_H__

#include <stdio.h>
#include "square.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "getpto.h"
#include "show_bmp.h"
#include "game_func.h"
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <fcntl.h>	
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "get_xy.h"
#include "list.h" 
#include "dirve.h"

extern char *bmp_name[];

int score;
int temp_score;

bool game_win(SQ_P bmp_head);
void get_x_rand(int *x,int m);
void get_y_rand(int *y);
int read_sq_from_file(SQ_P bmp_head,int fd);
void safe_now_sq_to_file(SQ_P bmp_head,int fd,int flag);
void set_sq(SQ_P bmp_head);
bool sq_is_full(SQ_P bmp_head);
bool sq_is_null(SQ_P bmp_head);
bool temp_sq_is_null(TEMP_P temp_head);
void sq_to_temp(SQ_P bmp_head,TEMP_P temp_head);
void insert_sq(SQ_P bmp_head,SQ_P *sq_buf);
void show_all_bmp(SQ_P bmp_head);
void shift_to_left(SQ_P bmp_head,int *flag,TEMP_P temp_head);
void shift_to_right(SQ_P bmp_head,int *flag,TEMP_P temp_head);
void shift_to_up(SQ_P bmp_head,int *flag,TEMP_P temp_head);
void shift_to_down(SQ_P bmp_head,int *flag,TEMP_P temp_head);
void undo_one_step(SQ_P bmp_head,TEMP_P temp_head);
void game_play();


#endif