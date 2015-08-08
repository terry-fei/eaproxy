#include "eaproxy.h"

void config(int argc, char **argv) { //配置
  promiscuous = 0;  //混杂模式
  ip_wan = 0;
  netmask_wan = 0;
  gateway_wan = 0;
  dns_wan = 0;
  opterr = 0; //错误操作
  char *lan = "br-lan";
  char *wan = "eth0.2";
  int option; //操作符
  while ((option = getopt(argc, argv, "hL:W:p:u:a:i:n:g:d:")) != -1) { //大写参数是对工作有关键性影响的
    switch (option) { //操作符
    case 'L': //lan
      lan = optarg;
      break;
    case 'W': //wan
      wan = optarg;
      break;
    case 'p': //wan
      if (strcmp("LOCAL", optarg) == 0) {
        promiscuous = 1; //本地混杂
      } else if (strcmp("BOTH", optarg) == 0) {
        promiscuous = 2; //双向混杂
      } else {
        promiscuous = 0; //不混杂
      }
      break;
    default:
      printf("The option -%c is invalid, so abort!\n", option);
      exit(1);
    }
  }
  find_lan(lan); //打开网卡
  find_wan(wan); //打开网卡
  printf("\tPromiscUous mode: ");
  switch (promiscuous % 3) { //比较promiscuous_lan
  case 0: //关闭
    printf("Out of use\n");
    break;
  case 1: //单播
    printf("LAN network\n");
    break;
  case 2: //组播
    printf("LAN and WAN\n");
    break;
  }
}

void finish(void) { //进程关闭回调
  puts("process exit");
  fflush(stdout);
}

void error(char *msg) { //处理错误信息
  perror(msg); //输出错误
  exit(1); //错误退出
}

int main(int argc, char **argv) { //主函数
  config(argc, argv); //初始化参数
  puts("Opening the main work threads...");
  unsigned long int tid_lan, tid_wan; //线程
  if (pthread_create(&tid_lan, NULL, (void *) work_lan, NULL) < 0
      || pthread_create(&tid_wan, NULL, (void *) work_wan, NULL) < 0) { //创建线程
    error("pthread_create() error"); //出错提示
  }
  atexit(finish); //进程关闭回调
  fflush(stdout); //刷新输出缓冲
  pthread_join(tid_wan, NULL); //等待wan线程
  pthread_join(tid_lan, NULL); //等待lan线程
  return 0;
}
