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

#include "queue.h"
#include "data.h"

#define DATA_SIZE 1024
extern int errno;

void *deal_register(void *arg){
	
	return NULL;
}

//处理各种tcp链接异常
void deal_with_errno(int connfd){
	if(errno == 104){
		printf("the client disconnect\n");
		close(connfd);
	}
	else if(errno == ETIMEDOUT | errno == EHOSTUNREACH){
		printf("the client is breakdown!");
		close(connfd);
	}
	else{
		perror("recv error");
		pthread_exit(NULL);
	}
}

//分析接收的数据，并扔给各个业务线程
void handle_recv(char *buf,int len,int connfd){
	printf("%s\n",buf);
	switch(buf[3]){
		case 1:
			break;
		case 3:
			break;
		default:
			break;
	}
}

//处理链接数据
void *handle_connect(void *arg){
	int connfd = (int)arg;
	int ret;
	int len;
	char buf[DATA_SIZE] = "";
	for(;;){
		bzero(buf,DATA_SIZE);
		len = recv(connfd,buf,DATA_SIZE,0);
		if(len == 0){
			printf("the client disconnect!\n");
			close(connfd);
			pthread_exit(0);
		}
		else if(len < 0){
			deal_with_errno(connfd);
		}
		handle_recv(buf,len,connfd);		
	}
	return NULL;
}