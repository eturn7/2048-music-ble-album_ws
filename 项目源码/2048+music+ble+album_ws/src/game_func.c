/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : game_func.c
-  Author  :  xiongzh		Date  :  2019-7-31
-  Description  : 
				  1、封装了所有2048需要的操作函数
				  2、以及包含了需要的头文件
-  Others  :  none
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

#include "game_func.h"
#include "square.h"
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
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
#include <pthread.h>
#include "xiangce.h"
#include "music_play.h"
#include "lanya.h"


/* 定义一个指针数组，存放2048游戏中的2~2018图片的路径 */
char *bmp_name[] = {"./2_2048/2.bmp","./2_2048/4.bmp","./2_2048/8.bmp","./2_2048/16.bmp",
					"./2_2048/32.bmp","./2_2048/64.bmp","./2_2048/128.bmp","./2_2048/256.bmp",
					"./2_2048/512.bmp","./2_2048/1024.bmp","./2_2048/2048.bmp"
					};
/* 定义一个指针数组，存放2048游戏中的分数图片路径 */
char *score_bmp[] = {"./score/score0.bmp","./score/score1.bmp","./score/score2.bmp","./score/score3.bmp","./score/score4.bmp",
					"./score/score5.bmp","./score/score6.bmp","./score/score7.bmp","./score/score8.bmp","./score/score9.bmp"};

					

int score_buf[]={4,8,16,32,64,128,256,512,1024,2048}; // 定义的分数数组，加积分时有用

/* 获取1~16，也就是方阵对应的位置 */
void get_x_rand(int *x,int m)
{
	srand(clock());
	*x= 1+rand()%m;	
}
/* 获取0~1，也就是随机显示的图片下标 */
void get_y_rand(int *y)
{
	struct timeval tpstart;
    gettimeofday(&tpstart,NULL);
    srand(tpstart.tv_usec);
	*y= rand()%2;	
}

/***********************************************************
-  Description  :  读取进度
-  Calls  :  calloc()、read()、strcpy()、
-  Iuput :  1、方阵头指针 2、进度文件描述符
-  Return  :  num
-  Others  :  进度文件内存储的不是完整的方阵节点，只储存了num_y与图片名
*************************************************************/
int read_sq_from_file(SQ_P bmp_head,int fd)
{
	int num = 0;
	int ret = 0;
	SQ_P p = bmp_head; //中间变量，用于遍历方阵
	WR_P pos = calloc(1,sizeof(WR)); //定义读取文件的缓冲区
	while(1)
	{
		/* 读取进度 */
		ret = read(fd,pos,sizeof(WR));
		if(ret == 0)
			break;
		else
			num = 1; // 用于判定是否产生随机图片
		while(p != NULL)
		{
			/* 把进度写入方阵 */
			if(pos->num == p->num)
			{
				p->num_y = pos->num_y;
				p->name = calloc(1,20);
				strcpy(p->name,pos->name);
			}
			p = p->next;
		}
		p = bmp_head;
	}
	return num;
}

/***********************************************************
-  Description  :  保存进度
-  Calls  :  calloc()、read()、strcpy()、
-  Iuput :  1、方阵头指针 2、进度文件描述符 3、进度文件是否为空的标志位
-  Return  :  none
-  Others  :  进度文件内存储的不是完整的方阵节点，只储存了num_y与图片名
			  注意mmap的第四个参数是MAP_PRIVATE的话，是实现不了的
*************************************************************/
void safe_now_sq_to_file(SQ_P bmp_head,int fd,int flag)
{
	int ret =0 ;
	unsigned int *m_fd;
	SQ_P p = bmp_head;
	WR_P pos = calloc(1,sizeof(WR));
	lseek(fd, 0, SEEK_SET);
	/* 先清空文件 */
	if(flag != 0)
	{
		m_fd =(unsigned int *)mmap(NULL, 28*16, PROT_WRITE | PROT_READ, MAP_SHARED
			,fd,0);
		bzero(m_fd,28*16);
	}
	/* 只写入非空方阵节点 */
	while(p != NULL)
	{
		if(p->name != NULL)
		{
			pos->num = p->num;
			pos->num_y = p->num_y;
			strcpy(pos->name,p->name);
			ret = write(fd,pos,sizeof(WR));
		}
		p = p->next;
	}
	if(flag != 0)
		munmap(m_fd,28*16);
}
/* 清空方阵 */
void set_sq(SQ_P bmp_head)
{
	SQ_P p = bmp_head;
	while(p != NULL)
	{
		if(p->name != NULL)
		{
			p->num_y = -1;
			free(p->name);
			p->name = NULL;
		}
		p = p->next;
	}
}
/* 判断游戏是否胜利，这里设的是出现一个2048 */
bool game_win(SQ_P bmp_head)
{
	SQ_P p = bmp_head;
	while(p != NULL)
	{
		if(p->name != NULL)
		{
			if(!strcmp(p->name,"./2_2048/2048.bmp"))
			{
				return true;
			}
		}
		p = p->next;
	}
	return false;	
}
/* 判断方阵是否满了 */
bool sq_is_full(SQ_P bmp_head)
{
	SQ_P p = bmp_head;
	while(p != NULL)
	{
		if(p->name == NULL)
		{
			return false;
		}
		p = p->next;
	}
	return true;
}
/* 判断保存前一步方阵的中间链表是否为空 */
bool temp_sq_is_null(TEMP_P temp_head)
{
	TEMP_P p = temp_head;
	while(p != NULL)
	{
		if(p->name != NULL)
		{
			return false;
		}
		p = p->next;
	}
	return true;
	
}
/* 判断方阵是否为空 */
bool sq_is_null(SQ_P bmp_head)
{
	SQ_P p = bmp_head;
	while(p != NULL)
	{
		if(p->name != NULL)
		{
			return false;
		}
		p = p->next;
	}
	return true;
	
}

/***********************************************************
-  Description  :  把上一步的方阵数据存入中间链表
-  Calls  :  calloc()
-  Iuput :  1、方阵头指针 2、中间链表头指针
-  Return  :  none
-  Others  :  滑屏后，方阵还未移动前先保存方阵数据到中间链表
*************************************************************/
void sq_to_temp(SQ_P bmp_head,TEMP_P temp_head)
{
	SQ_P pos1 = NULL;
	TEMP_P pos2 = NULL;
	
	/* 方阵和中间链表同时遍历 */
	for(pos1 = bmp_head,pos2 = temp_head; pos1 != NULL; pos1 = pos1->next,pos2 = pos2->next)
	{	
		/* 如果方阵节点不为空，就把数据覆盖中间链表节点 */
		if(pos1->name != NULL)
		{
			pos2->num_y = pos1->num_y;
			pos2->name = calloc(1,20);
			strcpy(pos2->name,pos1->name);
		}
		/* 为空的话，清空中间链表节点 */
		else
		{
			pos2->num_y = -1;
			if(pos2->name != NULL)
				free(pos2->name);
			pos2->name = NULL;
		}
	}
}

/* 反上面一个函数，把中间链表数据写入方阵 */
void undo_one_step(SQ_P bmp_head,TEMP_P temp_head)
{
	SQ_P pos1 = NULL;
	TEMP_P pos2 = NULL;
	for(pos1 = bmp_head,pos2 = temp_head; pos1 != NULL; pos1 = pos1->next,pos2 = pos2->next)
	{
		pos1->num_y = pos2->num_y;
		if(pos2->name == NULL)
		{
			if(pos1->name != NULL)
				free(pos1->name);
			pos1->name = NULL;
			pos1->num_y = -1;
		}
		else
		{
			if(pos1->name != NULL)
				free(pos1->name);
			pos1->name = calloc(1,20);
			strcpy(pos1->name,pos2->name);
		}
	}
	show_bmp_no_effect(0,0,"./game_pto/g2048.bmp");
	show_all_bmp(bmp_head);
}

/***********************************************************
-  Description  :  下滑操作
-  Calls  :  sq_to_temp()、strcpy()、strcpy()、calloc()、free()
			 show_bmp.c->show_bmp_no_effect()、show_all_bmp()、
-  Iuput :  1、方阵头指针 2、判定是否产生随机图片的标志位地址 3、中间链表头指针
-  Return  :  none
-  Others  :  移动前也保存分数，让撤回后分数也撤回
*************************************************************/
void shift_to_down(SQ_P bmp_head,int *flag,TEMP_P temp_head)
{
	sq_to_temp(bmp_head,temp_head);
	temp_score =score;
	*flag=0;
	int i = 0;
	SQ_P pos = NULL;
	SQ_P p =sq_13;
	/* 从方阵下一行至第一行遍历，逐个判断能否移动以及移动或者没移动后能否合并 */
	while(p != NULL && i <= 16)
	{
		/* 判断方阵节点的下一个不是空，且方阵节点本身内name不为空，
		就进去判断可移动还是可合并 */
		if(p->down != NULL && p->name != NULL)
		{
			pos = p;
			while(pos->down->name == NULL && pos->down != NULL)
			{
				pos = pos->down;
				if((pos->num) >= 13)
					break;  
			}
			/* 说明没有移动，因为下一个节点不为空 */
			if(pos == p)
			{
				/* 判定能否合并 */
				if(!strcmp(pos->name,pos->down->name))
				{
					/* 合并的操作 */
					pos->down->num_y++; // 图片数组下标加1
					pos->down->name = calloc(1,20);
					strcpy(pos->down->name,bmp_name[pos->down->num_y]); // 拷贝下一个数组下标的图片
					pos->num_y = -1; // 之前判定图片数组下标的初始化
					free(pos->name);
					pos->name = NULL; // 之前判定的name初始化
					*flag = 1;
					score += score_buf[pos->down->num_y-1]; // 分数加合成后的图片数字分数
					if(pos->down->down != NULL)
					{	
						if(!strcmp(pos->down->name,pos->down->down->name))
						{
							pos->down->down->num_y++;
							pos->down->down->name = calloc(1,20);
							strcpy(pos->down->down->name,bmp_name[pos->down->down->num_y]);
							pos->down->num_y = -1;
							free(pos->down->name);
							pos->down->name = NULL;
							*flag = 1;
							score += score_buf[pos->down->down->num_y-1];
						}
					}
				}
				i++;
				if(i%4 == 0 && i != 16)//i != 的值改变，单独一行调节
				{
					p = p->up->bef->bef->bef;
				}
				else
					p = p->next;
				continue;
			}
			pos->num_y = p->num_y;
			pos->name = calloc(1,20);
			strcpy(pos->name,p->name);
			p->num_y = -1;
			free(p->name);
			p->name = NULL;
			*flag = 1;
			if(pos->down != NULL)
			{
				if(!strcmp(pos->name,pos->down->name))
				{
					pos->down->num_y++;
					pos->down->name = calloc(1,20);
					strcpy(pos->down->name,bmp_name[pos->down->num_y]);
					pos->num_y = -1;
					free(pos->name);
					pos->name = NULL;
					*flag = 1;
					score += score_buf[pos->down->num_y-1];
				}
				if(pos->down->down != NULL)
				{	
					if(!strcmp(pos->down->name,pos->down->down->name))
					{
						pos->down->down->num_y++;
						pos->down->down->name = calloc(1,20);
						strcpy(pos->down->down->name,bmp_name[pos->down->down->num_y]);
						pos->down->num_y = -1;
						free(pos->down->name);
						pos->down->name = NULL;
						*flag = 1;
						score += score_buf[pos->down->down->num_y-1];
					}
				}
			}
		}
		i++;
		if(i%4 == 0 && i != 16)
		{
			p = p->up->bef->bef->bef;
		}
		else
			p = p->next;
	}
	show_bmp_no_effect(0,0,"./game_pto/g2048.bmp");
	show_all_bmp(bmp_head);
}
/* 上滑操作 */
void shift_to_up(SQ_P bmp_head,int *flag,TEMP_P temp_head)
{
	temp_score =score;
	sq_to_temp(bmp_head,temp_head);
	*flag = 0;
	SQ_P pos = NULL;
	SQ_P p =bmp_head;
	while(p != NULL)
	{
		if(p->up != NULL && p->name != NULL)
		{
			pos = p;
			while(pos->up->name == NULL && pos->up != NULL)
			{
				pos = pos->up;
				if((pos->num) <= 4) 
					break;
			}
			if(pos == p)
			{
				if(!strcmp(pos->name,pos->up->name))
				{
					pos->up->num_y++;
					pos->up->name = calloc(1,20);
					strcpy(pos->up->name,bmp_name[pos->up->num_y]);
					pos->num_y = -1;
					free(pos->name);
					pos->name = NULL;
					*flag = 1;
					score += score_buf[pos->up->num_y-1];
					if(pos->up->up != NULL)
					{	
						if(!strcmp(pos->up->name,pos->up->up->name))
						{
							pos->up->up->num_y++;
							pos->up->up->name = calloc(1,20);
							strcpy(pos->up->up->name,bmp_name[pos->up->up->num_y]);
							pos->up->num_y = -1;
							free(pos->up->name);
							pos->up->name = NULL;
							*flag = 1;
							score += score_buf[pos->up->up->num_y-1];
						}
					}
				}
				p = p->next;
				continue;
			}
			pos->num_y = p->num_y;
			pos->name = calloc(1,20);
			strcpy(pos->name,p->name);
			p->num_y = -1;
			free(p->name);
			p->name = NULL;
			*flag = 1;
			if(pos->up != NULL)
			{
				if(!strcmp(pos->name,pos->up->name))
				{
					pos->up->num_y++;
					pos->up->name = calloc(1,20);
					strcpy(pos->up->name,bmp_name[pos->up->num_y]);
					pos->num_y = -1;
					free(pos->name);
					pos->name = NULL;
					*flag = 1;
					score += score_buf[pos->up->num_y-1];
				}
				if(pos->up->up != NULL)
				{	
					if(!strcmp(pos->up->name,pos->up->up->name))
					{
						pos->up->up->num_y++;
						pos->up->up->name = calloc(1,20);
						strcpy(pos->up->up->name,bmp_name[pos->up->up->num_y]);
						pos->up->num_y = -1;
						free(pos->up->name);
						pos->up->name = NULL;
						*flag = 1;
						score += score_buf[pos->up->up->num_y-1];
					}
				}
			}
		}
		p = p->next;
	}
	show_bmp_no_effect(0,0,"./game_pto/g2048.bmp");
	show_all_bmp(bmp_head);
}
/* 右滑操作 */
void shift_to_right(SQ_P bmp_head,int *flag,TEMP_P temp_head)
{
	temp_score =score;
	sq_to_temp(bmp_head,temp_head);
	*flag = 0;
	int i = 0;
	SQ_P p =sq_4;
	SQ_P pos1 = NULL;
	while(p != NULL)
	{
		if((p->num%4)!=0 && p->name != NULL)
		{
			pos1 = p;
			while(pos1->next->name == NULL)
			{
				pos1 = pos1->next;
				if(pos1->num%4==0)
					break;
			}
			if(pos1 == p)
			{
				if(!strcmp(pos1->name,pos1->next->name))
				{
					pos1->next->num_y++;
					pos1->next->name = calloc(1,20);
					strcpy(pos1->next->name,bmp_name[pos1->next->num_y]);
					pos1->num_y = -1;
					free(pos1->name);
					pos1->name = NULL;
					*flag = 1;
					score += score_buf[pos1->next->num_y-1];
					if(pos1->next->next != NULL && pos1->next->num%4 != 0)
					{	
						if(!strcmp(pos1->next->name,pos1->next->next->name))
						{
							pos1->next->next->num_y++;
							pos1->next->next->name=calloc(1,20);
							strcpy(pos1->next->next->name,bmp_name[pos1->next->next->num_y]);
							pos1->next->num_y = -1;
							free(pos1->next->name);
							pos1->next->name = NULL;
							*flag = 1;
							score += score_buf[pos1->next->next->num_y-1];
						}
					}
				}
				i++;
				if(i%4 == 0 && i != 16)//i != 的值改变，单独一行调节
				{
					p = p->next->next->next->down;
				}
				else
					p = p->bef;
				continue;
			}
			pos1->num_y = p->num_y;
			pos1->name = calloc(1,20);
			strcpy(pos1->name,p->name);
			p->num_y = -1;
			free(p->name);
			p->name = NULL;
			*flag = 1;
			if(pos1->next != NULL && pos1->num%4!=0)
			{
				if(!strcmp(pos1->name,pos1->next->name))
				{
					pos1->next->num_y++;
					pos1->next->name = calloc(1,20);
					strcpy(pos1->next->name,bmp_name[pos1->next->num_y]);
					pos1->num_y = -1;//?
					free(pos1->name);
					pos1->name = NULL;
					*flag = 1;
					score += score_buf[pos1->next->num_y-1];
					if(pos1->next->next != NULL)
					{	
						if((pos1->next->num %4) != 0)
						{
							if(!strcmp(pos1->next->name,pos1->next->next->name))//?
							{
								pos1->next->next->num_y++;
								pos1->next->next->name = calloc(1,20);
								strcpy(pos1->next->next->name,bmp_name[pos1->next->next->num_y]);
								pos1->next->num_y = -1;
								free(pos1->next->name);
								pos1->next->name = NULL;
								*flag = 1;
								score += score_buf[pos1->next->next->num_y-1];
							}
						}
					}
				}
			}
		}
		i++;
		if(i%4 == 0 && i != 16)//i != 的值改变，单独一行调节
		{
			p = p->next->next->next->down;
		}
		else
			p = p->bef;
	}
	
	show_bmp_no_effect(0,0,"./game_pto/g2048.bmp");
	show_all_bmp(bmp_head);
}
/* 左滑操作 */
void shift_to_left(SQ_P bmp_head,int *flag,TEMP_P temp_head)
{
	temp_score =score;
	//提前保存未移动的链表
	sq_to_temp(bmp_head,temp_head);
	//=====================
	SQ_P pos = NULL;
	SQ_P p =bmp_head;
	*flag=0;
	while(p != NULL)
	{
		if(p->bef != NULL && p->name != NULL)
		{
			pos = p;
			while(pos->bef->name == NULL && pos->bef != NULL)
			{
				pos = pos->bef;
				if((pos->num)%4==1)
					break;  
			}
			if(pos == p)
			{
				if(!strcmp(pos->name,pos->bef->name))
				{
					pos->bef->num_y++;
					pos->bef->name = calloc(1,20);
					strcpy(pos->bef->name,bmp_name[pos->bef->num_y]);
					pos->num_y = -1;
					free(pos->name);
					pos->name = NULL;
					*flag = 1;
					score += score_buf[pos->bef->num_y-1];
					if(pos->bef->bef != NULL)
					{	
						if(!strcmp(pos->bef->name,pos->bef->bef->name))
						{
							pos->bef->bef->num_y++;
							pos->bef->bef->name = calloc(1,20);
							strcpy(pos->bef->bef->name,bmp_name[pos->bef->bef->num_y]);
							pos->bef->num_y = -1;
							free(pos->bef->name);
							pos->bef->name = NULL;
							*flag = 1;
							score += score_buf[pos->bef->bef->num_y-1];
						}
					}
				}
				p = p->next;
				continue;
			}
			pos->num_y = p->num_y;
			pos->name = calloc(1,20);
			strcpy(pos->name,p->name);
			p->num_y = -1;
			free(p->name);
			p->name = NULL;
			*flag = 1;
			if(pos->bef != NULL)
			{
				if(!strcmp(pos->name,pos->bef->name))
				{
					pos->bef->num_y++;
					pos->bef->name = calloc(1,20);
					strcpy(pos->bef->name,bmp_name[pos->bef->num_y]);
					pos->num_y = -1;
					free(pos->name);
					pos->name = NULL;
					*flag = 1;
					score += score_buf[pos->bef->num_y-1];
				}
				if(pos->bef->bef != NULL)
				{	
					if(!strcmp(pos->bef->name,pos->bef->bef->name))
					{
						pos->bef->bef->num_y++;
						pos->bef->bef->name = calloc(1,20);
						strcpy(pos->bef->bef->name,bmp_name[pos->bef->bef->num_y]);
						pos->bef->num_y = -1;
						free(pos->bef->name);
						pos->bef->name = NULL;
						*flag = 1;
						score += score_buf[pos->bef->bef->num_y-1];
					}
				}
			}
		}
		p = p->next;
	}
	show_bmp_no_effect(0,0,"./game_pto/g2048.bmp");
	show_all_bmp(bmp_head);
}

/***********************************************************
-  Description  :  随机出现一个2或者4的图片，并随即放入一个空的方针节点
-  Calls  :  get_y_rand()、get_x_rand()、strcpy()、show_all_bmp()
			 
-  Iuput :  1、方阵头指针 2、判定是否产生随机图片的标志位地址 3、中间链表头指针
-  Return  :  none
-  Others  :  这里面我用了算法，让随机产生的数不会重复
			  以及用字典的方式代替了索引遍历
*************************************************************/
void insert_sq(SQ_P bmp_head,SQ_P *sq_buf)
{
	//sq_to_temp(bmp_head,temp_head);
	int temp = 0;
	int m=16;
	/* 产生一个数组内的数后让它与数组最后一个数交换位置 */
	/* 同时让产生随机数的范围减一，这样就不会产生相同的随机数 */
	int num[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int i=0;
	int x_num,y_num;
	for(i=0; i<16; i++)
	{
		//两个优化
		get_y_rand(&y_num);
		get_x_rand(&x_num,m);
		/* 这里最多查找16次 */
		if(sq_buf[num[x_num-1]]->name == NULL)
		{
			/* sq_buf[]是存放方阵节点的指针的数组 */
			sq_buf[num[x_num-1]]->name = calloc(1,15);
			sq_buf[num[x_num-1]]->num_y = y_num;
			strcpy(sq_buf[num[x_num-1]]->name,bmp_name[sq_buf[num[x_num-1]]->num_y]);
			printf("Random location : %d  and  Random pictures  %s \n",sq_buf[num[x_num-1]]->num,sq_buf[num[x_num-1]]->name);
			break;
		} 
		temp = num[x_num-1];
		num[x_num-1] = num[m-1];
		num[m-1] = temp;
		m--;			
	}
	show_all_bmp(bmp_head);
}
/* 通过遍历把每个方阵节点内的图片刷到显示屏上 */
void show_all_bmp(SQ_P bmp_head)
{
	SQ_P p = NULL;
	for(p=bmp_head;p!=NULL;p=p->next)
	{
		if(p->name!=NULL)
		{
			show_bmp_no_effect(p->x,p->y,p->name);
		}
	}
}

int flag1=1;
void *play_music(void *arg)
		{      	
			char buf[100] = {0};
			strcpy(buf, (char*)arg);
			
			char buf1[200] = {0};
			sprintf(buf1, "madplay %s", buf); //madplay ./2.mp3
			
			system(buf1);  //system("madplay ./1.mp3");
			flag1 = 0;
		}
		
void game_play()
{
	int x,y;  // 定义触摸获取的坐标值
	/* 捕获Ctrl+C 对程序的SIGINIT信号，先执行black_lcd()这个清屏函数，再响应信号 */
	signal(SIGINT,black_lcd);
	/* 打开三个用于存储进度的文件 */
	int fd = open("./1.txt",O_RDWR | O_CREAT);
	int fd4 = open("./2.txt",O_RDWR | O_CREAT);
	int fd5 = open("./score.txt",O_RDWR | O_CREAT);
	int fd6 = open("./Hscore.txt",O_RDWR | O_CREAT);
	int Hscore=0;
	
	/* 定义2048中的分数个十百千位 */
	int score_bits = 0,score_ten = 0,score_hundred = 0,score_thousand = 0;
	int Hscore_bits = 0,Hscore_ten = 0,Hscore_hundred = 0,Hscore_thousand = 0;
	
	int i,j,k; // 定义几个循环要用到的整数
	/* 定义四个用来标志的整数 */
	int flag = 1,flag1 = 0,flag2 = 0,flag3 = 0;

	/* 读取分数 */
	read(fd5,&score,4);
	read(fd6,&Hscore,4);
	
	
	/* 初始化方阵 */
	SQ_P bmp_head = square_init(); // from square.c
	
	/*从文件中读取数据存入sq方阵中,如果不是空文件返回1 */
	if(read_sq_from_file(bmp_head,fd4))
	{
		/* 文件不为空就给一些标志位赋值，用于后面的随机图片是否可以产生 */
		flag = 0;
		flag3 = 1;
	}
	/* 初始化一条中间方阵链表,用来保存2048上一步 */
	TEMP_P temp_head = init_temp_list(); // from square.c
	
	/* 为了提高产生随机值时查找插入的效率而定义的结构体数组 */
	SQ_P sq_buf[]={sq_1,sq_2,sq_3,sq_4,
				   sq_5,sq_6,sq_7,sq_8,
				   sq_9,sq_10,sq_11,sq_12,
				   sq_13,sq_14,sq_15,sq_16
				};
		/* 进入游戏 */				
			while(1)
			{
				printf("getin 2048");
				pthread_t thread;	//保存线程号
				pthread_create(&thread, NULL, play_music, "./2.mp3");
				pthread_detach(thread);
				while(flag1);
				printf("654321\n");
				//show_bmp_no_effect(0,0,"./game_pto/gameface.bmp");
				get_xy(&x,&y);
				if(x > 300*1.28 && x < 500*1,28 && y > 200*1.25 && y <380)
				{
					// 开始游戏
					//显示游戏界面
					show_bmp_no_effect(0,0,"./game_pto/g2048.bmp");
					show_all_bmp(bmp_head); // from game_func.c
					while(1)
					{
						/* 显示分数 */
						score_bits = score%10;
						score_ten = score/10%10;
						score_hundred = score/100%10;
						score_thousand = score/1000;
						show_bmp_no_effect(700,25,score_bmp[score_thousand]);
						show_bmp_no_effect(721,25,score_bmp[score_hundred]);
						show_bmp_no_effect(742,25,score_bmp[score_ten]);
						show_bmp_no_effect(763,25,score_bmp[score_bits]);
						
						Hscore_bits = Hscore%10;
						Hscore_ten = Hscore/10%10;
						Hscore_hundred = Hscore/100%10;
						Hscore_thousand = Hscore/1000;
						show_bmp_no_effect(700,50,score_bmp[Hscore_thousand]);
						show_bmp_no_effect(721,50,score_bmp[Hscore_hundred]);
						show_bmp_no_effect(742,50,score_bmp[Hscore_ten]);
						show_bmp_no_effect(763,50,score_bmp[Hscore_bits]);
						
						/* 1、判定是否可以随机出现2或者4 */
						if((flag == 1 || sq_is_null(bmp_head)))
						{
							printf("Can produce\n");
							usleep(200000);
							/* 随机显示2或4 */
							insert_sq(bmp_head,sq_buf); // from game_func.c
							/* 保存当前进度到文件内 */
							safe_now_sq_to_file(bmp_head,fd4,flag3); // // from game_func.c
						}
						/* 游戏胜利 */
						if(game_win(bmp_head))
						{
							show_bmp_no_effect(0,0,"./gamewin.bmp");
							/* 清空方阵 */
							set_sq(bmp_head); // from game_func.c
							score = 0;
							lseek(fd5,0,SEEK_SET);
							write(fd5,&score,4);
							sleep(2);	
							system("pkill madplay");
							break;
						}
						/* 游戏结束 */
						if(flag !=1 && sq_is_full(bmp_head))
						{
							show_bmp_no_effect(0,0,"./gameover.bmp");
							set_sq(bmp_head);
							safe_now_sq_to_file(bmp_head,fd4,flag3); // from game_func.c
							score = 0;
							lseek(fd5,0,SEEK_SET);
							write(fd5,&score,4);
							sleep(2);	
							system("pkill madplay");
							break;
						}
						//touch(&x,&y);
						get_xy(&x,&y);
						flag1 = 0;
						
						/* 左滑 */
						if(x<256 && x>0 && y>200 && y<400)
						{
							//1,所有图片左移
							//2,相同的两个合并，后面的再往左移动一格
							shift_to_left(bmp_head,&flag,temp_head); // from game_func.c
							flag1 = 1;
						}					
						/* 右滑 */
						if(x>600*1.28 && x<800*1.28 && y>160*1.25 && y<320*1.25)
						{
							//1,所有图片右移
							//2,相同的两个合并，后面的再往右移动一格
							shift_to_right(bmp_head,&flag,temp_head); // from game_func.c
							flag1 = 1;
						}
						/* 上滑 */
						if(x>200*1.28 && x<600*1.28 && y>0&& y<160*1.25)
						{
							shift_to_up(bmp_head,&flag,temp_head); // from game_func.c
							flag1 = 1;
						}
						/* 下滑 */
						if(x>200*1.28 && x<600*1.28 && y>320*1.25 && y<480*1.25)
						{
							shift_to_down(bmp_head,&flag,temp_head); // from game_func.c
							flag1 = 1;
						}
						/* 撤销一步 */
						/*
						if(x > 600*1.28 && x<800*1.28 && y >0 && y < 160*1.25)
						{
							/* 判定能不能撤回 
							if(!sq_is_null(bmp_head) && !temp_sq_is_null(temp_head)) // from game_func.c
							{
								/* 撤回的实现 
								undo_one_step(bmp_head,temp_head); // from game_func.c
							}
							flag1 = 0;
							score = temp_score;
							safe_now_sq_to_file(bmp_head,fd4,flag3);
						}*/
						/* 重新开始游戏 */
						if(x>640*1.28 && x<800*1.28 && y>320*1.25 && y <400*1.25)
						{
							score = 0;
							set_sq(bmp_head);
							flag = 1;
							sq_to_temp(bmp_head,temp_head); // from game_func.c
							temp_score = 0;
							show_bmp_no_effect(0,0,"./game_pto/g2048.bmp");
						}
						
						/* 返回上一界面 */
						if(x > 770 && x<800 && y>0 && y < 100)
						{
							show_bmp_no_effect(0, 0, "./choose.bmp");
							//显示三个选择功能的图片
							printf("choose app");

							get_xy(&x, &y);

							if (533 < x && x < 800)
							{
								int flag_home;
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
								printf("getin lanya");
								lanyaplay();
							}
						}
						/* 为了是否能产生随机图片而改变标志位 */
						if(flag1 == 0)
						{
							flag = 0;
						}
						lseek(fd5,0,SEEK_SET);
						/* 每次都存储一次记录 */
						write(fd5,&score,4);
						if (score>Hscore)
						{
							Hscore=score;
							lseek(fd6,0,SEEK_SET);
						/* 每次都存储一次记录 */
							write(fd6,&Hscore,4);
						}
					}
				}
				if(x > 640*1.28 && x <800*1.28  && y > 400*1.25 && y < 480*1.25)
				{
					//show_bmp_no_effect(0,0,"./pictures/bg.bmp");
					//退出游戏，回到主页面
					system("pkill madplay");
					break;
				}
		
		/* 进入驱动界面 */
		/*
		if(x > 70 && x < 260 && y > 369 && y < 441)
		{
			dirve_led_beff(&x1,&x2,&x,&y,&y1,&y2,fd2,fd3); // from dirve.c
		}		*/
		
		/* 退出程序 */
		/*
		if(x > 0 && x <160 *1.28 && y > 400*1.25 && y < 480*1.25)
		{
			show_bmp_no_effect(0,0,"./game_pto/gameface.bmp");
			//black_lcd(); // from show_bmp.c
		} */
	}
	
	//释放堆空间
	//free_heap(head,sq_buf,temp_head);
	//bmp_head = NULL;
	
	close(fd);
	close(fd4);
}