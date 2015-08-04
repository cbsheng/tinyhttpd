/*linux socket AF_INET TCP 编程示例，单进程单线程，ehco 服务端*/
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

int main()
{
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serv_addr;
	int serv_addr_len = sizeof(serv_addr);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8123);
	
	//将 inet_ntoa() 改用 inet_ntop() 这个现代方法，支持 IPV4 和 IPV6
	//printf("bind in %s : %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	//INET_ADDRSTRLEN 是标识 IPV4 地址展现字符串的大小常量,INET6_ADDRSTRLEN是 IPV6 的
	char serv_ip[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, sizeof(serv_ip)) == NULL) {
		printf("inet_ntop error\n");
		close(server_sock);
		return 0;
	}
	
	printf("bind in %s : %d\n", serv_ip, ntohs(serv_addr.sin_port));
	if (bind(server_sock, (struct sockaddr*)&serv_addr, serv_addr_len) < 0) {
		printf("bind error\n");
		return 0;
	}
	
	if (listen(server_sock, 1)) {
		printf("listen error\n");
		return 0;
	}
	
	char buf[1024];
	while (1) {
		struct sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);
		
		printf("等待链接的到来\n");
		int client_sock = accept( server_sock, 
								  (struct sockaddr*)&client_addr, 
								  (unsigned int *)&client_addr_len );

		if (client_sock == -1) {
			printf("accept error\n");
			return 0;
		}
		printf("有新连接到来\n");
		int n = recv(client_sock, buf, sizeof(buf), 0);
		while (n > 0) {
			if (send(client_sock, buf, sizeof(buf), 0) < 0) {
				printf("send error\n");
				return 0;
			}
			n = recv(client_sock, buf, sizeof(buf), 0);
		}
		close(client_sock);
		printf("连接离开\n");
	}

	close(server_sock);
	return 0;
}