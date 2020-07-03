#include "tcp_net_socket.h"

/*
定义并初始化服务端socket，并开始监听端口
输入 —— char* ip: 服务端地址
	    port: 服务端端口
输出 —— int sfd： 服务端创建并处于监听状态的socket
*/
int tcp_init(const char* ip, int port){

	//第一步：创建服务端socket，获得socket
	//AF_INET表示协议族采用IPv4，若用IPv6则改为AF_INET6，AF可以用PF代替
	//SOCK_STREAM表示服务类型为TCP/IP，若用UDP则改为SOCK_DGRAM
	//第三个参数一般用0即可
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1){
		perror("error happen when apply socket()");
		exit(-1);
	}

	//第二步：定义服务端地址
	struct sockaddr_in serveraddr;
	//清空内存
	bzero(&serveraddr, sizeof(serveraddr));

	//定义服务端地址具体的协议族、端口号和IP
	//协议族跟上面定义socket时一样
	//ip要用inet_addr()将点分十进制字符串转为网络字节序整数表示的IP地址，端口号则用htons()转换
	serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    // serveraddr.sin_addr.s_addr = INADDR_ANY; 
    serveraddr.sin_addr.s_addr = inet_addr(ip);

    //第三步：命名socket（将第一步创建的socket与第二步定义地址绑定)
    //地址均要强制转换为struct sockaddr格式，这三个参数基本只要按需要改动&serveraddr即可
	if(bind(sfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) == -1){

        perror("error happen when apply bind()");
        close(sfd);
        exit(-1);

    }

	//第四步：监听，并设置其允许最大的连接数为10个，参数名字叫backlog
    if(listen(sfd, 10) == -1){

        perror("error happen when apply listen()");
        close(sfd);
        exit(-1);

    }

    return sfd;

}

/*
客户端接受连接
输入 —— int sfd：tcp_init()函数创建的客户端socket
输出 —— new_sfd: 与客户端建立连接后创建的新socket，用于后续的信息传送与接受
*/
int tcp_accept(int sfd){

	//创建缓冲存放客户端地址
    struct sockaddr_in clientaddr;
    bzero(&clientaddr, sizeof(clientaddr));

    //接受客户端连接，并创建新的socket为new_fd，将请求连接的客户端的ip、port保存在结构体clientaddr中
    //accept和bind的输入参数的类型基本一致，但是第三个参数是用内存地址传入，其原型为:
    //     int accept(int sockfd, sturct socketaddr *addr, socket_t *addrlen);
    //对比 int   bind(int sockfd, sturct socketaddr *addr, socket_t addrlen);
    int addrlen = sizeof(struct sockaddr);
	int new_fd = accept(sfd, (struct sockaddr*)&clientaddr, &addrlen);
    if(new_fd == -1)
    {
        perror("error happen when apply accept()");
        close(sfd);
        exit(-1);

    }

    //inet_ntoa是inet_addr的逆操作，ntohs是htons的逆操作
    printf("%s %d success connect...\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	return new_fd;

}


/*
创建客户端socket，并向服务端申请连接
输入 —— char* ip: 服务端地址，注意，这里填的仍然是服务端的地址，客户端的地址在这份文件的整个流程中都不需要自己填写
	    port: 服务端端口
输出 —— 客户端创建并与服务端连接后的socket
*/
int tcp_connect(const char* ip, int port)   
{
	//创建客户端socket，与tcp_init()中服务端的操作一致
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1){

        perror("socket");
        exit(-1);

    }

    //定义服务端地址，与tcp_init()中服务端的操作一致
    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    // serveraddr.sin_addr.s_addr = INADDR_ANY; // inet_addr(ip);
    serveraddr.sin_addr.s_addr = inet_addr(ip);

    //将sfd连接至制定的服务器网络地址serveraddr
	if(connect(sfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) == -1){

		perror("connect");
		close(sfd);
		exit(-1);

    }

    return sfd;

}


//用于信号处理，让服务端在按下Ctrl+c或Ctrl+\的时候不会退出
void signalhandler(void)   {

	sigset_t sigSet;
    sigemptyset(&sigSet);
    sigaddset(&sigSet,SIGINT);
    sigaddset(&sigSet,SIGQUIT);
    sigprocmask(SIG_BLOCK,&sigSet,NULL);

}