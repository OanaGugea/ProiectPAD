#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>

typedef struct _head{
	char tipMesaj; // c - comanda; e - eroare; m - mesaj
	int nrCaractere; //lungimea mesajului
}head;

typedef struct _body{
	char *bodyMessage; //mesajul propriu-zis
}body;

int main(){
	FILE *f;
	int clientSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	char ip[15];
	int port;
	int lungime;
	int count=0;
	head *primul = (head *)malloc(sizeof(head));
	body *alDoilea = (body *)malloc(sizeof(body));

	f=fopen("client.txt","r");
	fscanf(f,"%s", ip);
	fscanf(f,"%d", &port);
	fclose(f);

	//socket()
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	//configurare structura pentru adresa serverului
	//familia de protocoale TCP/IP
	serverAddr.sin_family = AF_INET;
	//numar port
	serverAddr.sin_port = htons(port);
	//adresa IP (local host)
	serverAddr.sin_addr.s_addr = inet_addr(ip);

	//connect()
	connect(clientSocket, (struct sockaddr *) &serverAddr, (sizeof serverAddr));

	printf("Pentru iesire tastati <end>\n");

	while(1){
		//free(alDoilea->bodyMessage);

		recv(clientSocket, primul, sizeof(head), 0);
		alDoilea->bodyMessage=(char *)malloc(primul->nrCaractere+1);
		recv(clientSocket, alDoilea->bodyMessage, (primul->nrCaractere+1), 0);

		if(primul->tipMesaj=='e'){
			printf("Mesaj de eroare: %s\n", alDoilea->bodyMessage);
			//printf("Server: %s\n", alDoilea->bodyMessage);
			printf("\n");
			free(alDoilea->bodyMessage);
		}
		else if(primul->tipMesaj=='c'){
			printf("Server: %s\n", alDoilea->bodyMessage);
			printf("\n");
			free(alDoilea->bodyMessage);

			strcpy(buffer,"");
			printf("Client comanda: ");
			scanf("%s", buffer);

			lungime=strlen(buffer);
			primul->tipMesaj='c';
			primul->nrCaractere=lungime;

			alDoilea->bodyMessage=(char *)malloc(lungime+1);
			strcpy(alDoilea->bodyMessage,buffer);

			send(clientSocket, primul, sizeof(head),0);
			send(clientSocket, alDoilea->bodyMessage, (lungime+1),0);
			
			free(alDoilea->bodyMessage);
		}
		else{
			printf("Server: %s\n", alDoilea->bodyMessage);
			printf("\n");
			free(alDoilea->bodyMessage);

			strcpy(buffer,"");
			printf("Client: ");
			scanf("%s", buffer);
			//fgets(buffer, 100, stdin);

			lungime=strlen(buffer);
			primul->tipMesaj='m';
			primul->nrCaractere=lungime;

			alDoilea->bodyMessage=(char *)malloc(lungime+1);
			strcpy(alDoilea->bodyMessage,buffer);

			send(clientSocket, primul, sizeof(head),0);
			send(clientSocket, alDoilea->bodyMessage, (lungime+1),0);

			free(alDoilea->bodyMessage);

			/*while(1){
				strcpy(buffer,"");
				printf("Client: ");
				scanf("%s", buffer);
				//fgets(buffer, 100, stdin);

				lungime=strlen(buffer);
				primul->tipMesaj='m';
				primul->nrCaractere=lungime;

				alDoilea->bodyMessage=(char *)malloc(lungime+1);
				strcpy(alDoilea->bodyMessage,buffer);

				send(clientSocket, primul, sizeof(head),0);
				send(clientSocket, alDoilea->bodyMessage, (lungime+1),0);

				free(alDoilea->bodyMessage);
				recv(clientSocket, primul, sizeof(head), 0);
				alDoilea->bodyMessage=(char *)malloc(primul->nrCaractere+1);
				recv(clientSocket, alDoilea->bodyMessage, (primul->nrCaractere+1), 0);

				printf("Server: %s\n", alDoilea->bodyMessage);
				printf("\n");
				free(alDoilea->bodyMessage);

				ioctl(clientSocket, FIONREAD, &count);
				if(count>0){
					recv(clientSocket, primul, sizeof(head), 0);
					alDoilea->bodyMessage=(char *)malloc(primul->nrCaractere+1);
					recv(clientSocket, alDoilea->bodyMessage, (primul->nrCaractere+1), 0);
					printf("Alt client: %s\n", alDoilea->bodyMessage);
				}
			}*/

		}

		if(strcmp(buffer, "end")==0){
			printf("La revedere!\n");
			exit(1);
		}
	}
	free(primul);
	free(alDoilea);
	return 0;
}
