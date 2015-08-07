#include "eaproxy.h"
int sock_lan; //连接
struct ifreq if_lan; //网络接口
void find_lan(char *interface) { //打开lan连接
  if ((sock_lan = socket(PF_PACKET, SOCK_RAW, htons(0x888e))) < 0) { //建立连接
    error("LAN socket() error"); //出错提示
  }
  strcpy(if_lan.ifr_name, interface); //指定lan接口
  printf("\tLAN interface: %s\n", if_lan.ifr_name);
  if (ioctl(sock_lan, SIOCGIFINDEX, &if_lan) < 0) { //查找接口
    error("LAN ioctl() error"); //出错提示
  }
  struct sockaddr_ll sll_lan;
  sll_lan.sll_family = AF_PACKET; //设置lan协议族
  sll_lan.sll_ifindex = if_lan.ifr_ifindex; //设置lan接口
  sll_lan.sll_protocol = htons(0x888e); //设置lan协议类型
  if (bind(sock_lan, (struct sockaddr *) &sll_lan, sizeof(sll_lan)) < 0) { //将接口与地址绑定
    error("LAN bind() error"); //出错提示
  }
}

void open_lan(void) {
  if (ioctl(sock_lan, SIOCGIFFLAGS, &if_lan) < 0) { //准备混杂模式
    error("LAN ioctl() error"); //出错提示
  }
  if (promiscuous != 0) { //混杂模式
    if_lan.ifr_flags |= IFF_PROMISC; //混杂模式
  } else { //非混杂模式
    if_lan.ifr_flags &= ~IFF_PROMISC; //非混杂模式
  }
  if (ioctl(sock_lan, SIOCGIFFLAGS, &if_lan) < 0) { //以混杂模式打开网卡
    error("LAN ioctl() error"); //出错提示
  }
  if (ioctl(sock_lan, SIOCGIFHWADDR, &if_lan) < 0) { //查询mac
    error("LAN ioctl() error"); //出错提示
  }
  memcpy(mac_lan, if_lan.ifr_hwaddr.sa_data, 6); //获得mac地址
  printf("\tLAN MAC address: %02x-%02x-%02x-%02x-%02x-%02x\n", mac_lan[0],
      mac_lan[1], mac_lan[2], mac_lan[3], mac_lan[4], mac_lan[5]);
}

void filter_lan(unsigned char *buffer) { //锁定客户端
  if (promiscuous == 0) { //非混杂模式
    memcpy(server_wan, buffer, 6); //初始化server为组播mac
  }
  memcpy(client_lan, buffer + 6, 6);  //取出client
  printf("\tClient MAC address: %02x-%02x-%02x-%02x-%02x-%02x\n",
      client_lan[0], client_lan[1], client_lan[2], client_lan[3],
      client_lan[4], client_lan[5]);
}

void send_lan(unsigned char *buffer, int length) { //lan发包
  memcpy(buffer, client_lan, 6);  //修改dst为client
  if (promiscuous < 1) {  //非混杂模式
    memcpy(buffer + 6, mac_lan, 6); //修改src为lan
  }
  if (sendto(sock_lan, buffer, length, 0, NULL, 0) < 0) {
    error("LAN sendto() error");  //错误提示
  }
  printf("\tPacket over LAN: %ld\n", time(NULL)); //输出响应时间
}

void work_lan(void) { //lan线程
  unsigned long int tid_hello = 0;  //中继线程
  puts("Opening the LAN socket connection...");
  open_lan(); //查询接口
  puts("Receiving the packets from LAN...");
  int len_lan; //包长度
  unsigned char buf_lan[1024]; //缓冲区
  while ((len_lan = recvfrom(sock_lan, buf_lan, 1024, 0, NULL, NULL)) > 0) { //循环接收
    puts("get packets form lan");
    fflush(stdout); //刷新输出缓冲区
  } //while
  error("lan recvfrom() error");  //监听失败
}
