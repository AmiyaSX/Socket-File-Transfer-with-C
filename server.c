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
    // ����socket��ַ�ṹserver_addr
    struct sockaddr_in   server_addr;  
    bzero(&server_addr, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);  
    // ����server_socket��ͻ����ṩ����Ľӿ�  
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);  
    // ��socket��socket��ַ�ṹ
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    // server_socket���ڼ���  
    listen(server_socket, LENGTH_OF_LISTEN_QUEUE);
    while(1)  
    {    
        struct sockaddr_in client_addr;  
        socklen_t length = sizeof(client_addr);  
        // accpet����һ���µ�socket,���ӵ�server��client����ͨ��   
        int To_clnt_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);  
        if (To_clnt_socket < 0)  
        {  
            printf("Server Accept Failed!\n");  
            break;  
        }  
        char buffer[BUFFER_SIZE];  
        bzero(buffer, sizeof(buffer));  
        length = recv(To_clnt_socket, buffer, BUFFER_SIZE, 0);  
        if (length < 0)  break;  
        char FileName[FILENAME_SIZE];  
        bzero(FileName, sizeof(FileName));  
        strncpy(FileName, buffer, strlen(buffer) > FILENAME_SIZE ? FILENAME_SIZE : strlen(buffer));  
        FILE *fp = fopen(FileName, "r");  
        if (fp == NULL)  
        {  
            printf("File:%s Not Found!\n", FileName);  
        }  
        else  
        {  
            bzero(buffer, BUFFER_SIZE);  
            int file_block_length = 0;  
            while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)  
            {  
                //printf("file_block_length = %d\n", file_block_length); 
                send(To_clnt_socket, buffer, file_block_length, 0);
                bzero(buffer, sizeof(buffer));  
            }  
            fclose(fp);  
            printf("File:%s Transfer Done!\n", FileName);  
        }  
  
        close(new_server_socket);  
    }  
  
    close(server_socket);  
  
    return 0;  
}  
