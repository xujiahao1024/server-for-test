#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <errno.h>

#include "data_struct.h"

//注册用队列&&发送用队列
int init_regque(REG_HEAD **head){
	*head = malloc(sizeof(REG_HEAD));
	if(head == NULL){
		perror("queue init error!");
		return -1;		
	}
	memset(*head,0,sizeof(REG_HEAD));
	return 0;
}

int add_regque(REG_HEAD *head,char *buf,int len,int connfd,int flag){
	if(head == NULL){
		perror("head not init!");
		return -1;
	}
	REG *node = NULL;
	REG *move = &(head->hreg);
	node = malloc(sizeof(REG));
	if(node == NULL){
		perror("add error!");
		return -1;		
	}
	memset(node,0,sizeof(REG));
	
	node->data = buf;
	node->connfd = connfd;
	node->len = len;
	node->flag = flag;
	
	if(head->count == 0){
		head->hreg.next = node;
		node->next = NULL;
		head->count++;
		return 0;
	}
	
	while(move->next != NULL){
		move = move->next;
	}
	move->next = node;
	node->next = NULL;
	head->count++;
	
	return 0;
}

REG *out_regque(REG_HEAD *head){
	if(head->count == 0){
		printf("queue is empty!");
		return NULL;
	}
	REG *move = head->hreg.next;
	if(head->count == 1){
		head->count--;
		head->hreg.next = NULL;
		return move;
	}
	head->hreg.next = move->next;
	head->count--;
	return move;
}

/*
状态队列
*/
//添加一个节点到状态队列
int add_stalink(STATUS **head,int connfd,int apid,int heart){
	if(*head == NULL){
		*head = malloc(sizeof(STATUS));
		memset(*head,0,sizeof(STATUS));
		(*head)->connfd = connfd;
		(*head)->heart = heart;
		(*head)->apid = apid;
		(*head)->next = NULL;
		return 0;
	}
	
	STATUS *node = malloc(sizeof(STATUS));
	if(node == NULL){
		perror("not malloc!");
		return -1;
	}
	node->connfd = connfd;
	node->heart = heart;
	node->apid = apid;
	node->next = NULL;
	STATUS *move = *head;
	while(move->next != NULL){
		move = move->next;  
	}
	move->next = node;
	
	return 0;
}

//根据链接套接字删除一个节点
int del_stalink(STATUS **head,int connfd){
	if(head == NULL){
		printf("the stalink is empty\n");
		return -1;
	}
	STATUS *node = *head;
	STATUS *pre = *head;
	
	if(node->connfd == connfd){
		*head = node->next;
		free(node);
		return 0;
	}
	while(node == NULL){
		if(node->connfd == connfd)
			break;
		pre = node;
		node = node->next;
	}
	if(node == NULL)
		return -1;
	pre->next = node->next;
	free(node);
	
	return 0;
}

//根据apid心跳位清零
int bzero_stalink(STATUS *head,int apid){
	if(head == NULL){
		printf("the stalink is empty\n");
		return -1;
	}
	STATUS *move = head;
	while(move != NULL){
		if(move->apid == apid){
			move->heart = 0;
			return 0;
		}
		move = move->next;
	}
	if(move == NULL){
		printf("not found the apid!\n");
		return -1;
	}
}

//所有apid心跳位加1
int alterheart_stalink(STATUS *head){
	if(head == NULL){
		printf("the stalink is empty!\n");
		return -1;
	}
	STATUS *move = head;
	while(move != NULL){
		move->heart = move->heart + 1;
		move = move->next;
	}
	return 0;
}

//心跳位检测，超过5就用close断开连接
int detection_stalink(STATUS *head){
	if(head == NULL){
		printf("the stalink is empty!\n");
		return -1;
	}	
	STATUS *move = head;
	while(move != NULL){
		if(move->heart >= 5){
			close(move->connfd);
			del_stalink(&head,move->connfd);
		}
		move = move->next;
	}	
}

//查询当前已注册基站的apid
int query_stalink(STATUS *head){
	if(head == NULL){
		printf("the stalink is empty!\n");
		return -1;
	}
	STATUS *move = head;
	printf("all of the apid:\n");
	while(move != NULL){
		printf("apid:%d\n",move->apid);
		move = move->next;
	}
	return 0;
}

//判断apid是否在链表中
STATUS *selectapid_stalink(STATUS *head,int apid){
	if(head == NULL){
		printf("the stalink is empty!\n");
		return NULL;
	}
	STATUS *move = head;
	while(move != NULL){
		if(move->apid == apid){
			return move;
		}
		move  = move->next;
	}
	if(move == NULL)
		return NULL;
}

//获得当前最小可用apid
int getapid_stalink(STATUS *head){
	if(head == NULL)
		return 0;
	int minapid = 0;
}



// int main(){
	// int ret,i;
	// REG_HEAD *head = NULL;
	// REG *node = NULL;
	// ret = init_regque(&head);
	// //printf("init %d\n",ret);
	// char buf[10] = "nihao!";
	// for(i=0;i<5;i++){
		// add_regque(head,buf,strlen(buf)+1,10);
		// printf("count %d\n",head->count);
	// }
	// while(1){
		// node = out_regque(head);
		// if(node == NULL)
			// break;
		// printf("%s\n%d\n%d\n",node->data,node->connfd,head->count);
		// free(node);
	// }
	// return 0;
// }