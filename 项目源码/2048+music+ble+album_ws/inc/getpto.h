/***********************************************************
-  Copyright (C), Ksd xiongzh
-  Filename  : getpto.h
-  Author  :  xiongzh		Date:  2019-8-1
-  Description  :  
				  1、对getpto.c文件内的函数进行声明
-  Others  :	none
-  Function List  :  
   1.  get_pto() ： 获取图片
*************************************************************/

#ifndef __GETPTO_H__
#define __GETPTO_H__
#include "list.h"

PTO get_pto(PTO head,char **argv);

#endif