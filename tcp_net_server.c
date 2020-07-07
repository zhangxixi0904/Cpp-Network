#include "tcp_net_socket.h"

int main(int argc, char* argv[]){

	if(argc < 3){

		printf("usage:./servertcp  ip  port\n");
		exit(-1);
	
	}


	//signalhandler();
	
	int sfd = tcp_init(argv[1], atoi(argv[2]));  //或int sfd = tcp_init("192.168.0.164", 8888);
	
	//用while循环表示可以与多个客户端接收和发送，但仍是阻塞模式的
	while(1) {

		
		int cfd = tcp_accept(sfd);
		// 这条信息是为了测试accept()函数是否为阻塞的
		//printf("if accept is non-block, then this message will be kept sending to the screen even the client didn't send any message ");
		char buf[512] = {0};

		//从cfd客户端接收数据存于buf中
		int ret = 0;
		if((ret=recv(cfd, buf, sizeof(buf), 0)) == -1){

			perror("recv");
			close(cfd);
			close(sfd);
			exit(-1);

		}

		// puts(buf);
		printf("get %d byte message: %s\n", ret, buf);
		
		//从buf中取向cfd客户端发送数据
		//如果在客户端关闭后，服务端无任何操作也是直接关闭，即下方这段发送的代码注释掉，则服务端会将四次挥手第二三次挥手合并为1个报文
		if(send(cfd, "hello world", 11, 0) == -1){

			perror("send");
			close(cfd);
			close(sfd);
			exit(-1);

		}
		
		
		close(cfd);

	}

    close(sfd);

}
