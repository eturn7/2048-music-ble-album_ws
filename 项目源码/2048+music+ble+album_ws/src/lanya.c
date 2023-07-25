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
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>



#define ON  0
#define OFF 1


#define TEST_MAGIC 'x'                    //定义幻数
#define LED1 _IO(TEST_MAGIC, 0)              
#define LED2 _IO(TEST_MAGIC, 1)
#define LED3 _IO(TEST_MAGIC, 2)
#define LED4 _IO(TEST_MAGIC, 3) 
#define LED_ON  	0	//灯亮
#define LED_OFF		1   //灯灭




int x, y;

int init_tty(int fd)
{
	struct termios old_flags,new_flags;
	bzero(&new_flags,sizeof(new_flags));
	
	//1. 获取旧的属性
	tcgetattr(fd,&old_flags);
	
	//2. 设置原始模式
	cfmakeraw(&new_flags);
	
	//3. 激活本地连接CLOCAL与接收使能CREAD的选项
	new_flags.c_cflag |= CLOCAL | CREAD; 
	
	//4. 设置波特率
	cfsetispeed(&new_flags, B9600); 
	cfsetospeed(&new_flags, B9600);
	
	//5. 设置数据位为8位
	new_flags.c_cflag &= ~CSIZE; //清空原有的数据位
	new_flags.c_cflag |= CS8;
	
	//6. 设置奇偶检验位
	new_flags.c_cflag &= ~PARENB;
	
	//7. 设置一位停止位
	new_flags.c_cflag &= ~CSTOPB;
	
	//8. 设置等待时间，最少接收字符个数
	new_flags.c_cc[VTIME] = 0;
	new_flags.c_cc[VMIN] = 1;
	
	//9. 清空串口缓冲区
	tcflush(fd, TCIFLUSH);
	
	//10. 设置串口的属性到文件中
	tcsetattr(fd, TCSANOW, &new_flags);
	
	return 0;
}

int lanyaplay()
{
    showpicture("./lanya_choose.bmp");

    //1. 访问串口2
	int fd = open("/dev/ttySAC2",O_RDWR | O_NOCTTY);
	
	//2. 配置串口2参数
	init_tty(fd);
	
	char wbuf[10]="hello";
	char ibuf[10];
	
	while(1)
	{
		write(fd,wbuf,5);
		sleep(1);
		read(fd,ibuf,sizeof(ibuf));
		printf("%s",ibuf);
		if(ibuf[0]=='1'){
			printf("buz");

			// 打开蜂鸣器
			int fdb = open("/dev/beep", O_RDWR);
			if(fdb == -1)
			{
				perror("open:");
				exit(0);
			}
			// 使能蜂鸣器
			ioctl(fdb, ON, 1);
			sleep(5);
			// 关闭蜂鸣器
			ioctl(fdb, OFF, 1);


			close(fdb);

		}
		if(ibuf[0]=='2'){
		printf("led");
		//2.访问驱动设备
		int fdx = open("/dev/Led",O_RDWR);
	
		ioctl(fdx,LED1,LED_OFF);
		ioctl(fdx,LED2,LED_OFF);
		ioctl(fdx,LED3,LED_OFF);
		ioctl(fdx,LED4,LED_OFF);
		//3.控制设备状态
	
			ioctl(fdx,LED1,LED_ON);
			sleep(1);
			ioctl(fdx,LED1,LED_OFF);
			sleep(1);
			ioctl(fdx,LED2,LED_ON);
			sleep(1);
			ioctl(fdx,LED2,LED_OFF);
			sleep(1);
			ioctl(fdx,LED3,LED_ON);
			sleep(1);
			ioctl(fdx,LED3,LED_OFF);
			sleep(1);
			ioctl(fdx,LED4,LED_ON);
			sleep(1);
			ioctl(fdx,LED4,LED_OFF);
			sleep(1);
		//4. 关闭设备文件
		close(fdx);
		}

		bzero(ibuf,sizeof(ibuf));
		sleep(1);

		

	}
	close(fd);


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

