/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : inlist.c
-  Author  :  xiongzh		Date  :  2019-7-31
-  Description  : 
				  1、封装了将相册名插入链表的函数
				   
-  Others  :  包含了一些所需的头文件
-  Function List  :  
   1. inlist()  :  把相片名插入链表 
*************************************************************/
#include "inlist.h"
#include "list.h"
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>

/***********************************************************
-  Description  :  采用尾插法把相片名插入链表
-  Calls  :  snprintf()、malloc()、strcpy()
-  Iuput arg :  1、储存相册链表的头指针 2、相册名 3、当前路径名
-  Return  :  返回新的头指针
-  Others  :  none
*************************************************************/
PTO inlist(PTO head,char *d_name,char *argv)
{
	char name[40] = {};
	/* 把当前路径和文件名拼成真正相片路径 */
	snprintf(name,40,"%s%s",argv,d_name);
	
	/* 创建一个新节点并把相册名拷贝给新节点内的name */
	PTO new_node = malloc(sizeof(struct pto_node));
	strcpy(new_node->pname,name);
	
	/* 如果是空链表 */
	if(head == NULL)
	{
		new_node->next = new_node;
		new_node->bef = new_node;
		head = new_node;
	}
	
	else
	{
		/* 尾插的方法 */
		new_node->next = head;
		new_node->bef = head->bef;
		head->bef->next = new_node;
		head->bef = new_node;
		
	}
	return head;
}