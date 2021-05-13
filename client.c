#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUFFER_SIZE 2048
#define FILENAME_SIZE 500
int main(int argc, char **argv ){
	if(argc != 2){
		exit(1);
	}
	//设置client地址结构
	struct sockaddr_in clnt_addr;
	memset(&clnt_addr, 0, sizeof(clnt_addr));
	clnt_addr.sin_family = AF_INET;
	clnt_addr.sin_addr.s_addr = htons(INADDR_ANY);
	clnt_addr.sin_port = htons(0);
	//创建客户端socket 
	int clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	bind(clnt_sock, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));
	struct sockaddr_in  sevr_addr;  
    memset(&sevr_addr, 0, sizeof(sevr_addr));
    sevr_addr.sin_family = AF_INET;
    //通过程序参数获取服务器IP地址
	inet_aton(argv[1], &sevr_addr.sin_addr);
	sevr_addr.sin_port = htons(1234);
    socklen_t sevr_addr_length = sizeof(sevr_addr);
	//向服务器(特定的IP和端口) 发起请求
	if(connect(clnt_sock, (struct sockaddr*)&sevr_addr, sevr_addr_length)<0){
		printf("Can't Connect %s!\n",argv[1]);
		exit(1);
	}
	char FileName[FILENAME_SIZE];
	char buffer[BUFFER_SIZE];
	memset(&FileName, 0, sizeof(FileName));
	memset(&buffer, 0, sizeof(buffer));
	printf("Please Input File Name: ");
	scanf("%s",FileName);
    strncpy(buffer, FileName, strlen(FileName) > BUFFER_SIZE ? BUFFER_SIZE : strlen(FileName));  
    send(clnt_sock, buffer, BUFFER_SIZE, 0);
	FILE *fp = fopen(FileName, "w");
	if(fp==NULL){
		printf("File:%s Can't Open!",FileName);
		exit(1);
	}
	int Length =  recv(clnt_sock, buffer,BUFFER_SIZE, 0);
	while(Length=recv(clnt_sock, buffer,BUFFER_SIZE, 0)>0){
		int write_len = fwrite(buffer, sizeof(char),Length,fp);
		if(write_len<Length){
			printf("File Write Failed!\n");
			break;
		}
		//memset(&buffer, 0, sizeof(buffer));
		
	}
	printf("Recieve File:%s From Server[%s]!\n", FileName, argv[1]);
	fclose(fp);
	close(clnt_sock);
	return 0;
}

