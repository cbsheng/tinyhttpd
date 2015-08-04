/*linux socket AF_INET  UDP 编程示例，客户端，单进程单线程。*/
#include <cstdio>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int cli_sock = socket(AF_INET, SOCK_DGRAM, 0);
	//conn_addr 是要连接的服务器地址结构
	struct sockaddr_in conn_addr;
	conn_addr.sin_family = AF_INET;
	conn_addr.sin_port = htons(8345);
	conn_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//serv_addr 是用来存储 recvfrom 中的地址结构
	struct sockaddr_in serv_addr;
	socklen_t serv_addr_len = sizeof(serv_addr);
	
	char c = 1, buf[255];
	int num = 0;
	while (c < 100) {
		sendto(cli_sock, &c, sizeof(c), 0,
			   (struct sockaddr*)&conn_addr,
			   sizeof(conn_addr) );
		int n = recvfrom(cli_sock, buf, sizeof(buf), 0,
						 (struct sockaddr*)&serv_addr,
						 &serv_addr_len );
		if (n > 0) {
			num++;
			printf("recv data from %s : %d  %c\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port), buf[0]);
		}
		++c;
	}
	c='\0';
	sendto(cli_sock, &c, sizeof(c), 0,
		   (struct sockaddr*)&conn_addr,
		   sizeof(conn_addr) );
	close(cli_sock);
	printf("\nnum : %d\n", num);
	return 0;
}