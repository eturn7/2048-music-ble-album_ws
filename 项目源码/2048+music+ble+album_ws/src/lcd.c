#include "lcd.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include<math.h>
#include <linux/input.h>
#include "music_play.h"

#define MAP_SIZE 800*480*4

int fd_lcd = 0;
int *plcd = NULL;
//����
void LCD_Draw_Point(int x, int y,  int color)
{
	if (x >= 0 && x < 800 && y >= 0 && y < 480)
	{
		*(plcd + 800*y + x) = color;
	}
}
//��Ļ��ʼ��
int LCD_Init()
{
	fd_lcd = open("/dev/fb0", O_RDWR);
	if(fd_lcd == -1)
	{
		perror("open lcd fail");
		return -1;
	}

	plcd = (int *)mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_lcd, 0);
	if(plcd == NULL)
	{
		perror("MMAP fail");
		return -1;
	}
}
//ˢ��
int shuapin()
{
	for(int i=0; i<480; i++)
	{
		for(int j=0; j<800; j++)
		{
			LCD_Draw_Point(j, i, 0x00ffffff);
		}
	}
}
//�����Ļ��ʼ��

void LCD_Uninit()
{
	munmap(plcd, MAP_SIZE);
	close(fd_lcd);
}

//����16*16��ʽ����ģ
void Draw_Word(int x0,int y0,char word[])
{

	for(int i=0; i<32; i++)
    //for(int i=0; i<16; i++) //��16��
    {
		for(int k=0; k<4; k++)
        //for(int k=0; k<2; k++)  //һ��2���ֽ�
        {
            for(int n=0; n<8; n++)  //����һ���ֽ�
            {
             
                if( word[i*4+k] & (1<<n) )
                    LCD_Draw_Point(x0+k*8+(8-n),y0+i,0x00ffffff);
            }
        }
    }

}
//��ʾͼƬ
int showpicture(char * path)
{
	
	char lcd_buf[800*480*4];  //lcd
	char bmp_buf[800*480*3];  //bmp
	
	//����ʾ��
	int lcd_fd = open("/dev/fb0",O_RDWR);
	if(lcd_fd < 0)
	{
		perror("open lcd failed");
		return -1;
	}
	
	//��bmp
	int bmp_fd = open(path,O_RDWR);
	if(bmp_fd < 0)
	{
		perror("open bmp failed");
		return -1;
	}
	int* p = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(p == NULL)
	{
		perror("mmap() failed");
		return -1;
	}
	//ƫ��54�ֽ�
	lseek(bmp_fd,54,SEEK_SET);
	//��ȡͼƬ����
	read(bmp_fd,bmp_buf,800*480*3);
	int buf[800*480];
	int i;
	for(i=0;i<800*480;i++)
	{
		buf[i] = bmp_buf[3*i+0] | bmp_buf[3*i+1]<<8 | bmp_buf[3*i+2]<<16;
					//b			|			g<<8		| 		r<<16
	}
	
	//�������ص�
	int spi_buf[800*480];
	int x,y;
	for(y=0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{
			*(p+ y*800+x) = buf[800*(479-y)+x];
		}
	}
	//�ر���ʾ����bmp
	close(lcd_fd);
	close(bmp_fd);
	
	return 0;
}
//����
int touch()
{
    struct input_event ev;
    int flag_lorr;
    int x=-1,y=-1;
	int touch_fd = open("/dev/input/event0", O_RDONLY);
		if(touch_fd < 0)
		{
			perror("fail");
			return -1;
		}
		while(1)
		{
			read(touch_fd, &ev, sizeof(ev));
			if(ev.type == EV_ABS && ev.code == ABS_X)
				x = ev.value;
			else if(ev.type == EV_ABS && ev.code == ABS_Y)
				y = ev.value;	

			if(ev.type == EV_KEY && ev.code == BTN_TOUCH)
				if(ev.value == 1)
					printf("press\n");
				else
				{
					printf("release\n");
					if(x/1.28<300&&y > 30*1.25 && y < 400 * 1.25)
						{
							flag_lorr=-1;
						}
					
					if(x/1.28>500&& y < 400 * 1.25)
					{
						flag_lorr=1;
					}
					if ((x/1.28>300)&&(x/1.28<500)&&y < 400 * 1.25)
					{
					 flag_lorr=0;
					}
					break;
				}
			
		}	
		int x1=x/1.28;
		int y1=y/1.25;
		printf("(%d,%d)\n",x1,y1);
		close(touch_fd);
	return flag_lorr;

}
/*
void drawbutton()
{
	//left
	for(int 440=0; i<470; i++)
	{
		for(int 50=0; j<150; j++)
		{
			LCD_Draw_Point(j, i, 0x00ffffff);
		}
	}
	//mid
	for(int 440=0; i<470; i++)
	{
		for(int =385; j<415; j++)
		{
			LCD_Draw_Point(j, i, 0x00ffffff);
		}
	}
	//right
	for(int 440=0; i<470; i++)
	{
		for(int =600; j<750; j++)
		{
			LCD_Draw_Point(j, i, 0x00ffffff);
		}
	}
	
}*/

