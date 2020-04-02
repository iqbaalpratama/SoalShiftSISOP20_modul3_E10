# SoalShiftSISOP20_modul3_E10

Soal 4
4a.1. Buatlah program C dengan nama "4a.c", yang berisi program untuk
melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2, dan
matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks
nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).
4a.2. Tampilkan matriks hasil perkalian tadi ke layar.
4b.1. Buatlah program C kedua dengan nama "4b.c". Program ini akan
mengambil variabel hasil perkalian matriks dari program "4a.c" (program
sebelumnya), dan tampilkan hasil matriks tersebut ke layar.
(Catatan!: gunakan shared memory)
4b.2. Setelah ditampilkan, berikutnya untuk setiap angka dari matriks
tersebut, carilah nilai faktorialnya, dan tampilkan hasilnya ke layar dengan
format seperti matriks.

Contoh: misal array [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], ...],

maka:

1 2 6 24
120 720 ... ...
...

(Catatan! : Harus menggunakan Thread dalam penghitungan
faktorial)
4c.1. Buatlah program C ketiga dengan nama "4c.c". Program ini tidak
memiliki hubungan terhadap program yang lalu.
4c.2. Pada program ini, Norland diminta mengetahui jumlah file dan
folder di direktori saat ini dengan command "ls | wc -l". Karena sudah belajar
IPC, Norland mengerjakannya dengan semangat.
(Catatan! : Harus menggunakan IPC Pipes)

Source code nomor 4a:
```c
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

pthread_t tid[5]; //inisisasi banyaknya thread (dalam kasus ini 5 thread)
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
    key_t key = 1234;
    int shmid = shmget(key,sizeof(int[4][5]),0666|IPC_CREAT); 
    arrays3 =  shmat(shmid,NULL,0);  
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
```
Penjelasan source code no 4a:
Untuk soal no 4a digunakan thread sebanyak 5, yaitu dimana setiap thread digunakan untuk menghitung hasil perkalian dari setiap kolom matriks, dimana dideklarasikan juga ukuran matriks yang dikalikan, matriks hasil seperti berikut, matriks hasil disini menggunakan pointer karena akan digunakan dalam shared memory
```c
int arrays1[4][2], arrays2[2][5];
int (*arrays3)[5];``
```
Kemudian ada fungsi multiplier yang merupakan fungsi yg menjalankan tugas masing-masing thread 
```c
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
```
Dari setiap id thread dicek untuk menjalankan tugasnya yaitu menghitung dan menyimpan nilai hasil perkalian dari setiap kolom. Untuk perkaliannya, seperti matriks pada umumnya yaitu misal untuk menghitung nilai matriks hasil pada elemen (1,1) maka caranya elemen (1,1) Matriks 1  *  elemen (1,1) Matriks 2 + elemen (1,2) Matriks 1 * elemen (2,1) Matriks 2 
