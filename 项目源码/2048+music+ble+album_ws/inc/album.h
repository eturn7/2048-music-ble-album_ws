#ifndef __ALBUM_H__
#define __ALBUM_H__

//MAX_BMP_NUM 最多256 张图片，可自行修改
#define MAX_BMP_NUM 256

//数组 bmpPath 用来保存 合成后的图片路径
extern char bmpPath[MAX_BMP_NUM][MAX_BMP_NUM];

//curBmpNum 保存当前读取到的bmp图片的最大数量
extern unsigned int curBmpNum;

//curShowBmpNum 保存当前显示的图片在二维数组中的编号 --- 后续在重新进入该界面时可以显示编号所对应的图片
extern unsigned int curShowBmpNum; 

//函数 AlbumInterface 实现相册界面的显示
int AlbumInterface();

//函数 getBmpName 用来获取指定目录下的所有文件名 
int getBmpName(char * const dirPath);

//函数 changeShowBmp 实现对相册图片的切换
int changeShowBmp();

#endif