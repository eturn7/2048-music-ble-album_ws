#include "lcd.h"
#include "album.h"
#include <pthread.h>
#include "xiangce.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "get_xy.h"
#include "music_play.h"
#include "game_func.h"
int flag = 1;
void *play_music1(void *arg)
{
    char buf[100] = {0};
    strcpy(buf, (char *)arg);

    char buf1[200] = {0};
    sprintf(buf1, "madplay %s", buf); //madplay ./1.mp3

    system(buf1); //system("madplay ./1.mp3");
    flag = 0;
}

int xiangce(int flag_home)
{
    int x, y;
    LCD_Init();
    int position = 0;

    getBmpName("./picture/pictures");
    touch();
    showpicture(bmpPath[position]);
    printf("xiangce");
    while (1)
    {
        pthread_t thread; //保存线程号
        pthread_create(&thread, NULL, play_music1, "./1.mp3");
        pthread_detach(thread);
        printf("%d", &position);
        int move = touch();
        if (move == 0)
            break;
        if ((position + move < curBmpNum) && (position + move >= 0))
        {
            position += move;
            showpicture(bmpPath[position]);
            get_xy(&x,&y);
            //返回上一级
            if (x > 0 && x < 160 * 1.28 && y > 400 * 1.28 && y < 480 * 1.25)
            {
                showpicture("./picture/7.bmp");
                //flag = 0;
                system("pkill madplay"); //system("killall madplay")
                break;
            }
            //返回主界面
    /* 返回上一界面 */
    get_xy(&x,&y);
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
        }
    }
    LCD_Uninit();
    return 0;
}