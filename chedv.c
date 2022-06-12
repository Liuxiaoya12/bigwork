//字符设备
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ctype.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
//计时器
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/jiffies.h>
//信号量
//#include<linux/semaphore.h>
//线程
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/io.h>
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
#define THREAD1 1
#define THREAD2 2


static int major =237;
static int minor=0;
static dev_t devno;
static struct class *cls;
static struct device *test_device;

struct task_struct * task_1 =NULL;//定义线程
struct task_struct * task_2=NULL;

//struct semaphore sem_1;//定义信号量
//struct semaphore sem_2;

struct timer_list mytimer_1;
struct timer_list mytimer_2;//定义计数器

int i1=1;
int i2=1;
static void kernel_thread_1(struct timer_list *timer)
{	

	printk("thread1   num1:%d\n",i1);
	i1++;
	
	if(i1==20)
		{
			i1=1;
			printk("num1 set 1.\n");
		}
	mod_timer(&mytimer_1,jiffies+HZ);
}

static void kernel_thread_2(struct timer_list *timer)
{	

	printk("thread2   num2:%d\n",i2);
	i2++;
	
	if(i2==10)
		{
			i2=1;
			printk("num2 set 1.\n");
		}
	mod_timer(&mytimer_2,jiffies+2*HZ);
}


static int thread_1(void *data)
{
	init_timer_key(&mytimer_1,&kernel_thread_1,TIMER_DEFERRABLE,"mytimer1",NULL);
	mytimer_1.function=kernel_thread_1;
	printk("计数器1初始化完成\n");
	add_timer(&mytimer_1);
	return 0;
}

static int thread_2(void *data)
{
	init_timer_key(&mytimer_2,&kernel_thread_2,TIMER_DEFERRABLE,"mytimer1",NULL);
	mytimer_2.function=kernel_thread_2;
	printk("计数器2初始化完成\n");
	add_timer(&mytimer_2);
	return 0;
}


static int chedv_open(struct inode *inode,struct file *filep)
{
printk("chedv_open \n");
return 0;
}

static int chedv_release(struct inode *inode,struct file *filep)
{
printk("chedv_release \n");
return 0;
}

static long chedv_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
	printk("输入操作数为：%d \n",cmd);
	switch(cmd){
				case START_1:
				
					task_1=kthread_create(&thread_1,NULL,"TASK_THREAD_1");
					if(!IS_ERR(task_1))
						wake_up_process(task_1);
					printk("thread1唤醒成功\n");
					printk("thread1开始\n");
					break;
				case START2:
				
					task_2=kthread_create(&thread_2,NULL,"TASK_THREAD_2");
					if(!IS_ERR(task_2))
						wake_up_process(task_2);
					printk("thread2唤醒成功\n");
					printk("thread2开始\n");
					break;
				case PAUSE_1:
					
					del_timer(&mytimer_1);
					printk("暂停thread1\n");
					break;
				case PAUSE_2:	
					
					del_timer(&mytimer_2);
					printk("暂停thread2\n");
					break;
				case CONTINUE_1:
					
					add_timer(&mytimer_1);
					printk("继续thread1\n");
					break;
				case CONTINUE_2:	
					
					add_timer(&mytimer_2);
					printk("继续thread2\n");
					break;
				case STOP_1:
					del_timer(&mytimer_1);
					printk("停止thread1\n");
					break;
					
				case STOP_2:				
					del_timer(&mytimer_2);
					printk("停止thread2\n");
					break;
				case TH1_TH2_START:
					task_1=kthread_create(&thread_1,NULL,"TASK_THREAD_1");
					task_2=kthread_create(&thread_2,NULL,"TASK_THREAD_2");
					if(!IS_ERR(task_1))
						wake_up_process(task_1);
					if(!IS_ERR(task_2))
						wake_up_process(task_2);
					printk("thread1,thread2全部开始");
					break;
				case TH1_TH2_PAUSE:
					
					del_timer(&mytimer_1);
					del_timer(&mytimer_2);
					printk("全部暂停\n");
					break;
					
				case TH1_TH2_CONTINUE:
					add_timer(&mytimer_1);
					add_timer(&mytimer_2);
					printk("全部继续\n");
					break;

				case TH1_TH2_STOP:
					del_timer(&mytimer_1);
					del_timer(&mytimer_2);
					printk("全部停止\n");
					break;
				default:
					printk("输入错误");
				}
				return 0;
					
}
static struct file_operations chedv_ops=
{
.owner=THIS_MODULE,
.open=chedv_open,
.release=chedv_release,
.unlocked_ioctl=chedv_ioctl,
};
static int chedv_init(void){
int ret;
printk("hello_init \n");
devno=MKDEV(major,minor);

ret=register_chrdev(major,"chedv",&chedv_ops);
cls=class_create(THIS_MODULE,"myclass");
if(IS_ERR(cls)){
unregister_chrdev(major,"chedv");
return -EBUSY;
}
test_device=device_create(cls,NULL,devno,NULL,"chedv");//mknod/dev/chedv
if(IS_ERR(test_device)){
class_destroy(cls);
unregister_chrdev(major,"chedv");
return -EBUSY;
}
return 0;
}
static void chedv_exit(void){
del_timer(&mytimer_1);
del_timer(&mytimer_2);
device_destroy(cls,devno);
class_destroy(cls);
unregister_chrdev(major,"chedv");
printk("chedv_exit\n");
}
MODULE_LICENSE("GPL");
module_init(chedv_init); 
module_exit(chedv_exit);

