/*linux socket tcp 编程示例，客户端，单进程单线程。*/
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
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8123);
	printf("bind in %s : %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	
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