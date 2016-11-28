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

#include "server.h"
#include "data.h"
#include "deal.h"
#include "cmd_thread.h"

/***global***/
QUEUE_head *reg;			//注册队列
QUEUE_head *status;			//状态队列
sem_t reg_sem;				//注册用信号量
sem_t sta_sem;				//状态用信号量
/***********/

void init_queue_sem(DATA *data){
	int ret = -1;
	ret = sem_init(&reg_sem,0,1);
	if(ret < 0){
		perror("reg_sem init!");
		exit(0);
	}
	
	ret = sem_init(&sta_sem,0,1);
	if(ret < 0){
		perror("sta_sem init!");
		exit(0);
	}
}

void init_work_queue(DATA *data){
	int ret = -1;
	sem_wait(&reg_sem);
	ret = init_que(&reg);
	sem_post(&reg_sem);
	if(ret < 0)
		perror("init register!");
	
	sem_wait(&sta_sem);
	ret = init_que(&status);
	sem_post(&sta_sem);
	if(ret < 0)
		perror("init status!");	
	
}

void start_work_thread(DATA *data){
	pthread_create(&(data->reg_pid),0,deal_register,(void *)data);
	
	pthread_detach(data->reg_pid);
}

int main(int argc,char *argv[]){
	DATA data;
	bzero(&data,sizeof(data));
	
	init_queue_sem(&data);
	
	//初始化工作队列
	init_work_queue(&data);

	//启动工作线程
	start_work_thread(&data);
	
	//启动命令线程
	start_cmd_thread(&data);
	
	//启动服务器
	start_server(&data);
	
	return 0;
}