/*linux socket AF_INET  UDP 编程示例，客户端，单进程单线程。*/
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int serv_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	//服务端自己的地址结构
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8345);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//将 inet_ntoa() 改用 inet_ntop() 这个现代方法，支持 IPV4 和 IPV6
	//printf("bind in %s : %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	//INET_ADDRSTRLEN 是标识 IPV4 地址展现字符串的大小常量,INET6_ADDRSTRLEN是 IPV6 的
	char serv_ip[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, sizeof(serv_ip)) == NULL) {
		printf("inet_ntop error\n");
		close(serv_sock);
		return 0;
	}
	printf("bind in %s : %d\n", serv_ip, ntohs(serv_addr.sin_port));
	
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		printf("bind error\n");
		return 0;
	}
	
	//客户端的地址结构
	struct sockaddr_in cli_addr;
	socklen_t cli_addr_len = sizeof(cli_addr);
	
	char buf[255];
	int num = 0;
	ssize_t n = recvfrom(serv_sock, buf, sizeof(buf), 0,
						 (struct sockaddr*)&cli_addr,
						 &cli_addr_len );
	while (buf[0] != '\0') {
		num++;
		printf("recv data from %s : %d  %c\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf[0]);
		ssize_t nn = sendto(serv_sock, buf, sizeof(buf), 0, 
							(struct sockaddr*)&cli_addr, 
							sizeof(cli_addr) );
		
		if (nn == -1)
			printf("sendto error\n");
		n = recvfrom(serv_sock, buf, sizeof(buf), 0,
					 (struct sockaddr*)&cli_addr,
					 &cli_addr_len );
	}
	printf("\nnum: %d\n", num);
	close(serv_sock);
	return 0;
}