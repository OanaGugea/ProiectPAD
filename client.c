#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
	int clientSocket;
	char buffer[1024], buff[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	//socket()
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	//configurare structura pentru adresa serverului
	//familia de protocoale TCP/IP
	serverAddr.sin_family = AF_INET;
	//numar port
	serverAddr.sin_port = htons(7891);
	//adresa IP (local host)
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//connect()
	connect(clientSocket, (struct sockaddr *) &serverAddr, (sizeof serverAddr));

	while(1){
		printf("Client: ");
		scanf("%s", buffer);
		send(clientSocket,buffer, 10,0);
		strcpy(buffer,"");
		recv(clientSocket, buffer, 10, 0);
		printf("Server: %s\n",buffer);
		strcpy(buffer,"");
		printf("\n");
	}
	return 0;
}
