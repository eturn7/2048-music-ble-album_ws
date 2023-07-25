/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : list.h
-  Author  :  xiongzh		Date:  2019-8-1
-  Description  :  
				  1、定义了存储相片的结构体，并给其取了别名
-  Others  :	none
-  Function List  :  none
*************************************************************/
#ifndef __LIST_H__
#define __LIST_H__


typedef struct pto_node
{
	char pname[40];
	struct pto_node *next;
	struct pto_node *bef;
}pto,* PTO;

#endif