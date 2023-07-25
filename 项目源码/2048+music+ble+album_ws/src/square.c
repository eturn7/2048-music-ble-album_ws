/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : squqre.c
-  Author  :  xiongzh		Date  :  2019-7-31
-  Description  : 
				  1、封装了初始化2048游戏的方阵的函数
				  2、封装了初始化2048中间变量链表的函数
-  Others  :  包含了一些所需的头文件
-  Function List  :  
   1.  init_temp()  ：单个中间链表节点初始化
   2.  init_temp_list())  : 整个个中间链表节点初始化
   3.  init_sq() : 单个方阵节点初始化
   4.  square_init()  : 整个方阵初始化
*************************************************************/

#include "square.h"
#include <stdlib.h>
#include <stdio.h>

/***********************************************************
-  Description  :  单个中间链表节点初始化
-  Calls  :  malloc()
-  Iuput :  none
-  Return  :  返回节点结构体指针
-  Others  :  none
*************************************************************/
TEMP_P init_temp()
{
	TEMP_P new_node = malloc(sizeof(TEMP));
	new_node->num_y = -1;
	new_node->name = NULL;
	new_node->next = NULL;
	return new_node;
}

/***********************************************************
-  Description  :  整个个中间链表节点初始化
-  Calls  :  malloc()
-  Iuput :  none
-  Return  :  返回整个链表的头指针
-  Others  :  none
*************************************************************/
TEMP_P init_temp_list()
{
	TEMP_P head = malloc(sizeof(TEMP));
	TEMP_P temp_1 = init_temp();
	TEMP_P temp_2 = init_temp();
	TEMP_P temp_3 = init_temp();
	TEMP_P temp_4 = init_temp();
	TEMP_P temp_5 = init_temp();
	TEMP_P temp_6 = init_temp();
	TEMP_P temp_7 = init_temp();
	TEMP_P temp_8 = init_temp();
	TEMP_P temp_9 = init_temp();
	TEMP_P temp_10 = init_temp();
	TEMP_P temp_11 = init_temp();
	TEMP_P temp_12 = init_temp();
	TEMP_P temp_13 = init_temp();
	TEMP_P temp_14 = init_temp();
	TEMP_P temp_15 = init_temp();
	TEMP_P temp_16 = init_temp();
	temp_1->next = temp_2;
	temp_2->next = temp_3;
	temp_3->next = temp_4;
	temp_4->next = temp_5;
	temp_5->next = temp_6;
	temp_6->next = temp_7;
	temp_7->next = temp_8;
	temp_8->next = temp_9;
	temp_9->next = temp_10;
	temp_10->next = temp_11;
	temp_11->next = temp_12;
	temp_12->next = temp_13;
	temp_13->next = temp_14;
	temp_14->next = temp_15;
	temp_15->next = temp_16;
	temp_16->next = NULL;
	head = temp_1;
	
	return head;
}

/***********************************************************
-  Description  :  单个个方阵节点初始化
-  Calls  :  malloc()
-  Iuput :  none
-  Return  :  返回单个方阵节点指针
-  Others  :  none
*************************************************************/
SQ_P init_sq(int num,int x,int y)
{
	SQ_P new_list = malloc(sizeof(SQ));
	new_list->num = num;
	new_list->num_y = -1;
	new_list->x = x;
	new_list->y = y;
	new_list->up = NULL;
	new_list->down = NULL;
	new_list->next = NULL;
	new_list->bef = NULL;
	new_list->name = NULL;
	return new_list;
}

/***********************************************************
-  Description  :  整个方阵初始化
-  Calls  :  init_sq()
-  Iuput :  none
-  Return  :  返回整个方阵的头指针
-  Others  :  none
*************************************************************/
SQ_P square_init()
{
	SQ_P head = NULL;
	sq_1 = init_sq(1,237,60);
	sq_2 = init_sq(2,327,60);
	sq_3 = init_sq(3,417,60);
	sq_4 = init_sq(4,507,60);
	sq_5 = init_sq(5,237,153);
	sq_6 = init_sq(6,327,153);
	sq_7 = init_sq(7,417,153);
	sq_8 = init_sq(8,507,153);
	sq_9 = init_sq(9,237,245);
	sq_10 = init_sq(10,327,245);
	sq_11 = init_sq(11,417,245);
	sq_12 = init_sq(12,507,245);
	sq_13 = init_sq(13,237,339);
	sq_14 = init_sq(14,327,339);
	sq_15 = init_sq(15,417,339);
	sq_16 = init_sq(16,507,339);	
	head = sq_1;
	sq_1->next = sq_2;
	sq_1->down = sq_5;

	sq_2->next = sq_3;
	sq_2->bef = sq_1;
	sq_2->down = sq_6;

	sq_3->next = sq_4;
	sq_3->bef = sq_2;
	sq_3->down = sq_7;

	sq_4->bef = sq_3;
	sq_4->next = sq_5;
	sq_4->down = sq_8;

	sq_5->up = sq_1;
	sq_5->down = sq_9;
	sq_5->next = sq_6;

	sq_6->up = sq_2;
	sq_6->down = sq_10;
	sq_6->bef = sq_5;
	sq_6->next = sq_7;

	sq_7->up = sq_3;
	sq_7->down = sq_11;
	sq_7->bef = sq_6;
	sq_7->next = sq_8;

	sq_8->up = sq_4;
	sq_8->down = sq_12;
	sq_8->bef = sq_7;
	sq_8->next = sq_9;

	sq_9->up = sq_5;
	sq_9->down = sq_13;
	sq_9->next = sq_10;

	sq_10->up = sq_6;
	sq_10->down = sq_14;
	sq_10->bef = sq_9;
	sq_10->next = sq_11;

	sq_11->up = sq_7;
	sq_11->down = sq_15;
	sq_11->bef = sq_10;
	sq_11->next = sq_12;

	sq_12->up = sq_8;
	sq_12->down = sq_16;
	sq_12->bef = sq_11;
	sq_12->next = sq_13;

	sq_13->up = sq_9;
	sq_13->next = sq_14;

	sq_14->up = sq_10;
	sq_14->bef = sq_13;
	sq_14->next = sq_15;

	sq_15->up = sq_11;
	sq_15->bef = sq_14;
	sq_15->next = sq_16;

	sq_16->up = sq_12;
	sq_16->bef = sq_15;

	return head;	
}