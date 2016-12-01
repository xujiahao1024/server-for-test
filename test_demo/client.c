#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/fcntl.h>

#define SERVER_PORT 8080

int main(int argc,char *argv[]){
	int sockfd,ret,len;
	unsigned char server_ip[] = "192.168.154.129"; 
	unsigned char buf[1024] = "";
	unsigned char recv_buf[1024] = "";
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		perror("socket error");
		return -1;
	}
	
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	ret = inet_pton(AF_INET,server_ip,&server_addr.sin_addr);
	if(ret <= 0){
		perror("inet_pton error");
		return -1;
	}
	
	ret = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret < 0){
		perror("connect error!");
		return -1;
	}
	
	
		sprintf(buf,"%c%c%c%c%d",0,0,0,1,1024);
		if(buf[3] == 1)
			printf("1 is ok\n");

		ret = send(sockfd,buf,1024,0);
		if(ret < 0){
			perror("send error!");
			return -1;
		}
		printf("haven send!\n");
		
		ret = recv(sockfd,recv_buf,1024,0);
		if(ret < 0)
			perror("recv error");
		printf("data num:%d\n",ret);
	while(1);
	close(sockfd);
	
	
	return 0;
}