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
#include <time.h>

#include "../data_struct/data.h"
#include "../data_struct/data_struct.h"
#include "register.h"

#define REGISTER_SIZE 1024
#define BUF_SIZE 36

/***global***/
extern int errno;
extern unsigned int apid;
extern REG_HEAD *reg;				//注册队列
extern sem_t reg_sem;				//注册用信号量
extern REG_HEAD *qsend;				//发送队列
extern sem_t send_sem;				//发送用信号量
extern STATUS *status;				//状态链表
extern sem_t heartdet_sem;			//检测心跳信号量
/***********/

void *deal_register(void *arg){
	printf("the register pthread is start!\n");
	REG *reg_data = NULL;
	int flag = 0;
	int size = 0;
	for(;;){
		if(reg->count == 0){
			continue;
		}
		printf("recv register data\n");
		sem_wait(&reg_sem);
		reg_data = out_regque(reg);
		sem_post(&reg_sem);
		if(reg_data == NULL){
			perror("register data get error!");
			continue;
		}
		printf("deal %d register\n",reg->count+1);
		sscanf(reg_data->data+4,"%d",&size);//取出size
		printf("----------%d\n",size);
		if(size != REGISTER_SIZE){
			register_fault(reg_data);
		}
		else{
			register_successed(reg_data);
		}
		/*
			分析数据，存入状态对列，回复注册成功报文，或失败报文
			free(reg_data->data);
			free(reg_data);
		*/
	}
	return NULL;
}

void get_time(unsigned char *buf){
	time_t timep;
	struct tm *p;
	int year;
	int mon;
	time(&timep);
	p = localtime(&timep);
	year = p->tm_year + 1900;
	mon = p->tm_mon + 1;
	buf[16] = year/1000+'0';
	buf[17] = (year/100)%10+'0';
	buf[18] = (year/10)%100+'0';
	buf[19] = year%10+'0';
	buf[20] = '-';
	buf[21] = mon/10+'0';
	buf[22] = mon%10+'0';
	buf[23] = '-';
	buf[24] = p->tm_mday/10+'0';
	buf[25] = p->tm_mday%10+'0';
	buf[26] = '-';
	buf[27] = ' ';
	buf[28] = p->tm_hour/10+'0';
	buf[29] = p->tm_hour%10+'0';
	buf[30] = ':';
	buf[31] = p->tm_min/10+'0';
	buf[32] = p->tm_min%10+'0';
	buf[33] = ':';
	buf[34] = p->tm_sec/10+'0';
	buf[35] = p->tm_sec%10+'0';
}

void register_fault(REG *reg_data){
	int ret;
	int id;
	printf("the register is fault!\n");
//记得释放空间
	unsigned char *send_buf = malloc(BUF_SIZE);
	if(send_buf == NULL){
		perror("send malloc error");
		return;
	}
	memset(send_buf,0,BUF_SIZE);
	send_buf[3] = 2;
	send_buf[7] = 28;
	id = apid;
	if(id > 99){
		printf("esl_working is full!");
		free(reg_data->data);	
		free(reg_data);
		close(reg_data->connfd);
	}
	send_buf[11] = id;
	apid++;
	send_buf[15] = 1;
	get_time(send_buf);
	
	//添加到发送队列
	sem_wait(&send_sem);
	add_regque(qsend,send_buf,BUF_SIZE,reg_data->connfd,1);
	sem_post(&send_sem);
	
	//shutdown(reg_data->connfd,0);	//close 改成shoutdown
	free(reg_data->data);	
	free(reg_data);
}

void register_successed(REG *reg_data){
	int ret;
	int id;
	printf("the register is successed!\n");
//记得释放空间
	unsigned char *send_buf = malloc(BUF_SIZE);
	if(send_buf == NULL){
		perror("send malloc error");
		return;
	}
	memset(send_buf,0,BUF_SIZE);
	send_buf[3] = 2;
	send_buf[7] = 28;
	id = apid;
	if(id > 99){
		printf("esl_working is full!");
		free(reg_data->data);	
		free(reg_data);
		close(reg_data->connfd);
	}
	send_buf[11] = id;
	apid++;
	send_buf[15] = 0;	
	get_time(send_buf);	
	//添加到发送队列
	sem_wait(&send_sem);
	add_regque(qsend,send_buf,BUF_SIZE,reg_data->connfd,0);
	sem_post(&send_sem);
	/*
		将数据添加到状态队列
	*/
	sem_wait(&heartdet_sem);
	ret = add_stalink(&status,reg_data->connfd,id,0);
	sem_post(&heartdet_sem);
	free(reg_data->data);	
	free(reg_data);
}