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
extern REG_HEAD *heart;				//������������
extern sem_t heart_sem;				//���������ź���
extern STATUS *status;				//״̬����
extern sem_t heartdet_sem;			//��������ź���
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
			perror("����λ���ӳ���");
		
		sem_wait(&heartdet_sem);
		ret = detection_stalink(status);
		sem_post(&heartdet_sem);
		if(ret < 0)
			perror("���ӶϿ�����");		
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