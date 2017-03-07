#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
	int welcomeSocket, newSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	//socket()
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

	//configurare structura pentru adresa serverului
	//familia de protocoale TCP/IP
	serverAddr.sin_family = AF_INET;
	//numar port
	serverAddr.sin_port = htons(7891);
	//adresa IP (local host)
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
	//bind()
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	//listen()
	if(listen(welcomeSocket,5)==0)
		printf("Listening\n");
	else
		printf("Error\n");
  
	//accept()
	addr_size = sizeof serverStorage;
	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

	while(1){
		strcpy(buffer,"");
		recv(newSocket, buffer, 10, 0);
		printf("Data received from client: %s\n",buffer);
		send(newSocket,buffer, 10,0);
	}
	return 0;
}
