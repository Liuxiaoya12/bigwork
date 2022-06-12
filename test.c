#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#define CHAR_DEVICE "/dev/chedv"

#define START_1       1
#define START2       12
#define PAUSE_1       3
#define PAUSE_2       4
#define CONTINUE_1    5
#define CONTINUE_2    6
#define STOP_1         7
#define STOP_2         8
#define TH1_TH2_START    9
#define TH1_TH2_PAUSE    10
#define TH1_TH2_STOP     11
#define TH1_TH2_CONTINUE 13
int main(int argc,char ** argv)
{
	int fd;
	int choice;
	fd = open(CHAR_DEVICE,O_RDWR);
	if(fd<0)
	{
		printf("open %s error\n",CHAR_DEVICE);
		return 0;
	}
	 while(1)
    {
	
	      puts("please enter you want operation:");
	      puts("1: thread1 print start 12:thread2 print start 9: thread1 and thread2 print start");
	      puts("3: thread1 print pause4: thread2 print pause 10: thread1 and thread2 print pause");
	      puts("5: thread1 print continue 6: thread2 print continue 13:thread1 and thread2 print continue");
	      puts("7: thread1 print stop 8: thread2 print stop 11: thread1 and thread2 print stop");
	      puts("请输入：");
	      scanf("%d",&choice);
	      getchar();
	      printf("choice = %d\n",choice);
	      switch(choice){
	      case 1:
		      printf("thread 1 start\n");
		      ioctl(fd,START_1,NULL);
		      break;
	      case 12:
		      printf("thread 2 start\n");
		      ioctl(fd,START2,NULL);
		      break;
	      case 3:
		      printf("thread 1 pause\n");
		      ioctl(fd,PAUSE_1,NULL);
		      break;
	      case 4:
		      printf("thread 2 PAUSE\n");
		      ioctl(fd,PAUSE_2,NULL);
		      break;
	      case 5:
		      printf("thread 1 continue\n");
		      ioctl(fd,CONTINUE_1,NULL);
		      break;
	      case 6:
		      printf("thread 2 countinue\n");
		      ioctl(fd,CONTINUE_2,NULL);
		      break;
	      case 7:
		      printf("thread 1 stop\n");
		      ioctl(fd,STOP_1,NULL);
		      break;
	      case 8:
		      printf("thread 2 stop\n");
		      ioctl(fd,STOP_2,NULL);
		      break;
	      case 9:
		      printf("thread 1,2 start\n");
		      ioctl(fd,TH1_TH2_START,NULL);
		      break;
	      case 10:
		      printf("thread 1,2 pause\n");
		      ioctl(fd,TH1_TH2_PAUSE,NULL);
		      break;
	      case 11:
		      printf("thread 1,2 stop\n");
		      ioctl(fd,TH1_TH2_STOP,NULL);
		      break;
	      case 13:
		      printf("thread 1,2 continue\n");
		      ioctl(fd,TH1_TH2_CONTINUE,NULL);
		      break;
		    
}}
	close(fd);
	return 0;
}
