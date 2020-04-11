#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <pthread.h>
#include <ctype.h> 
#include <limits.h>
#define PORT 8080

pthread_t tid[100];
// tid[0] untuk screen 2
char* create_call_login(char buffer[], int sock, int valread) 
{
    char username[200] = {0};
    char password[100] = {0};
    printf("Username : ");
    scanf("%s", username);
    printf("Password : ");
    scanf("%s", password);
    strcat(username, "|");
    strcat(username, password);

    send(sock , username , strlen(username) , 0 );
    valread = read(sock , buffer, 1024);
    return buffer;
}

void create_call_register(char buffer[], int sock, int valread) {
    char username[200] = {0};
    char password[100] = {0};
    printf("Username : ");
    scanf("%s", username);
    printf("Password : ");
    scanf("%s", password);
    strcat(username, "|");
    strcat(username, password);
    send(sock , username , strlen(username) , 0 );
    valread = read(sock , buffer, 1024);
    printf("%s\n", buffer);
}

void *screen_two(void *ardg){
    int sock = *(int *) ardg;
        while(1) {
        char strings[1024] = {0}, string2[1024] = {0}, amp;
        printf("1. Find Match\n2. Logout\nChoices : ");
        scanf("%s", string2);
        if(strcmp(string2, "Find Match")){
            while(1){
                if(strcmp(string2, "2") != 0){
                    printf("Waiting for player ..\n");
                    send(sock, "find match" , strlen("find match") , 0 );
                    read(sock , strings, 1024);
                    if(strcmp(strings, "Game dimulai")==0){
                        break;
                    }
                }else{
                    printf("Game dimulai silahkan tap tap secepat mungkin !!\n");
                    while (1){
                        // scanf("%c", &amp);
                        // send()
                        // read()
                        // if(buffer <= 0)
                        // break
                    }
                }
            }
        }else if(strcmp(string2, "Logout")){
            break;
        }
    }
    
}

int main() {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_address;
    int command;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_address, '0', sizeof(serv_address));

    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0) 
        {
            printf("\nConnection Failed \n");
            return -1;
        }
    

    while(1) 
    {   
        
        char buffer[1024] = {0}, buffer2[1024] = {0};
        printf("1. Login\n2. Register\nChoices : ");
        scanf("%s", buffer2);
        send(sock, buffer2 , strlen(buffer2), 0 );
        if (strcmp(buffer2,"login") == 0)
        {
           create_call_login(buffer, sock, valread);
           printf("%s\n", buffer);
           if (strcmp(buffer, "login success") == 0) {
                pthread_create(&(tid[0]), NULL, screen_two, &sock);
                pthread_join(tid[0], NULL);
            }
        }
        else if(strcmp(buffer2,"register") == 0)
        {
            create_call_register(buffer, sock, valread);
        }
    }
    return 0;
}
