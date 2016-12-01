#ifndef __DEAL_H__
#define __DEAL_H__

extern void *handle_connect(void *arg);

void deal_recv(int connfd,fd_set *fds);

void deal_recv_error(int ret,int connfd,fd_set *fds); 

void deal_recv_data(int connfd,char *recv_buf,int len);

#endif