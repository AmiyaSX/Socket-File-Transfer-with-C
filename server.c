#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUFFER_SIZE 2048
#define FILENAME_SIZE 500
int main(int argc, char **argv)  
{  
    // ����һ��socket��ַ�ṹserver_addr,���������internet�ĵ�ַ�Ͷ˿�  
    struct sockaddr_in   server_addr;  
    bzero(&server_addr, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
    server_addr.sin_port = htons(1234);   
    // (TCP)socket����ͻ����ṩ����Ľӿ�  
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);   
    // ��socket��socket��ַ�ṹ��  
	bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)); 
	if(listen(server_socket, 20) == -1)
	{	
		perror("listen");
	    exit(1);
	}
    // �������˳���Ϊ�ͻ����ṩ����  
    while(1)  
    {  
        // ����ͻ��˵�socket��ַ�ṹclient_addr�����յ����Կͻ��˵�����󣬵���accept  
        struct sockaddr_in client_addr;  
        socklen_t          length = sizeof(client_addr);  
        int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);  
        if (new_server_socket < 0)  
        {  
            printf("Server Accept Failed!\n");  
            break;  
        }  
  
        char buffer[BUFFER_SIZE];  
        bzero(buffer, sizeof(buffer));  
        length = recv(new_server_socket, buffer, BUFFER_SIZE, 0);  
        if (length < 0)  break;
        char file_name[FILENAME_SIZE];  
        bzero(file_name, sizeof(file_name));  
        strncpy(file_name, buffer, strlen(buffer) >FILENAME_SIZE ? FILENAME_SIZE : strlen(buffer));  
        FILE *fp = fopen(file_name, "r");  
        if (fp == NULL)  
        {  
            printf("File:\t%s Not Found!\n", file_name);  
        }  
        else  
        {  
            bzero(buffer, BUFFER_SIZE);  
            int file_block_length = 0;  
            while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)  
            {   
                if (send(new_server_socket, buffer, file_block_length, 0) < 0)  
                {  
                    printf("Send File:\t%s Failed!\n", file_name);  
                    break;  
                }  
  
                bzero(buffer, sizeof(buffer));  
            }  
            fclose(fp);  
            printf("File:\t%s Transfer Done!\n", file_name);  
        }  
  
        close(new_server_socket);  
    }  
  
    close(server_socket);  
  
    return 0;  
}  
