#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include "./server/server.h"
#include "./data_struct/data.h"
#include "./server/deal.h"
#include "./server/register.h"
#include "./server/send.h"
#include "./server/heart.h"
#include "./server/cmd.h"
#include "./server/update.h"

/***global***/
unsigned int apid ;
REG_HEAD *reg;				//传递注册队列
sem_t reg_sem;				//注册用信号量
REG_HEAD *qsend;			//发送队列
sem_t send_sem;				//发送用信号量
REG_HEAD *heart;			//传递心跳队列
sem_t heart_sem;			//传递心跳信号量
STATUS *status;				//状态链表
sem_t heartdet_sem;			//检测心跳信号量
REG_HEAD *update;
sem_t update_sem;
int update_flag;
int time_flag;
/***********/

//初始化信号量
void init_sem()
{
	int ret;
	ret = sem_init(&reg_sem,0,1);
	if(ret < 0){
		perror("reg_sem init!");
		exit(-1);	
	}
	
	ret = sem_init(&send_sem,0,1);
	if(ret < 0){
		perror("send_sem init!");
		exit(-1);	
	}
	
	ret = sem_init(&heart_sem,0,1);
	if(ret < 0){
		perror("heart_sem init!");
		exit(-1);	
	}
	
	ret = sem_init(&heartdet_sem,0,1);
	if(ret < 0){
		perror("heartdet_sem init!");
		exit(-1);	
	}

	ret = sem_init(&update_sem,0,1);
	if(ret < 0){
		perror("update_sem init!");
		exit(-1);	
	}
}

//初始化各种队列
void init_work_queue()
{
	int ret;
	ret = init_regque(&reg);
	if(ret < 0){
		perror("init register");
		exit(-1);
	}
	
	ret = init_regque(&qsend);
	if(ret < 0){
		perror("init send");
		exit(-1);
	}
	
	ret = init_regque(&heart);
	if(ret < 0){
		perror("init heart");
		exit(-1);
	}

	ret = init_regque(&update);
	if(ret < 0){
		perror("init update");
		exit(-1);
	}
	
	status = NULL;
}

//初始化工作线程
void init_work_thread(DATA *data)
{
	//命令行线程
	pthread_create(&(data->command_pid),0,command,(void *)data);
	//注册线程
	pthread_create(&(data->reg_pid),0,deal_register,(void *)data);
	//发送线程
	pthread_create(&(data->send_pid),0,deal_send,(void *)data);
	//心跳检测线程
	pthread_create(&(data->heartdet_pid),0,deal_heartdet,(void *)data);
	//心跳清零线程
	pthread_create(&(data->heartzero_pid),0,deal_heartzero,(void *)data);
	//升级线程
	pthread_create(&(data->update_pid),0,deal_update,(void *)data);
	
	
	pthread_detach(data->reg_pid);
	pthread_detach(data->send_pid);
	pthread_detach(data->heartdet_pid);
	pthread_detach(data->heartzero_pid);
	pthread_detach(data->command_pid);
	pthread_detach(data->update_pid);
}

int main(int argc,char *argv[])
{
	DATA data;
	bzero(&data,sizeof(DATA));
	
	//初始化信号量
	init_sem();
	
	//初始化各种队列
	init_work_queue();
	
	//初始化工作线程
	init_work_thread(&data);
	
	//启动服务器
	start_server(&data);
	
	return 0;
}