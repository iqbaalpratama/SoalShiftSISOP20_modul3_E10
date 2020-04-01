#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
pthread_t tid[5]; //inisisasi banyaknya thread (dalam kasus ini 3 thread)
pid_t child;
int controller = 0;

int arrayshasil[4][5];
int (*arrays3)[5];

void *multiplier(void *arg) 
{
    pthread_t id = pthread_self();
    if(pthread_equal(id, tid[0]))
    {
        for (int i=0; i<4; i++) 
        {
           for(int j=(int)arrays3[i][4]; j>0; j--)
            {
                arrayshasil[i][4]*=j;
            }
        }
    }
    else if(pthread_equal(id, tid[1]))
    {
        for (int i=0; i<4; i++) 
        {
           for(int j=(int)arrays3[i][3]; j>0; j--)
            {
                arrayshasil[i][3]*=j;
            }
        }
    }
    else if(pthread_equal(id, tid[2]))
    {
        for (int i=0; i<4; i++) 
        {
            for(int j=(int)arrays3[i][2]; j>0; j--)
            {
                arrayshasil[i][2]*=j;
            }
        }
    }
    else if(pthread_equal(id, tid[3]))
    {
        for (int i=0; i<4; i++) 
        {
            for(int j=(int)arrays3[i][1]; j>0; j--)
            {
                arrayshasil[i][1]*=j;
            }  
        }
    }
    else 
    {
        for (int i=0; i<4; i++) 
        {
             int bound=(int)arrays3[i][0];
             for(int j=1; j<=bound; j++)
             {
                 arrayshasil[i][0]*=j;
             }
        }
    }
}

int main() 
{
    int k=0, err;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<5;j++)
        {
            arrayshasil[i][j]=1;
        }
    } 
 
    key_t key = ftok("shmfile",65); 

    int shmid = shmget(key,sizeof(int[4][5]),0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    arrays3 =  shmat(shmid,(void*)0,0);  
    printf("Matriks hasil:\n");
    for(int i=0; i<4; i++){
        for(int k=0; k<5; k++)
        {
            printf("%d ", arrays3[i][k]);
        }
        printf("\n");
    }
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

    printf("Matriks Faktorial:\n");
    for(int i=0; i<4; i++){
        for(int k=0; k<5; k++)
        {
            printf("%d ", arrayshasil[i][k]);
        }
        printf("\n");
    }

    shmdt(arrays3); 
    
    shmctl(shmid,IPC_RMID,NULL); 
     
    return 0; 
} 
