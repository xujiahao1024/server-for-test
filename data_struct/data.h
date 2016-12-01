#ifndef __DATA_H__
#define __DATA_H__

#include "data_struct.h"

typedef struct data{
	int sockfd;
	int flag;			//更新用状态位
	pthread_t reg_pid;
	pthread_t send_pid;
	pthread_t cmd_pid;
	pthread_t heartdet_pid;
	pthread_t heartzero_pid;
	pthread_t command_pid;
	pthread_t update_pid;
}DATA;


#endif