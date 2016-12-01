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

#include "../data_struct/data.h"
#include "../data_struct/data_struct.h"
#include "update.h"

/***global***/
extern REG_HEAD *update;
extern sem_t update_sem;
extern int update_flag;
extern int time_flag;
/***********/

void *deal_update(void *arg){
	printf("the update pthread is start!\n");
	REG *update_data = NULL;
	for(;;){
		if(update->count == 0)
			continue;
		printf("start update\n");
		
		sem_wait(&update_sem);
		update_data = out_regque(update);
		sem_post(&update_sem);		
		if(update_data == NULL){
			perror("update data get error!");
			continue;
		}
		/*
			分析反馈包的内容，判断基站状态，发送更新数据
		*/

		//开启超时
		start_timeout();
		while((update_flag != 1) || (time_flag != 1)){
		}
		if(update_flag== 1){
			printf("update succeed!\n");
		}
		else if(time_flag == 1){
			printf("update fault!\n");
			/*
				发送指令异常，断开连接
			*/
		}
	}
	return NULL;
}

void start_timeout(){
	pthread_t time_pid;
	pthread_create(&time_pid,0,timeout,NULL);
	pthread_detach(time_pid);
}

void *timeout(void *arg){
	struct timeval tempval;
	tempval.tv_sec = 60;
	tempval.tv_usec = 0;
	select(0,NULL,NULL,NULL,&tempval);
	time_flag = 1;
	return NULL;
}










