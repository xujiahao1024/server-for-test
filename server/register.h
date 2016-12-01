#ifndef __REGISTER_H__
#define __REGISTER_H__

extern void *deal_register(void *arg);

void get_time(unsigned char *buf);

void register_fault(REG *reg_data);

void register_successed(REG *reg_data);

#endif