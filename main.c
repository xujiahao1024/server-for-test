#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <pthread.h>

#include "server.h"
#include "data.h"


int main(int argc,char *argv[]){
	DATA data;
	bzero(&data,sizeof(data));
	
	//start_work_thread();
	
	//start_cmd_thread();
	
	start_server(&data);
	return 0;
}