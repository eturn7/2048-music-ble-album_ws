/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : getpto.c
-  Author  :  xiongzh		Date  :  2019-7-31
-  Description  : 
				  1、封装了读取目录内的相片的函数
				   
-  Others  :  包含了一些所需的头文件
-  Function List  :  
   1. get_pto()  : 获取相片 
*************************************************************/
#include "getpto.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "inlist.h"
#include "list.h"

/***********************************************************
-  Description  :  通过递归获取目录内的相册并插入到链表
-  Calls  :  opendir()、calloc()、readdir()、
			 closedir()、snprintf()、inlist.c->inlist()
-  Iuput arg :  1、储存相册链表的头指针 2、目录名的地址
-  Return  :  返回新的头指针
-  Others  :  none
*************************************************************/
PTO get_pto(PTO head,char **argv)
{
	/* 打开目录文件 */
	DIR *bmp_dir = opendir(*argv);
	struct dirent *dir = calloc(1,sizeof(struct dirent)); // 定义目录结构体
	int i=0;
	int j=0;
	//char **pto1=malloc(100);
	while(1)
	{
		/* 读取目录内的文件 */
		dir = readdir(bmp_dir);
		if(dir == NULL)
		{
			/* 读完了或者出错跳出循环 */	
			break;
		}
		if(i > 1)
		{
			/* 把读取的目录项放入链表 */
			if(dir->d_type == DT_DIR)
			{
				/* 递归之前先改变参数 */
				char *name=calloc(1,40);
				snprintf(name,40,"%s%s/",*argv,dir->d_name);
				get_pto(head,&name);
			}
			else
			{
				/* 插入链表 */
				head = inlist(head,dir->d_name,*argv);
			}
		}
		i++;
	}
	closedir(bmp_dir);
	return head;
}