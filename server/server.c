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

#include "server.h"
#include "deal.h"

extern int errno;

#define DATA_SIZE 1024
#define PORT 8080
#define CONNECT_COUNT 10

void start_server(DATA *data){
	int connfd;
	int ret = -1;
	int flags;
	pthread_t pid;
	
	//创建tcp链接套接字
	data->sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(data->sockfd < 0){
		perror("socket error!");
		exit(-1);
	}
	
	//获取本地信息
	struct sockaddr_in myaddr;
	bzero(&myaddr,sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(PORT);
	myaddr.sin_addr.s_addr = htonl(0);	
	
	//将套接字与本地信息进行绑定
	int yes = 1;
    if (setsockopt(data->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt error!");
        exit(1);
    }
	ret = bind(data->sockfd,(struct sockaddr*)&myaddr,sizeof(myaddr));
	if(ret < 0){
		perror("bind fail!");
		exit(-1);
	}	
	
	//将套接字由主动变为被动
	ret = listen(data->sockfd,CONNECT_COUNT);
	if(ret != 0){
		perror("listen error");
		exit(-1);
	}

	printf("the server is succeed!\n");
	
	//监听套接字通信
	struct sockaddr_in client_addr;
	int addrlen = sizeof(client_addr);
	for(;;){
		bzero(&client_addr,addrlen);
		connfd = accept(data->sockfd,(struct sockaddr*)&client_addr,&addrlen);
		printf("%s is connected!\n",inet_ntoa(client_addr.sin_addr));
		pthread_create(&pid,NULL,handle_connect,(void *)connfd);
	}
}