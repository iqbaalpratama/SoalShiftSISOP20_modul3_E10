#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

pthread_t tid[5]; //inisisasi banyaknya thread (dalam kasus ini 3 thread)
pid_t child;
int controller = 0;

int arrays1[4][2], arrays2[2][5];
int (*arrays3)[5];
void *multiplier(void *arg) {
    pthread_t id = pthread_self();
    if(pthread_equal(id, tid[0]))
    {
        for (int i=0; i<4; i++) {
            arrays3[i][4] = (arrays1[i][0]*arrays2[0][4]) + (arrays1[i][1]*arrays2[1][4]);
        }
    }
    else if(pthread_equal(id, tid[1]))
    {
        for (int i=0; i<4; i++) {
            arrays3[i][3] = (arrays1[i][0]*arrays2[0][3]) + (arrays1[i][1]*arrays2[1][3]);
        }
    }
    else if(pthread_equal(id, tid[2]))
    {
        for (int i=0; i<4; i++) {
            arrays3[i][2] = (arrays1[i][0]*arrays2[0][2]) + (arrays1[i][1]*arrays2[1][2]);
        }
    }
    else if(pthread_equal(id, tid[3]))
    {
        for (int i=0; i<4; i++) {
            arrays3[i][1] = (arrays1[i][0]*arrays2[0][1]) + (arrays1[i][1]*arrays2[1][1]);    
        }
    }
    else 
    {
        for (int i=0; i<4; i++) {
            arrays3[i][0] = (arrays1[i][0]*arrays2[0][0]) + (arrays1[i][1]*arrays2[1][0]);
        }
    }
}

int main()
{
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,sizeof(int[4][5]),0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    arrays3 =  shmat(shmid,(void*)0,0);  

    int k=0, err;

    
    printf("Matriks 1:\n");
    for(int i=0; i<4; i++)
    {
        int size = 1;
        for(int j=0; j<2 ;j++)
        {
            arrays1[i][j] = size;
            printf("%d ", arrays1[i][j]);
            size++;
        }
        printf("\n");
    }
    printf("\nMatriks2:\n");
    for(int i=0; i<2; i++)
    {
        int size1=1;
        for(int j=0; j<5 ;j++){
            arrays2[i][j] = size1;
            printf("%d ", arrays2[i][j]);
            size1++;
        }
        printf("\n");
    }
    printf("\n");

    while(k<5)
    {
        err = err=pthread_create(&(tid[k]), NULL, &multiplier, NULL); //pembuatan thread
        if(err != 0){
            printf("Can't create thread : [%s]\n", strerror(err));
        }else{
            //printf("Crate thread success\n");
        }
        k++;
    }
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[4], NULL);
    
    printf("Matriks hasil:\n");
    for(int i=0; i<4; i++){
        for(int k=0; k<5; k++)
        {
            printf("%d ", arrays3[i][k]);
        }
        printf("\n");
    }
    shmdt(arrays3);
    exit(0);
    return 0;
}
