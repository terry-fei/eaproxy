#ifndef EAPROXY_H_
#define EAPROXY_H_

#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
//MAIN
unsigned char promiscuous; //混杂
//LAN
unsigned char mac_lan[6]; //mac
unsigned char client_lan[6]; //mac地址
//WAN
unsigned int ip_wan; //绑定IP或自动获得的IP
unsigned int netmask_wan; //掩码
unsigned int gateway_wan; //网关
unsigned int dns_wan; //DNS
unsigned char mac_wan[6]; //mac地址
unsigned char server_wan[6]; //初始服务器地址
//PACKET
int size_buffer;
unsigned char data_buffer[1024]; //临时包
//MAIN
void error(char *message); //出错退出
//LAN
void find_lan(char *interface); //打开lan连接
void send_lan(unsigned char *buffer, int length); //lan发包
void work_lan(void); //lan线程
//WAN
void print_wan(void); //取出并打印地址
void refresh_wan(void); //dhcp并输出
void find_wan(char *interface); //打开wan连接
void send_wan(unsigned char *buffer, int length); //wan发包
void work_wan(void); //wan线程
//PACKET
void get_hello(unsigned char *data); //从hello得到中继变量
void get_interval(unsigned char *data); //得到时间间隔
void get_success(unsigned char *data); //从success得到中继变量
void set_hello(unsigned char *data); //修改hello包
void set_head(unsigned char *data, int size); //修改加密位
int set_identity(unsigned char *data, int size); //修改账户
int set_success(unsigned char *data, int size); //修改携带信息

#endif /* EAPROXY_H_ */
