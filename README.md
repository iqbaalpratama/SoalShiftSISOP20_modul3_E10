# SoalShiftSISOP20_modul3_E10

**Soal 4**

4a.
1. Buatlah program C dengan nama "4a.c", yang berisi program untuk
melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2, dan
matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks
nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).
4a.
2. Tampilkan matriks hasil perkalian tadi ke layar.
4b.
1. Buatlah program C kedua dengan nama "4b.c". Program ini akan
mengambil variabel hasil perkalian matriks dari program "4a.c" (program
sebelumnya), dan tampilkan hasil matriks tersebut ke layar.
(Catatan!: gunakan shared memory)
4b.
2. Setelah ditampilkan, berikutnya untuk setiap angka dari matriks
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

**Source code nomor 4a:**
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

**Soal 3 :**

Buatlah sebuah program dari C untuk mengkategorikan file. Program ini akan
memindahkan file sesuai ekstensinya (tidak case sensitive. JPG dan jpg adalah
sama) ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working
directory ketika program kategori tersebut dijalankan.
● Semisal program dijalankan:
```
#File kategori terletak di /home/​ izone​ /kategori
$ ./kategori -f path/to/file1.jpg path/to/file2.c path/to/file3.zip
#Hasilnya adalah sebagai berikut
/home/​ izone
|-jpg
|--file1.jpg
|-c
|--file2.c
|-zip
|--file3.zip
```
● Pada opsi -f tersebut, user bisa menambahkan argumen file yang bisa
dikategorikan sebanyak yang user inginkan seperti contoh di atas.
● Pada program kategori tersebut, folder jpg,c,zip tidak dibuat secara manual,
melainkan melalui program c. Semisal ada file yang tidak memiliki ekstensi,
maka dia akan disimpan dalam folder “Unknown”.
● Program kategori ini juga menerima perintah (*) seperti di bawah;

```$ ./kategori \*```
● Artinya mengkategori seluruh file yang ada di working directory ketika
menjalankan program C tersebut.
● Selain hal itu program C ini juga menerima opsi -d untuk melakukan kategori
pada suatu directory. Untuk opsi -d ini, user hanya bisa menginput 1 directory
saja, tidak seperti file yang bebas menginput file sebanyak mungkin.

```$ ./kategori -d /path/to/directory/```
● Hasilnya perintah di atas adalah mengkategorikan file di /path/to/directory dan
hasilnya akan disimpan di working directory di mana program C tersebut
berjalan (hasil kategori filenya bukan di /path/to/​ directory​ ).
● Program ini tidak rekursif. Semisal di directory yang mau dikategorikan, atau
menggunakan (*) terdapat folder yang berisi file, maka file dalam folder
tersebut tidak dihiraukan, cukup file pada 1 level saja.
● Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan
secara paralel sehingga proses kategori bisa berjalan lebih cepat. ​ Dilarang
juga menggunakan fork-exec dan system.
● Silahkan download soal3.zip sebagai percobaan. Namun silahkan
dicoba-coba sendiri untuk kemungkinan test case lainnya yang mungkin
belum ada di soal3.zip.

**Source Code :**
``` c
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

		remove(arg);
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

		remove(arg);
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
``

**Penjelasan :**
