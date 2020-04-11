# SoalShiftSISOP20_modul3_E10

**Soal 2 :**
Qiqi adalah sahabat MamMam dan Kaka. Qiqi , Kaka dan MamMam sangat senang
bermain “Rainbow six” bersama-sama , akan tetapi MamMam sangat Toxic ia selalu
melakukan Team killing kepada Qiqi di setiap permainannya. Karena Qiqi orang yang
baik hati, meskipun marah Qiqi selalu berkata “Aku nggk marah!!”. Kaka ingin
meredam kemarahan Qiqi dengan membuatkannya sebuah game yaitu TapTap
Game. akan tetapi Kaka tidak bisa membuatnya sendiri, ia butuh bantuan mu. Ayo!!
Bantu Kaka menenangkan Qiqi.
TapTap Game adalah game online berbasis text console. Terdapat 2 program yaitu
tapserver.c dan tapplayer.c
Syarat :
- Menggunakan Socket, dan Thread
Hint :
- fwrite, fread
Spesifikasi Game :

CLIENT SIDE

Screen 1 :
1. Login
2. Register
Choices : {your input}
★ Pada screen 1 kalian dapat menginputkan “login”, setelah menekan enter
anda diminta untuk menginputkan username dan password seperti berikut
Screen 1 :
1. Login
2. Register
Choices : login
Username : { ex : qiqi }
Password : { ex : aku nggak marah!! }
★ Jika login berhasil maka akan menampilkan pesan “login success”, jika gagal
akan menampilkan pesan “login failed” (pengecekan login hanya mengecek
username dan password, maka dapat multi autentikasi dengan username dan
password yang sama)
★ Pada screen 1 kalian juga dapat menginputkan “register”, setelah menekan
enter anda diminta untuk menginputkan username dan password sama
halnya seperti login
★ Pada register tidak ada pengecekan unique username, maka setelah register
akan langsung menampilkan pesan “register success” dan dapat terjadi
double account
★ Setelah login berhasil maka anda berpindah ke screen 2 dimana
menampilkan 2 fitur seperti berikut.

Screen 2 :
1. Find Match
2. Logout
Choices : {your input}
★ Pada screen 2 anda dapat menginputkan “logout” setelah logout anda akan
kembali ke screen 1
★ Pada screen 2 anda dapat menginputkan “find”, setelah itu akan
menampilkan pesan “Waiting for player ...” print terus sampai menemukan
lawan
★ Jika menemukan lawan maka akan menampilkan pesan “Game dimulai
silahkan tap tap secepat mungkin !!”
★ Pada saat game dimulai diberikan variable health = 100,dan anda dapat
men-tap (menekan space pada keyboard tanpa harus menekan enter)
★ Pada saat anda men-tap maka akan menampilkan pesan “hit !!”, dan pada
lawan healthnya akan berkurang sebanyak 10 kemudian pada lawan
menampilkan pesan status healthnya sekarang. (conclusion : anda tidak bisa
melihat status health lawan)
★ Jika health anda <= 0 maka akan menampilkan pesan “Game berakhir kamu
kalah”, apabila lawan anda healthnya <= 0 maka akan menampilkan pesan
”Game berakhir kamu menang”
★ Setelah menang atau kalah maka akan kembali ke screen 2

SERVER SIDE

★ Pada saat program pertama kali dijalankan maka program akan membuat file
akun.txt jika file tersebut tidak ada. File tersebut digunakan untuk menyimpan
username dan password
★ Pada saat user berhasil login maka akan menampilkan pesan “Auth success” jika
gagal “Auth Failed”
★ Pada saat user sukses meregister maka akan menampilkan List account yang
terdaftar (username dan password harus terlihat)

Source code server:
```c
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
```
Penjelasan source code server:
```c
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
```
Kode di atas merupakan fungsi client yang mana di fungsi tersebut ada tiga bagian utama yaitu login, register dan find match. Untuk register, nantinya setelah client mengirim password dan usernamenya, maka akan ditulis di file datalogin.txt dan kemudian file tersebut akan diprint seluruh isinya yang merupakan list dari semua user yang sudah register, kemudian dari server akan mengirimkan string "register success" untuk ditampilkan di layar client. Kemudian untuk login, setelah user memasukkan username dan passwordnya dan dibaca oleh server, maka akan dicek apakah username dan password tersebut ada di file datalogin.txt, jika tidak ada maka akan menampilkan "Auth Failed" serta mengirim string "login failed" ke client. jika username dan passwordnya ada maka akan menampilkan "Auth success" dan mengirim string "login success" ke client

```c
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
```
Kemudian untuk fungsi main sendiri merupakan fungsi utama dari socket yang dibuat. Dimana di akhir fungsi tersebut akan membuat thread dengan parameter fungsi client dan socketfd yang sudah didefinisikan sebelumnya

Source code client:
```c
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

char *create_call_login(char buffer[], int sock, int valread) 
{
    char username[200] = {0};
    char password[100] = {0};
    printf("Username : ");
    scanf("%s", username);
    printf("Password : ");
    scanf("%s", password);
    strcat(username, "|");
    strcat(username, password);

    // printf(" username %s",username);
    send(sock , username , strlen(username) , 0 );
    valread = read(sock , buffer, 1024);
    return buffer;
    printf("%s\n", buffer);
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

    // printf(" username %s",username);
    send(sock , username , strlen(username) , 0 );
    valread = read(sock , buffer, 1024);
    //printf("%s\n", buffer);
}

void *screen_two(int *ardg){
    int sock = *(int)ardg;
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
    return;
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


    while(1) 
    {
        if (connect(sock, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0) 
        {
            printf("\nConnection Failed \n");
            return -1;
        }
        char buffer[1024] = {0}, buffer2[1024] = {0};
        printf("1. Login\n2. Register\nChoices : ");
        scanf("%s", buffer2);
        send(sock, buffer2 , strlen(buffer2), 0 );
        if (strcmp(buffer2,"login") == 0)
        {
            strcpy(buffer, create_call_login(buffer, sock, valread));
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
```
Penjelasan source code client:
Menggunakan socket, dimana saat muncul pilihan login dan register kemudian memilih antara login dan register maka, pilihan tersebut akan dikirim ke server. Jika saat memilih login dan dari server mengirim string login success ke client dari hasil fungsi create_call_login maka dijalankan thread untuk memunculkan screen dua. Sedangkan jika ternyata user memilih register maka dipanggil fungsi create_call_register yang akan memproses register user

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
```

**Penjelasan :**
```c
void *hitung (void * arg)
```
fungsi ini berfungsi untuk melakakukan sortir file hingga memindahakannya ke folder masing - masing. Penjelasan setiap bagiannya sebagai berikut :
``` c
int is_dir(const char* path) {
	struct stat buf;
	stat(path, &buf);
	return S_ISDIR(buf.st_mode);
}

if (!is_dir(arg)){
	strcpy(kopian, arg);
	token = strtok(kopian, "/");

	while( token != NULL ) {
		arr[n] = token;
		n++;
		token = strtok(NULL, "/");	
	}
```
is_dir adalah fungsi untuk melakukan pemeriksaan apkah suatu input itu adalaha directory atau bukan, pada is_dir sendiri kita menyimpan hasil stat (semua informasi tentang inputan kita) kemudian kita ambil bagian st_mode (mode ini memiliki return value tertentu untuk file dan folder). Kemudian, setelah melakukan pengecekan, lanjut melakukan pemotongan string dengan strtok untuk mendapatkan setiap kata yang dipisah dengan "/". Semisal ada kalimat /home/apagitu/main.c,  arr[n] yang merupakan array 2 dimensi akan melakukan penyimpanan seperti berikut :
```
arr[0] = home
arr[1] = apagitu
arr[2] = main.c
```
Tapi sebelumnya path asli harus tetap ada (karena strtok memotong string asli), sehingga kita buat kopiannya lalu dipotong kopiannya itu. Setelah selesai pemotongan string pertama akan dilanjut pemotongan string tahap kedua untuk bagian main.c, potongan kodenya seperti berikut :
```c
strcpy(temp, arr[n-1]);
//printf("%s", temp);
token2 = strtok(temp, ".");

while( token2 != NULL ) {
	arrs[n2] = token2;
	n2++;
	token2 = strtok(NULL, ".");
}
```
Untuk bagian pertama, hasil dari arr[n-1] yaitu main.c diduplikat pada temp lalu dilakukan pemotongan string  berdasarkan titik. Sehingga pada arrs akan berisi seperti berikut 
```c
arrs[0] = main;
arrs[1] = c;
```
Dari arrs tersebut kita ambil bagian ekstensinya, karena dari soal tidak mempermasalahkan *case sensitive* maka kami buat *lowercase* semua dengan fungsi tolower.
```c
strcpy(ektensi, arrs[n2-1]);
for(int i=0 ; i<strlen(ektensi) ;i++){
	ektensi[i] = tolower(ektensi[i]);
}
```
Setelah mendapatkan ekstensi (lowercsase) selanjutnya kita buat folder sesuai ekstensi yang kita dapat dengan fungsi mkdir() dengan parameter pathnya (concat dari /home/syubban/ dan ektensi) dan permission. Ketika n2 == 2 (ada ekstensi) maka buat folder sesuai ekstensi, selain itu buat folder Unknown. 
```c
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
```
Setelah pembuatan folder selesai, langkah selanjutnya adalah pemindahan file-file ke folder-folder yang bersesuaian. Proses pemindahan dilakukan dengan mengkopi data-data biner dari file asal ke file baru (membuat duplikat) yaitu pertama kita buka file asal dengan pf1 (metode baca), lalu kita buka file tujuan (nantinya) dengan metode tulis. File tujuan diperoleh dengan menggabungkan tempo (/home/apagitu/c/) dengan . dan ektensi. Lalu dilakukan perulangan untuk membaca pf1 dan memindahkannya ke pf2 (nama dari asal dan tujuan sama) sampai semua isi dan bentuk file terbentuk pada tujuan. Ketika selesai file asal di hapus dengan fungsi remove() lalu tutup semua pointer pf1 dan pf2.
 
Untuk fungsi hitung2 sama dengan hitung, hanya berbeda pada path tujuannya. Untuk hitung2 digunakan untuk bagian yang menggunakan working path. Selanjutnya fungsi hitung dan hitung2 bertipe void * untuk penyesuaian pada parameter fungsi pembuatan thread. Lalu pembahasan kode pada fungsi main adalah sebagai berikut :
```c
if(strcmp(argv[1], "-f") == 0){
	for(int i=2; i<argc; i++){
		pthread_create(&(tid[i]), NULL, hitung, (void *)argv[i]);
	}
	for(int i=2; i<argc ;i++){
		pthread_join(tid[i], NULL);
	}
}
```
Pada bagian pertama akan dilakukan pengecekan ketika argumen pertama (argv[1]) nya adalah -f maka kita jalankan aturan pertama yaitu memindahkan file dari path inputan ke /home/apagitu/ekstensi. Lalu setiap argumen kami pasing dalam fungsi dan setiap fungsi kami jalankan dalam thread-thread. Setelah loop untuk operasi selesai, thread dijoin kembali. 
```c
else if(strcmp(argv[1], "*") == 0){
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
}
```
Pada bagian ini ketika argumen adalah * maka akan memindahakan semua file pada working directory saat ini ke /home/apagitu/ektensi. Untuk mendapatkan current working directory digunakan fungsi getcwd lalu disimpan pada array global cwd. Setelah itu kita lakukan loop untuk mengambil seluruh file (folder tidak) dan hanya diambil 1 level saja. Lalu kita lakukan pengecekan bahwa current working folder tidak kosong lalu lakukan looping untuk setiap file, pertama kita buka pointer untuk direktori yaitu dir, ketika dir tidak null maka ada isinya, lalu masuk kedalam loop dan ambil nama file tersebut lalu kita concat dengan current working direktori lalu lakukan exception untuk .. dan . ketika melakukan loopin. Setelah itu cek apakah hasil concat adalah file, ketika benar maka kita passing kedalam fungsi hitung melewati thread. Setelah semua file terloop dan terpindah maka join kembali threadnya.
```c
else if(strcmp(argv[1], "-d") == 0){
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
```
Untuk bagian terakhir konsepnya sama dengan * namun yang dipindah dari path input ke current working directory, konsepnya sama semua hanya ada modifikasi pada goal path saja.

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
int (*arrays3)[5], size=1, size1=1;
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
Untuk soal no 4a digunakan thread sebanyak 5, yaitu dimana setiap thread digunakan untuk menghitung hasil perkalian dari setiap kolom matriks, dimana dideklarasikan juga ukuran matriks yang dikalikan, matriks hasil seperti berikut, matriks hasil disini menggunakan pointer karena akan digunakan dalam shared memory. Digunakan juga variabel size dan size1 untuk membantu deklarasi nilai elemen matriks
```c
int arrays1[4][2], arrays2[2][5];
int (*arrays3)[5], size=1, size1=1;``
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
```
key_t key = 1234;
int shmid = shmget(key,sizeof(int[4][5]),0666|IPC_CREAT); 
arrays3 =  shmat(shmid,NULL,0);  
```
Kode diatas yaitu digunakan untuk deklarasi shared memory yang akan digunakan untuk mendapatkan hasil dari array perkalian matriks untuk ditampilkan di nomor selanjutnya. key digunakan sebagai parameter dalam shmget. shmget sendiri akan mereturn identifier untuk shared memory, int[4][5] dalam parameter digunakan sebagai acuan ukuran variabel yang ada di shared memory (array hasil perkalian matriks). Dan setelah identifier shared memory didapat,variabel diattach ke shared memory identifier tadi dengan menggunakan shmat. 

```c
printf("Matriks 1:\n");
    for(int i=0; i<4; i++)
    {
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
        for(int j=0; j<5 ;j++){
            arrays2[i][j] = size1;
            printf("%d ", arrays2[i][j]);
            size1++;
        }
        printf("\n");
    }
    printf("\n");
 ```
 Kode tersebut untuk mencetak matriks yang dikalikan.
 ```c
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
 ```
Kemudian untuk kode diatas, yaitu untuk menjalankan thread dimana menggunakan perulangan while sejumlah (banyak thread - 1) serta fungsi multiplier yang sudah dibuat sebelumnya. Setelah dijalankan, maka semua thread dijoin dengan perintah pthread_join 
```c
printf("Matriks hasil:\n");
    for(int i=0; i<4; i++){
        for(int k=0; k<5; k++)
        {
            printf("%d ", arrays3[i][k]);
        }
        printf("\n");
    }
 ```
Kode tersebut digunakan untuk mencetak matriks hasil perkalian.
Kemudian kode berikut, yaitu shmdt digunakan untuk mendetach variabel dari shared memory. dan exit(0) digunakan untuk keluar dari program
 ```
 shmdt(arrays3);
 exit(0);
 ```
 **Source code nomor 4b:**
 ```c
 #include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
pthread_t tid[5]; //inisisasi banyaknya thread (dalam kasus ini 5 thread)
pid_t child;
int controller = 0;

int arrayshasil[4][5];
int (*arrays3)[5],size=1, size1=1;

void *multiplier(void *arg) 
{
    pthread_t id = pthread_self();
    if(pthread_equal(id, tid[0]))
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][4]; j++)
             {
                 arrayshasil[i][4]+=j;
             }
        }
    }
    else if(pthread_equal(id, tid[1]))
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][3]; j++)
             {
                 arrayshasil[i][3]+=j;
             }
        }
    }
    else if(pthread_equal(id, tid[2]))
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][2]; j++)
             {
                 arrayshasil[i][2]+=j;
             }
        }
    }
    else if(pthread_equal(id, tid[3]))
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][1]; j++)
             {
                 arrayshasil[i][1]+=j;
             }
        }
    }
    else 
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][0]; j++)
             {
                 arrayshasil[i][0]+=j;
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
            arrayshasil[i][j]=0;
        }
    } 

    key_t key = 1234;
    int shmid = shmget(key,sizeof(int[4][5]),0666|IPC_CREAT);  
    arrays3 =  shmat(shmid,NULL,0);  
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

    printf("\nMatriks Penjumlahan:\n");
    for(int i=0; i<4; i++){
        for(int k=0; k<5; k++)
        {
            printf("%d ", arrayshasil[i][k]);
        }
        printf("\n");
    }

    shmdt(arrays3); 
    shmctl(shmid,IPC_RMID,NULL); 
    exit(0);
    return 0; 
} 
```
Penjelasan source code no 4b:
Untuk soal no 4b digunakan thread sebanyak 5, yaitu dimana setiap thread digunakan untuk menghitung hasil penjumlahan dari 1 hingga bilangan di elemen matriks itu sendiri untuk setiap kolom matriks, dimana dideklarasikan juga ukuran matriks yang dikalikan, matriks hasil seperti berikut, matriks hasil disini menggunakan pointer karena akan digunakan dalam shared memory. Digunakan juga variabel size dan size1 untuk membantu deklarasi nilai elemen matriks
```c
int arrays1[4][2], arrays2[2][5];
int (*arrays3)[5], size=1, size1=1;``
```
Kemudian ada fungsi multiplier yang merupakan fungsi yg menjalankan tugas masing-masing thread 
```c
void *multiplier(void *arg) 
{
    pthread_t id = pthread_self();
    if(pthread_equal(id, tid[0]))
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][4]; j++)
             {
                 arrayshasil[i][4]+=j;
             }
        }
    }
    else if(pthread_equal(id, tid[1]))
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][3]; j++)
             {
                 arrayshasil[i][3]+=j;
             }
        }
    }
    else if(pthread_equal(id, tid[2]))
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][2]; j++)
             {
                 arrayshasil[i][2]+=j;
             }
        }
    }
    else if(pthread_equal(id, tid[3]))
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][1]; j++)
             {
                 arrayshasil[i][1]+=j;
             }
        }
    }
    else 
    {
        for (int i=0; i<4; i++) 
        {
             for(int j=1; j<=(int)arrays3[i][0]; j++)
             {
                 arrayshasil[i][0]+=j;
             }
        }
    }
}
```
Dari setiap id thread dicek untuk menjalankan tugasnya yaitu menghitung dan menyimpan nilai hasil penjumlahan dari 1 hingga bilangan di elemen matriks itu sendiri dari setiap kolom. Untuk perhitungannya, seperti matriks pada umumnya yaitu misal untuk menghitung nilai matriks hasil pada elemen (1,1) maka caranya dilakukan perulangan mulai dari 1 hingga bilangan di elemen (1,1) matriks hasil sebelumnya dan ditambahkan elemen matriks hasil baru dengan nilai yang menjadi acuan perulangan.  
```
key_t key = 1234;
int shmid = shmget(key,sizeof(int[4][5]),0666|IPC_CREAT); 
arrays3 =  shmat(shmid,NULL,0);  
```
Kode diatas yaitu digunakan untuk deklarasi shared memory yang akan digunakan untuk mendapatkan hasil dari array perkalian matriks untuk ditampilkan di nomor selanjutnya. key digunakan sebagai parameter dalam shmget. shmget sendiri akan mereturn identifier untuk shared memory, int[4][5] dalam parameter digunakan sebagai acuan ukuran variabel yang ada di shared memory (array hasil perkalian matriks). Dan setelah identifier shared memory didapat,variabel diattach ke shared memory identifier tadi dengan menggunakan shmat. 

```c
printf("Matriks hasil:\n");
    for(int i=0; i<4; i++){
        for(int k=0; k<5; k++)
        {
            printf("%d ", arrays3[i][k]);
        }
        printf("\n");
    }
 ```
 Kode tersebut untuk mencetak matriks yang hasil dari perkalian di nomor sebelumnya.
 ```c
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
 ```
Kemudian untuk kode diatas, yaitu untuk menjalankan thread dimana menggunakan perulangan while sejumlah (banyak thread - 1) serta fungsi multiplier yang sudah dibuat sebelumnya. Setelah dijalankan, maka semua thread dijoin dengan perintah pthread_join 
```c
    printf("\nMatriks Penjumlahan:\n");
    for(int i=0; i<4; i++){
        for(int k=0; k<5; k++)
        {
            printf("%d ", arrayshasil[i][k]);
        }
        printf("\n");
    }
 ```
Kode tersebut digunakan untuk mencetak matriks hasil penjumlahan.
Kemudian kode berikut, yaitu shmdt digunakan untuk mendetach variabel dari shared memory. shmctl digunakan untuk mendestroy shared memory yang dibuat dan exit(0) digunakan untuk keluar dari program
 ```
 shmdt(arrays3);
 shmctl(shmid,IPC_RMID,NULL); 
 exit(0);
 ```
 **Source code nomor 4c:**
 ```c
#include <stdlib.h>
#include <unistd.h>

int pid;
int pipe1[2];

int main()
{
    if (pipe(pipe1) == -1)
        exit(1);

    if ((fork()) == 0)
    {
        // output to pipe1
        dup2(pipe1[1], 1);
        // close fds
        close(pipe1[0]);
        close(pipe1[1]);
        // exec
        char *argv1[] = {"ls", NULL};
        execv("/bin/ls", argv1);
    }
    else
    {
        // input from pipe1
        dup2(pipe1[0], 0);
        close(pipe1[0]);
        close(pipe1[1]);
        // exec
        char *argv1[] = {"wc", "-l", NULL};
        execv("/usr/bin/wc", argv1);
    }

}
```
Penjelasan source code no 4c:
```
int pid;
int pipe1[2];
```
digunakan untuk mendeklarasikan pid dan juga pipe. Pipe yang digunaka yaitu berjumlah 1.
```c
   if (pipe(pipe1) == -1)
        exit(1);

    if ((fork()) == 0)
    {
        // output to pipe1
        dup2(pipe1[1], 1);
        // close fds
        close(pipe1[0]);
        close(pipe1[1]);
        // exec
        char *argv1[] = {"ls", NULL};
        execv("/bin/ls", argv1);
    }
 ```
Kode tersebut yaitu digunakan untuk mengecek pembuatan pipe1 yang akan digunakan fail atau tidak. kemudian digunakan fork untuk mengeksesekusi command. Jika ternyata berada di child (pid==0) maka dilakukan eksekusi untuk command ls. sebelum dieksekusi, digunakan perintah `dup2(pipe1[1], 1);` yang digunakan untuk menulis output dari eksekusi yang akan dijalankan ke pipe1. kemudian pipe1 ditutup dengan perintah 
```
close(pipe1[0]);
close(pipe1[1]);
```
Setelah itu baru dieksekusi command ls dengan perintah execv menggunakan parameter `/bin/ls` 
```c
    else
    {
        // input from pipe1
        dup2(pipe1[0], 0);
        close(pipe1[0]);
        close(pipe1[1]);
        // exec
        char *argv1[] = {"wc", "-l", NULL};
        execv("/usr/bin/wc", argv1);
    }
```
Setelah itu, jika ternyata pid ada di parent maka digunakan perintah `dup2(pipe1[0], 0);` untuk mengambil inputan dari pipe1. Setelah itu pipe1 ditutup dengan perintah 
```
close(pipe1[0]);
close(pipe1[1]);
```
Baru kemudian dieksekusi command wc -l dengan perintah execv menggunakan parameter `usr/bin/wc`

