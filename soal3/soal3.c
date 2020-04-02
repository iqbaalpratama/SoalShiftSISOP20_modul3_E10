#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <limits.h>
#include <dirent.h>

pthread_t tid[100];
pid_t child;

char cwd[1024] = {0};

int is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

void *hitung(void *arg) {
    if (!is_dir(arg)){
        char kopian[1024] = {0};
        memset(kopian, 0, 1024);

        char *token, *token2;
        int n=0, n2=0;
        char *arr[4], *arrs[10], ektensi[100], temp[100];
        memset(arr, 0, 4);
        memset(arrs, 0, 10);
        memset(ektensi, 0, 100);
        memset(temp, 0, 100);

        strcpy(kopian, arg);
        token = strtok(kopian, "/");
        while( token != NULL ) {
            arr[n] = token;
            n++;
            token = strtok(NULL, "/");
        }
        
        strcpy(temp, arr[n-1]);
        //printf("%s", temp);
        token2 = strtok(temp, ".");
        while( token2 != NULL ) {
            arrs[n2] = token2;
            n2++;
            token2 = strtok(NULL, ".");
        }

        printf("%d", n2);
        // printf("%s", arrs[n2-2]);

        strcpy(ektensi, arrs[n2-1]);
        for(int i=0 ; i<strlen(ektensi) ;i++){
            ektensi[i] = tolower(ektensi[i]);
        }
        
        // printf("%d", n2);
        char tempo[1024] = {0};
        char directory[1024] = {0};
        memset(tempo, 0, 1024);
        memset(directory, 0, 1024);
        strcpy(tempo, "/home/syubban/");
        
        if(n2 == 2) {
            strcpy(directory, arrs[n2-1]);
            strcat(tempo, directory);
            // printf("%s\n", tempo);
            mkdir(tempo, 0777);
        }else {
            strcpy(directory, "Unknown");
            strcat(tempo, directory);
            // printf("%s\n", tempo);
            mkdir(tempo, 0777);
        }
        
        // printf("%s\n", tempo);
        // printf("%s", directory);
        int ch;
        FILE *pf1, *pf2;
        pf1 = fopen(arg, "r");
        // printf("%s\n", argv[i]);
        // printf("%s", aa);
        
        strcat(tempo, "/");       
        strcat(strcat(strcat(tempo, arrs[n2-2]), "."), ektensi);

        // printf("%s\n", tempo);
        // printf("%s", directory);
        pf2 = fopen(tempo, "w");
        while ((ch = fgetc(pf1)) != EOF) {
            fputc(ch, pf2);
        }
        // remove(arg);
        // Close file
        fclose(pf1);
        fclose(pf2);
    }
}

void *hitung2(void *arg) {
    if (!is_dir(arg)){
        char kopian[1024] = {0};
        memset(kopian, 0, 1024);

        char *token, *token2;
        int n=0, n2=0;
        char *arr[4], *arrs[10], ektensi[100], temp[100];
        memset(arr, 0, 4);
        memset(arrs, 0, 10);
        memset(ektensi, 0, 100);
        memset(temp, 0, 100);

        strcpy(kopian, arg);
        token = strtok(kopian, "/");
        while( token != NULL ) {
            arr[n] = token;
            n++;
            token = strtok(NULL, "/");
        }
        
        strcpy(temp, arr[n-1]);
        //printf("%s", temp);
        token2 = strtok(temp, ".");
        while( token2 != NULL ) {
            arrs[n2] = token2;
            n2++;
            token2 = strtok(NULL, ".");
        }

        printf("%d", n2);
        // printf("%s", arrs[n2-2]);

        strcpy(ektensi, arrs[n2-1]);
        for(int i=0 ; i<strlen(ektensi) ;i++){
            ektensi[i] = tolower(ektensi[i]);
        }
        
        // printf("%d", n2);
        char tempo[1024] = {0};
        char directory[1024] = {0};
        memset(tempo, 0, 1024);
        memset(directory, 0, 1024);
        strcpy(tempo, cwd);
        strcat(tempo, "/");
        
        if(n2 == 2) {
            strcpy(directory, arrs[n2-1]);
            strcat(tempo, directory);
            // printf("%s\n", tempo);
            mkdir(tempo, 0777);
        }else {
            strcpy(directory, "Unknown");
            strcat(tempo, directory);
            // printf("%s\n", tempo);
            mkdir(tempo, 0777);
        }
        
        // printf("%s\n", tempo);
        // printf("%s", directory);
        int ch;
        FILE *pf1, *pf2;
        pf1 = fopen(arg, "r");
        // // printf("%s\n", argv[i]);
        // // printf("%s", aa);
        
        strcat(tempo, "/");       
        strcat(strcat(strcat(tempo, arrs[n2-2]), "."), ektensi);

        printf("%s\n", tempo);
        // // printf("%s", directory);
        pf2 = fopen(tempo, "w");
        while ((ch = fgetc(pf1)) != EOF) {
            fputc(ch, pf2);
        }
        // remove(arg);
        // Close file
        fclose(pf1);
        fclose(pf2);
    }
}

int main(int argc, char const *argv[]) {
    int loop = 0;
    if(strcmp(argv[1], "-f") == 0){
        for(int i=2; i<argc; i++){
            pthread_create(&(tid[i]), NULL, hitung, (void *)argv[i]);
        }
        for(int i=2; i<argc ;i++){
            pthread_join(tid[i], NULL);
        }
    }else if(strcmp(argv[1], "*") == 0){
        getcwd(cwd, sizeof(cwd));
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (cwd)) != NULL) {
            char temp[1024] = "\0";
            while ((ent = readdir (dir)) != NULL) {
                strcpy(temp, cwd);
                strcat(temp, "/");
                strcat(temp, ent->d_name);
                
                printf ("%s\n", temp);
                
                char a[50];
                char b[50];
                strcpy(a, cwd);
                strcat(a, "..");
                strcpy(b, cwd);
                strcat(b, ".");
                if(strcmp(temp,a)==0 || strcmp(temp,b)==0){
                    continue;
                }
                if(!is_dir(temp)){
                    pthread_create(&(tid[loop]), NULL, hitung, (void *)temp);
                    loop++;
                }
                for(int j=0; j<loop; j++){
                    pthread_join(tid[j], NULL);
                }
            }
            closedir (dir);
        }
    }else if(strcmp(argv[1], "-d") == 0){
        getcwd(cwd, sizeof(cwd));
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (argv[2])) != NULL) {
            char temp[1024] = "\0";
            while ((ent = readdir (dir)) != NULL) {
                strcpy(temp, argv[2]);
                //strcat(temp, "/");
                strcat(temp, ent->d_name);
                
                printf ("%s\n", temp);
                
                char a[50];
                char b[50];
                strcpy(a, cwd);
                strcat(a, "..");
                strcpy(b, cwd);
                strcat(b, ".");
                if(strcmp(temp,a)==0 || strcmp(temp,b)==0){
                    continue;
                }
                if(!is_dir(temp)){
                    pthread_create(&(tid[loop]), NULL, hitung2, (void *)temp);
                    loop++;
                }
                for(int j=0; j<loop; j++){
                    pthread_join(tid[j], NULL);
                }
            }
            closedir (dir);
        }
    }else{
        exit(0);
    }
}