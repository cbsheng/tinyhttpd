#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
 int sockfd;
 int len;
 struct sockaddr_in address;
 int result;
 char ch = 'A';

 //申请一个流 socket
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 //填充地址结构，指定服务器的 IP 和 端口
 address.sin_family = AF_INET;
 //inet_addr 可以参考 man inet_addr
 address.sin_addr.s_addr = inet_addr("127.0.0.1");
 address.sin_port = htons(9734);
 len = sizeof(address);
 
 //可以用下面的语句输出连接的 IP 地址
 //printf("%s\n", inet_ntoa( address.sin_addr));
 
 result = connect(sockfd, (struct sockaddr *)&address, len);

 if (result == -1)
 {
  perror("oops: client1");
  exit(1);
 }
 
 //往服务端写一个字节
 write(sockfd, &ch, 1);
 //从服务端读一个字符
 read(sockfd, &ch, 1);
 printf("char from server = %c\n", ch);
 close(sockfd);
 exit(0);
}
