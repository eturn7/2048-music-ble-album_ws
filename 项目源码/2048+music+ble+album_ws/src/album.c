#include "album.h"
#include "lcd.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

char bmpPath[MAX_BMP_NUM][MAX_BMP_NUM] = {0};

unsigned int curBmpNum = 0;

unsigned int curShowBmpNum = 0;

int getBmpName(char * const dirPath)
{
	//打开一个目录 opendir
	DIR *p = NULL;
	p = opendir(dirPath);
	if(p == NULL)
	{
		perror("open dir fail");
		return -1;
	}
	//读取一个目录 readdir
	struct dirent *dirp = NULL;
	while(1)
	{
		dirp = readdir(p);
		if(NULL == dirp)
		{
			break;
			
		}
		if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
			continue;
			
		sprintf(bmpPath[curBmpNum++], "%s/%s", dirPath, dirp->d_name);		
		puts(bmpPath[curBmpNum-1]);
	}
	
	//关闭目录
	closedir(p);
}

int AlbumInterface()
{
	/*
		...
	*/
}

int changeShowBmp()
{
	/*
		...
	*/
}