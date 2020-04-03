#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>
#include <ctype.h> 
#include <limits.h>
#define PORT 8080

pthread_t tid[1000];
int player=0;

void *client(void *arg){
    int valread;
    int socketfd = *(int *)arg;
    char buffer[1024] = {0};
    FILE *fpoint;
    fpoint = fopen("/home/syubban/Desktop/praktikum3/datalogin.txt", "a+");
    
    while (1)
    {
        char str;
        char temp[512];
        int line_num = 1;
        int result=0;
        
        valread = read(socketfd , buffer, 1024);
        // printf("valreadnya->%d\n", valread);
        if (strcmp(buffer, "register")==0)
        {
            char buffer2[1024] = {0};
            valread = read(socketfd , buffer2, 1024);
            strcat(buffer2,"\n");
            fputs(buffer2,fpoint);
            fclose(fpoint);
            fpoint = fopen("/home/syubban/Desktop/praktikum3/datalogin.txt", "r");
            printf("List akun: \n");
            printf("Username | Password\n");
            while((str=fgetc(fpoint))!=EOF) 
            {
                printf("%c",str);
            }
            send(socketfd , "register success" , strlen("register success"), 0 );
            fclose(fpoint);  
        }
        else if (strcmp(buffer, "login")==0)
        {
            char buffer2[1024] = {0};
            valread = read(socketfd , buffer2, 1024);
            fpoint = fopen("/home/syubban/Desktop/praktikum3/datalogin.txt", "r");
            while(fgets(temp, 512, fpoint) != NULL) 
            {
                if((strstr(temp, buffer2)) != NULL) 
                {
                    result++;
                    break;       
                }
                line_num++;
            }
            if(result!=0)
            {
                printf("Auth success\n");
                send(socketfd , "login success" , strlen("login success"), 0 );
            }
            else
            {
                send(socketfd , "login failed" , strlen("login failed"), 0 );
            }
            fclose(fpoint);
        }
        else if (strcmp(buffer, "find match")==0){
            printf("Masuk sini\n");
            // player++;
            // if(player == 2){
            //     send(socketfd , "Game dimulai" , strlen("Game dimulai"), 0 );
            //     player = 0;
            // }
        }
    }
}

int main(int argc, char const *argv[]) 
{
    
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int counter = 1;
    int server_fd, socketfd;
    // , valread
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) 
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    int i;
    int total = 0;
    char buffering [1024] = {0};
    // printf("coba");
    while(1)
    {
        if ((socketfd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        pthread_create(&(tid[total]), NULL, &client, &socketfd);
        total++;
    }
}



