//防止头文件重复包含
#ifndef __LCD_H__
#define __LCD_H__

// 对全局变量进行全局声明
extern int fd_lcd;
extern int *plcd;

// 对函数进行声明
void LCD_Draw_Point(int x, int y,  int color);

int LCD_Init();

int shuapin();

void LCD_Uninit();

void Draw_Word();

int touch();

int showpicture();
#endif
