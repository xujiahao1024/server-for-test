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
#include "heart.h"

/***global***/
extern REG_HEAD *heart;				//传递心跳队列
extern sem_t heart_sem;				//传递心跳信号量
extern STATUS *status;				//状态链表
extern sem_t heartdet_sem;			//检测心跳信号量
/***********/

void *deal_heartdet(void *arg){
	printf("the heartdet pthread is start!\n");
	struct timeval tempval;
	int ret;
	for(;;){
		if(status == NULL)
			continue;
		tempval.tv_sec = 60;
		tempval.tv_usec = 0;
		select(0,NULL,NULL,NULL,&tempval);
		
		sem_wait(&heartdet_sem);
		ret = alterheart_stalink(status);
		sem_post(&heartdet_sem);
		if(ret < 0)
			perror("心跳位增加出错！");
		
		sem_wait(&heartdet_sem);
		ret = detection_stalink(status);
		sem_post(&heartdet_sem);
		if(ret < 0)
			perror("连接断开出错！");		
	}
	return NULL;
}

void *deal_heartzero(void *arg){
	printf("the heartzero pthread is start!\n");
	REG *heart_data = NULL;
	int ret;
	for(;;){
		if(heart->count == 0){
			continue;
		}
		printf("recv heart beat!\n");
		sem_wait(&heart_sem);
		heart_data = out_regque(heart);
		sem_post(&heart_sem);
		
		sem_wait(&heartdet_sem);
		ret = bzero_stalink(status,heart_data->connfd);
		sem_post(&heartdet_sem);
		if(ret < 0)
			perror("heart bzero errro!");
		
		free(heart_data);
	}
	return NULL;
}