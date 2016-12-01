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
#include "cmd.h"

#define BUF_SIZE 96

/***global***/
extern STATUS *status;					//状态链表
extern sem_t heartdet_sem;			//检测心跳信号量
extern REG_HEAD *qsend;				//发送队列
extern sem_t send_sem;				//发送用信号量
/***********/

void *command(void *arg){
	int option;
	help();
	for(;;){
		scanf("%d",&option);
		switch(option){
			case 0:
				help();
				break;
			case 1:
				query_apid();
				break;
			case 2:
				update_one();
				break;
			case 3:
				update_all();
				break;
			default:
				break;
		}
	}
	return NULL;
}


void help(){
	printf("please enter your option:\n");
	printf("0.help\n");
	printf("1.query the BS apid\n");
	printf("2.update one BS\n");
	printf("3.update all BS\n");
}

void query_apid(){
	int ret;
	ret = query_stalink(status);
	if(ret < 0)
		printf("please wait a moment!\n");
}

void update_one(){
	int apid;
	STATUS *ret = NULL;
	if(status == NULL){
		printf("the stalink is empty!\nplease wait a moment.\n");
		return;
	}
	printf("please enter the apid.\n");
	scanf("%d",&apid);
	ret = selectapid_stalink(status,apid);
	if(ret == NULL){
		printf("please enter a right apid.\n");
		return;
	}
	unsigned char *send_buf = malloc(BUF_SIZE);
	if(send_buf == NULL){
		perror("send malloc error");
		return;
	}
	memset(send_buf,0,BUF_SIZE);
	
	/*
		组基站查询包
	*/

	
	//添加到发送队列
	sem_wait(&send_sem);
	add_regque(qsend,send_buf,BUF_SIZE,ret->connfd,0);
	sem_post(&send_sem);	
}

void update_all(){
	if(status == NULL){
		printf("the stalink is empty!\nplease wait a moment.\n");
		return;
	}
	STATUS *move = status;
	while(move != NULL){
		unsigned char *send_buf = malloc(BUF_SIZE);		//发送完成后会释放空间
		if(send_buf == NULL){
			perror("send malloc error");
			return;
		}
		memset(send_buf,0,BUF_SIZE);
		
		/*
			组基站查询包
		*/
		
		//添加到发送队列
		sem_wait(&send_sem);
		add_regque(qsend,send_buf,BUF_SIZE,move->connfd,0);
		sem_post(&send_sem);
		move = move->next;
	}
}
















