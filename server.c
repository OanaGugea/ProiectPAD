#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

typedef struct _head{
	char tipMesaj; // c - comanda; e - eroare; m - mesaj
	int nrCaractere; //lungimea mesajului
}head;

typedef struct _body{
	char *bodyMessage; //mesajul propriu-zis
}body;


head *primul;
body *alDoilea;
char parola[100];
int allSocket[10];
int allClients;

void userExists(char *name){
	FILE *fisier;
	fisier=fopen("dateMembrii.txt", "r");
	char temp[100];
	strcpy(parola,"");
	while(fscanf(fisier, "%s", temp)>0){
		printf("Membrii: %s\n", temp);
		if(strcmp(temp, name)==0){
			fscanf(fisier, "%s", parola);
			printf("Parola: %s\n", parola);
			break;
		}
		fscanf(fisier, "%s", temp);
	}
	fclose(fisier);
}

void getID(int newSocket){
	//free(alDoilea->bodyMessage);
	alDoilea->bodyMessage=(char *)malloc(20);
	strcpy(alDoilea->bodyMessage, "Dati user name:");
	primul->tipMesaj='c';
	primul->nrCaractere=strlen(alDoilea->bodyMessage);

	send(newSocket, primul, sizeof(head),0);
	send(newSocket, alDoilea->bodyMessage, (primul->nrCaractere+1),0);
	free(alDoilea->bodyMessage);

	recv(newSocket, primul, sizeof(head), 0);
	alDoilea->bodyMessage=(char *)malloc(primul->nrCaractere+1);
	recv(newSocket, alDoilea->bodyMessage, (primul->nrCaractere+1), 0);

	printf("ID: %s\n", alDoilea->bodyMessage);

	strcpy(parola,"");
	userExists(alDoilea->bodyMessage);

	free(alDoilea->bodyMessage);
}
void getPassword(int newSocket){
	//free(alDoilea->bodyMessage);
	alDoilea->bodyMessage=(char *)malloc(20);
	strcpy(alDoilea->bodyMessage, "Dati parola:");

	primul->tipMesaj='c';
	primul->nrCaractere=strlen(alDoilea->bodyMessage);

	send(newSocket, primul, sizeof(head),0);
	send(newSocket, alDoilea->bodyMessage, (primul->nrCaractere+1),0);
	free(alDoilea->bodyMessage);

	recv(newSocket, primul, sizeof(head), 0);
	alDoilea->bodyMessage=(char *)malloc(primul->nrCaractere+1);
	recv(newSocket, alDoilea->bodyMessage, (primul->nrCaractere+1), 0);

	printf("Parola: %s\n", alDoilea->bodyMessage);

	//free(alDoilea->bodyMessage);
}	
void *executeThread(void *socket){
	int newSocket=*(int *)socket;

	getID(newSocket);

	while(strcmp(parola, "")==0){
		//free(alDoilea->bodyMessage);
		alDoilea->bodyMessage=(char *)malloc(20);
		strcpy(alDoilea->bodyMessage, "User-ul nu exista!");
		primul->tipMesaj='e';
		primul->nrCaractere=strlen(alDoilea->bodyMessage);

		send(newSocket, primul, sizeof(head),0);
		send(newSocket, alDoilea->bodyMessage, (primul->nrCaractere+1),0);

		free(alDoilea->bodyMessage);
		
		getID(newSocket);
	}

	getPassword(newSocket);

	while(strcmp(parola, alDoilea->bodyMessage)!=0){
		free(alDoilea->bodyMessage);
		alDoilea->bodyMessage=(char *)malloc(20);

		strcpy(alDoilea->bodyMessage, "Parola incorecta!");
		primul->tipMesaj='e';
		primul->nrCaractere=strlen(alDoilea->bodyMessage);

		send(newSocket, primul, sizeof(head),0);
		send(newSocket, alDoilea->bodyMessage, (primul->nrCaractere+1),0);

		free(alDoilea->bodyMessage);
		
		getPassword(newSocket);
	}

	alDoilea->bodyMessage=(char *)malloc(25);

	strcpy(alDoilea->bodyMessage, "Bine ati venit la chat!");
	primul->tipMesaj='m';
	primul->nrCaractere=strlen(alDoilea->bodyMessage);

	send(newSocket, primul, sizeof(head),0);
	send(newSocket, alDoilea->bodyMessage, (primul->nrCaractere+1),0);
	free(alDoilea->bodyMessage);

	while(1){
		recv(newSocket, primul, sizeof(head), 0);
		alDoilea->bodyMessage=(char *)malloc(primul->nrCaractere+1);

		recv(newSocket, alDoilea->bodyMessage, (primul->nrCaractere+1), 0);
		printf("Data received from client: %s\n", alDoilea->bodyMessage);
	
		int k;
		for(k=0;k<allClients;k++){
				send(allSocket[k], primul, sizeof(head),0);
				send(allSocket[k], alDoilea->bodyMessage, (primul->nrCaractere+1),0);
		}
		free(alDoilea->bodyMessage);
	}	
}
int main(){
	int welcomeSocket; 
	char buffer[1024];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	FILE *f;
	char ip[15];
	int port;
	int lungime;
	pthread_t fire[10];
	int i=0, j;
	allClients=0;

	f=fopen("client.txt","r");
	fscanf(f,"%s", ip);
	fscanf(f,"%d", &port);
	fclose(f);

	primul = (head *)malloc(sizeof(head));
	alDoilea = (body *)malloc(sizeof(body));

	//socket()
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

	//configurare structura pentru adresa serverului
	//familia de protocoale TCP/IP
	serverAddr.sin_family = AF_INET;
	//numar port
	serverAddr.sin_port = htons(port);
	//adresa IP (local host)
	serverAddr.sin_addr.s_addr = inet_addr(ip);
 
	//bind()
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	//listen()
	if(listen(welcomeSocket,5)==0)
		printf("Waiting for clients...\n");
	else
		printf("Error\n");
  

	while(1){
		//accept()
		addr_size = sizeof serverStorage;
		allSocket[i] = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

		printf("Connection accepted!\n");

		if((pthread_create(&fire[i], NULL, executeThread, &allSocket[i]))!=0){
			printf("Eroare la crearea thread-ului!");
			exit(1);
		}
		allClients++;

		while(i>=9){
			printf("Numarul maxim de clienti a fost atins!");
			sleep(1000);
		}
		i++;

	}
	for(j=0;j<i;j++)
			pthread_join(fire[j], NULL);
	free(primul);
	free(alDoilea);
	return 0;
}
