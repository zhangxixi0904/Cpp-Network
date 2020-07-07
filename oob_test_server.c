#include "tcp_net_socket.h"

int main(int argc, char* argv[]){

	if(argc < 3){

		printf("usage:./servertcp  ip  port\n");
		exit(-1);
	
	}

	//signalhandler();
	
	int sfd = tcp_init(argv[1], atoi(argv[2]));  //或int sfd = tcp_init("192.168.0.164", 8888);

	int cfd = tcp_accept(sfd);
	char buf[512];
	fd_set read_fds;
	fd_set exception_fds;
	FD_ZERO( &read_fds );
	FD_ZERO( &exception_fds);
	//int count=0;
	while(1) {		
		// printf("%d\n", count++);
		memset(buf, '\0', sizeof(buf));
		//从cfd客户端接收数据存于buf中
		FD_SET(cfd, &read_fds);
		FD_SET(cfd, &exception_fds);

		int ret = select(cfd+1, &read_fds, NULL, &exception_fds, NULL);
		// printf("there are %d sockets ready\n", ret);
		if( ret <= 0 ){
			
			printf("selection failure\n");
			break;

		}
		

		if( FD_ISSET(cfd, &read_fds) ){

			int ret = recv(cfd, buf, sizeof(buf)-1, 0);
			if( ret == -1 ){

				perror("error happen when apply recv()");
				close(cfd);
				close(sfd);
				break;

			}
			//这一段ret<=0必须要要加要break，实测发现如果客户端断开了链接，select还是会一直返回可读事件read_fds
			//但是recv函数返回的却是空buffer
			else if( ret <= 0 ){
				//printf("In read_fds recive empty buf\n");
				break;
			}

			printf(" get %d bytes of normal data: %s \n", ret, buf);
			

			// if(send(cfd, "we get your normal data", 24, 0) == -1){

			// 	printf("%d\n", count);
			// 	perror("error happen when send back message to client");
			// 	close(cfd);
			// 	close(sfd);
			// 	break;

			//  }

		}


		else if( FD_ISSET(cfd, &exception_fds) ){

			int ret = ret = recv(cfd, buf, sizeof(buf), MSG_OOB);
			if( ret == -1){

				perror("error happen when apply recv()");
				close(cfd);
				close(sfd);
				break;

			}
			else if( ret<=0 ){
				// printf("In exception_fds recive empty buffer\n");
				break;
			}

			printf(" get %d bytes of oob data: %s \n", ret, buf);

			// if(send(cfd, "we get your oob data", 21, 0) == -1){

			// 	perror("error happen when send back message to client");
			// 	close(cfd);
			// 	close(sfd);
			// 	break;

			// }

		}
		//printf("%d\n",count);

	}

		
	close(cfd);	
	close(sfd);


}
