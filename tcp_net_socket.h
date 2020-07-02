#ifndef __TCP__NET__SOCKET__H
#define __TCP__NET__SOCKET__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

/*
extern 用在头文件函数声明时，只是暗示该函数可能会在其他源文件使用，没有实际意义，加不加都是一样
extern 修饰变量时，表示变量在别的地方已经定义过了，在这里要使用那个变量。注意最好在头文件中只做
声明，不做定义，因为如果在两个源文件中引用了该头文件，会报重复定义的错。
*/
extern int tcp_init(const char* ip, int port); 
extern int tcp_accept(int sfd);
extern int tcp_connect(const char* ip, int port);
extern void signalhandler(void);

#endif