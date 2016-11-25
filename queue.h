#ifndef __QUEUE_H__
#define __QUEUE_H__

#define MAXCOUNT 50
#define BUFSIZE 1024

typedef struct qlink{
	struct qlink *pre;
	struct qlink *next;
	char data[BUFSIZE];
	int connfd;
}QUEUE_node;

typedef struct queue{
	int count;
	int size;
	QUEUE_node hnode;
}QUEUE_head;

extern int init_que(QUEUE_head **head);

extern int add_que(QUEUE_head *head,char *buf,int len,int connfd);

extern QUEUE_node *out_que(QUEUE_head *head);

#endif