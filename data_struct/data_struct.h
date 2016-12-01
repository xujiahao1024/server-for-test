#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__

//ע����&����ö��е����ݽṹ
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

//״̬���е����ݽṹ
typedef struct status{
	int connfd;
	int apid;
	int heart;
	struct status *next;
}STATUS;

//���һ���ڵ㵽״̬����
extern int add_stalink(STATUS **head,int connfd,int apid,int heart);

//���������׽���ɾ��һ���ڵ�
extern int del_stalink(STATUS **head,int connfd);

//����apid����λ����
extern int bzero_stalink(STATUS *head,int apid);

//����apid����λ��1
extern int alterheart_stalink(STATUS *head);

//����λ��⣬����5����close�Ͽ�����
extern int detection_stalink(STATUS *head);

//��ѯ��ǰ��ע���վ��apid
extern int query_stalink(STATUS *head);

//�ж�apid�Ƿ���������
extern STATUS *selectapid_stalink(STATUS *head,int apid);

#endif