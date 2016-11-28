#ifndef __DATA_H__
#define __DATA_H__

#include "queue.h"

typedef struct data{
	int sockfd;
	int flag;			//更新用状态位
	pthread_t reg_pid;
	pthread_t cmd_pid;
}DATA;


#endif