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
#include "deal.h"

#define DATA_SIZE 1024
#define MAXFD 1024

/***global***/
extern int errno;
extern REG_HEAD *reg;				//注册队列
extern sem_t reg_sem;			//注册用信号量
extern REG_HEAD *heart;			//传递心跳队列
extern sem_t heart_sem;			//传递心跳信号量
extern REG_HEAD *update;
extern sem_t update_sem;
extern int update_flag;
extern STATUS *status;
extern sem_t heartdet_sem;
/***********/

//处理链接数据
void *handle_connect(void *arg){
	int connfd = (int)arg;
	int ret;
	int len;
	int fd_flag;
	fd_set fds;
	//将sockfd设置成非阻塞模式
	if(fd_flag = fcntl(connfd, F_GETFL, 0) < 0){
		perror("fcntl get");
		return;
	}
	fd_flag |= O_NONBLOCK;
	if(fcntl(connfd, F_SETFL, fd_flag) < 0){
		perror("fcntl set");
		return;
	}
	
	struct timeval time;
	for(;;){
		time.tv_sec = 20;
		time.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(connfd,&fds);
		ret = select(MAXFD,&fds,NULL,NULL,&time);
		switch(ret){
			case -1:
				//perror("select");
				printf("one recv pthread is closed\n");
				pthread_exit(NULL);
			case 0:
				break;
			default:
				deal_recv(connfd,&fds);
				break;
		}
	}
	return NULL;
}

//处理接受
void deal_recv(int connfd,fd_set *fds){
	char *recv_buf = NULL;
	int ret;
	if(FD_ISSET(connfd,fds)){
		recv_buf = malloc(DATA_SIZE);
		if(recv_buf == NULL)
		{
			perror("malloc error");
			return;
		}
		memset(recv_buf,0,DATA_SIZE);
		ret = recv(connfd,recv_buf,DATA_SIZE,0);
		if(ret > 0){
			deal_recv_data(connfd,recv_buf,ret);
		}
		else{
			deal_recv_error(ret,connfd,fds);
		}
		
	}
}

//关闭连接
void closesock(int connfd,fd_set *fds){
	printf("connfd -- %d\n",connfd);
	close(connfd);
	FD_CLR(connfd,fds);
printf("after clear fds\n");
	sem_wait(&heartdet_sem);
	del_stalink(&status,connfd);	
	sem_post(&heartdet_sem);
}

//接受出错处理
void deal_recv_error(int ret,int connfd,fd_set *fds){
	if(ret == 0){
		printf("the client disconnect\n");
		closesock(connfd,fds);
	}
	else{
		if(errno == 104){
			printf("the client disconnect 104\n");
			closesock(connfd,fds);
		}
		else if(errno == ETIMEDOUT | errno == EHOSTUNREACH){
			printf("the client is breakdown!");
			closesock(connfd,fds);
		}
		else{
			perror("recv error!");
			closesock(connfd,fds);
			return;
		}
	}
	pthread_exit(NULL);
}

//分析接受到的数据并给出相应的处理
void deal_recv_data(int connfd,char *recv_buf,int len){
	switch(recv_buf[3]){
		case 1:
			sem_wait(&reg_sem);
			add_regque(reg,recv_buf,len,connfd,0);
			sem_post(&reg_sem);
			break;
		case 3:
			sem_wait(&heart_sem);
			add_regque(heart,NULL,0,connfd,0);
			sem_post(&heart_sem);
			break;
		case 19:
			sem_wait(&update_sem);
			add_regque(update,recv_buf,len,connfd,0);
			sem_post(&update_sem);
			break;
		case 13:
			update_flag = 1;
			break;
		default:
			break;
	}
}