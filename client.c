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

  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  serverAddr.sin_family = AF_INET;
 
  serverAddr.sin_port = htons(7891);

  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  while(1){
	  scanf("%s", buffer);
	  send(clientSocket,buffer, 10,0);
	  strcpy(buffer,"");
	  recv(clientSocket, buffer, 10, 0);
	  printf("Data received from server: %s\n",buffer);
	  strcpy(buffer,"");
  }
  return 0;
}
