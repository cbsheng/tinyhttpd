/*linux socket AF_INET TCP 编程示例，单进程单线程，ehco 客户端*/
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main()
{
	int cli_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8123);
	//serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//将 inet_addr() 改用 inet_pton() 这个现代的方法，支持 IPV4 和 IPV6
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) == -1) {
		printf("inet_pton error\n");
		close(cli_sock);
		return 0;
	}
	
	//将 inet_ntoa() 改用 inet_ntop() 这个现代方法，支持 IPV4 和 IPV6
	//printf("bind in %s : %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	//INET_ADDRSTRLEN 是标识 IPV4 地址展现字符串的大小常量,INET6_ADDRSTRLEN是 IPV6 的
	char serv_ip[INET_ADDRSTRLEN]; 
	if (inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, sizeof(serv_ip)) == NULL) {
		printf("inet_ntop error\n");
		close(cli_sock);
		return 0;
	}
	printf("bind in %s : %d\n", serv_ip, ntohs(serv_addr.sin_port));
	
	int conn = connect(cli_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (conn == -1) {
		printf("connect error\n");
		return 0;
	}
	
	char s[] = "hello world";
	char buf[256];
	write(cli_sock, s, strlen(s) + 1);
	read(cli_sock, buf, sizeof(buf));
	printf("read %s\n", buf);
	
	close(cli_sock);
	return 0;
}