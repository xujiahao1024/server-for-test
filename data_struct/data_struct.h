#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__

//注册用&输出用队列的数据结构
typedef struct reg{
	int connfd;
	char *data;
	int len;
	int flag;
	struct reg *next;
}REG;

typedef struct reg_head{
	int count;
	REG hreg;
}REG_HEAD;

extern int init_regque(REG_HEAD **head);

extern int add_regque(REG_HEAD *head,char *buf,int len,int connfd,int flag);

extern REG *out_regque(REG_HEAD *head);

//状态队列的数据结构
typedef struct status{
	int connfd;
	int apid;
	int heart;
	struct status *next;
}STATUS;

//添加一个节点到状态队列
extern int add_stalink(STATUS **head,int connfd,int apid,int heart);

//根据链接套接字删除一个节点
extern int del_stalink(STATUS **head,int connfd);

//根据apid心跳位清零
extern int bzero_stalink(STATUS *head,int apid);

//所有apid心跳位加1
extern int alterheart_stalink(STATUS *head);

//心跳位检测，超过5就用close断开连接
extern int detection_stalink(STATUS *head);

//查询当前已注册基站的apid
extern int query_stalink(STATUS *head);

//判断apid是否在链表中
extern STATUS *selectapid_stalink(STATUS *head,int apid);

#endif