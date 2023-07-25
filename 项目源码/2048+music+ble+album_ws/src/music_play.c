/*
音乐播放界面

*/
#include "album.h"
#include <pthread.h>
#include "xiangce.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "get_xy.h"
#include "music_play.h"
#include "show_bmp.h"
#include "lcd.h"
#include "get_xy.h"

int x, y;

int musicplay()
{
    showpicture("./music.bmp");
    
    /* 返回上一界面 */
    get_xy(&x, &y);

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
								printf("getin 2048");
								game_play();
							}
						}
    return 0;
}