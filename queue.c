#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/fcntl.h>
#include <errno.h>

#include "queue.h"


int init_que(QUEUE_head **head){
	*head = malloc(sizeof(QUEUE_head));
	if(head == NULL){
		perror("queue init error!");
		return -1;
	}
	(*head)->count = 0;
	(*head)->size = MAXCOUNT;
	(*head)->hnode.next = NULL;
	(*head)->hnode.pre = NULL;
	return 0;
}

int add_que(QUEUE_head *head,char *buf,int len,int connfd){
	if(head->count >= head->size){
		printf("queue is full!\n");
		return -1;
	}
	QUEUE_node *node = NULL;
	QUEUE_node *move = &(head->hnode);
	node = malloc(sizeof(QUEUE_node));
	if(node == NULL){
		perror("add error!");
		return -1;
	}
	
	memcpy(node->data,buf,len);
	node->connfd = connfd;
	
	if(head->hnode.next == NULL){
		head->hnode.next = node;
		node->pre = &(head->hnode);
		node->next = NULL;
	}
	
	while(move->next != NULL){
		move = move->next;
	}
	move->next = node;
	node->pre = move;
	node->next = NULL;

	head->count++;
	
	return 0;
}

QUEUE_node *out_que(QUEUE_head *head){
	if(head->count == 0){
		printf("queue is empty");
		return NULL;
	}
	QUEUE_node *move = head->hnode.next;
	
	if(head->count == 1){
		head->count--;
		return move;
	}
	
	head->hnode.next = move->next;
	move->next->pre = &(head->hnode);
	head->count--;
	return move;

}

// int main(){
	// int ret,i;
	// QUEUE_head *head = NULL;
	// QUEUE_node *node = NULL;
	// ret = init_que(&head);
	// printf("init %d\n",ret);
	// char buf[10] = "nihao!";
	// for(i=0;i<5;i++){
		// add_que(head,buf,strlen(buf)+1,10);
		// printf("count %d\n",head->count);
	// }
	// while(1){
		// node = out_que(head);
		// if(node == NULL)
			// break;
		// printf("%s\n%d\n%d\n",node->data,node->pid,head->count);
		// free(node);
	// }
	// return 0;
// }