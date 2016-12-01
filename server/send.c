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

/***global***/
extern int errno;
extern REG_HEAD *qsend;			//发送队列
extern sem_t send_sem;				//发送用信号量
extern STATUS *status;
extern sem_t heartdet_sem;
/***********/

void *deal_send(void *arg){
	printf("the send pthread is start!\n");
	REG *send_data = NULL;
	int ret;
	for(;;){
		if(qsend->count == 0){
			continue;
		}
		sem_wait(&send_sem);
		send_data = out_regque(qsend);
		sem_post(&send_sem);
		if(send_data == NULL){
			perror("send data get error!");
			continue;
		}
		ret = send(send_data->connfd,send_data->data,send_data->len,0);
		printf("%s\n",send_data->data);
		printf("should send num %d\n",send_data->len);
		printf("send num %d\n",ret);
		if(ret < 0){
			close(send_data->connfd);
			sem_wait(&heartdet_sem);
			del_stalink(&status,send_data->connfd);
			sem_post(&heartdet_sem);
		}
		//错误数据需要断开连接
		if(send_data->flag == 1){
			close(send_data->connfd);
		}
		free(send_data->data);
		free(send_data);
	}
}