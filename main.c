#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include "server.h"
#include "data.h"
#include "deal.h"

/***global***/
QUEUE_head *reg;
sem_t reg_sem;
/***********/

void init_queue_sem(DATA *data){
	int ret = -1;
	ret = sem_init(&reg_sem,0,1);
	if(ret < 0){
		perror("reg_sem init!");
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
	
}

void start_work_thread(DATA *data){
	pthread_create(&(data->reg_pid),0,deal_register,(void *)data);
	
	pthread_detach(data->reg_pid);
}

int main(int argc,char *argv[]){
	DATA data;
	bzero(&data,sizeof(data));
	
	init_queue_sem(&data);
	
	init_work_queue(&data);

	start_work_thread(&data);
	
	//start_cmd_thread();
	
	start_server(&data);
	return 0;
}