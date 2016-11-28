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

#include "data.h"

void help(){
	printf("please enter number for your option!\n");
	printf("0.查看帮助.\n");
	printf("1.查看当前连入基站.\n");
	printf("2.更新基站.\n");
}

void *del_cmd(void *arg){
	int option = 0;
	help();
	while(1){
		scanf("%d",&option);
		switch(option){
			case 0:
				help();
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
		}
	}
	return NULL;
}

void start_cmd_thread(DATA *data){
	pthread_create(&(data->cmd_pid),NULL,del_cmd,(void *)data);
	
	pthread_detach(data->cmd_pid);
}
