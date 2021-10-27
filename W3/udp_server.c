/*UDP Echo Server*/
#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include<stdlib.h>

#define PORT 5550  /* Port that will be opened */ 
#define BUFF_SIZE 1024

int XuLyXau(char * buffer, char * only_number,char * only_string){
	int k=0;
	int j=0;

	int m = 0;
	for(int i=0; i<100 ;i++){
		char ch = buffer[i];
		if (ch == '\0'){
			break;
		}else if(ch == '\n'){
			break;
		}
		else if(ch >= '0' && ch <= '9'){
    	    only_number[j] = ch;
    	   	j++;
   		}
   		else if ((ch >= 'a' && ch <= 'z')||(ch == ' ')){
   			only_string[k] = ch;
   			k++;
   		}else{
		   return 0;
		}
   	}
	
   	only_number[j] = '\0'; 
	only_string[k] = '\0'; 
	return 1;

}

void response(int server_sock, char* OutputNumber, char* OutputString, int bytes_received,struct sockaddr_in tmp, struct sockaddr_in client1,struct sockaddr_in client2, int sin_size){
	int bytes_sent;
	if(tmp.sin_port == client1.sin_port){
		bytes_sent = sendto(server_sock, OutputNumber, bytes_received, 0, (struct sockaddr*)&client2, sin_size); 
		if (bytes_sent < 0)
			perror("\nError: ");
		bytes_sent =  sendto(server_sock, OutputString, bytes_received, 0, (struct sockaddr*)&client2, sin_size); 					
		if (bytes_sent < 0)
			perror("\nError: ");
	}else{
		bytes_sent = sendto(server_sock, OutputNumber, bytes_received, 0, (struct sockaddr*)&client1, sin_size); 
		if (bytes_sent < 0)
			perror("\nError: ");
		bytes_sent =  sendto(server_sock, OutputString, bytes_received, 0, (struct sockaddr*)&client1, sin_size); 					
		if (bytes_sent < 0)
			perror("\nError: ");
	}
}

int main(int argc, char *argv[])
{

	if(argc==1){
		printf("No port number\n");
		return 0;
	}

	char * port_Number = argv[1];
	int port = atoi(port_Number);
	int server_sock; /* file descriptors */
	char buff[BUFF_SIZE];
	char buff1[BUFF_SIZE];
	int bytes_sent, bytes_received,bytes_received1;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	struct sockaddr_in client2; /* client2's address information */
	struct sockaddr_in tmp,tmp1; /* tmp's address information */
	int sin_size;

	// memset((char *) &client2, 0, sizeof(client2));
	// client2.sin_family = AF_INET;
	// // client2.sin_port = htons(CLIENT2_PORT);
	// // if(inet_aton(CLIENT2_HOST, &client2.sin_addr) == 0)
	// // 	perror("inet_aton");
	char OutputString[100];
	char OutputNumber[100];
	//Step 1: Construct a UDP socket
	if ((server_sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		exit(0);
	}
	
	//Step 2: Bind address to socket
	server.sin_family = AF_INET;         
	server.sin_port = htons(port);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = INADDR_ANY;  /* INADDR_ANY puts your IP address automatically */   
	bzero(&(server.sin_zero),8); /* zero the rest of the structure */

  
	if(bind(server_sock,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ /* calls bind() */
		perror("\nError: ");
		exit(0);
	}     
	
	//Step 3: Communicate with clients
	int step = 0;
	while(1){
		sin_size=sizeof(struct sockaddr_in);
		memset(buff,0,BUFF_SIZE);
		memset(buff1,0,BUFF_SIZE);
		memset(OutputNumber,0,100);
		memset(OutputString,0,100);
		bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &tmp, &sin_size);
		if (step == 0)
		{
			client = tmp;
			step=1;
		}
		
		bytes_received1 = recvfrom(server_sock, buff1, BUFF_SIZE-1, 0, (struct sockaddr *) &tmp1, &sin_size);
		if (step == 1 && tmp1.sin_port != client.sin_port)
		{
			client2 = tmp1;
			step=2;
		}
		if (step == 2)
		{
			buff[bytes_received] = '\0';
			buff1[bytes_received1] = '\0';
			//printf("[%s:%d]: %s", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buff);
			//printf("Server: %s\n",buff);
			if (!XuLyXau(buff,OutputNumber,OutputString))
			{
				strcpy(OutputNumber,"error");
				strcpy(OutputString,"");
			}

			response(server_sock,OutputNumber,OutputString,bytes_received,tmp,client,client2,sin_size);
			memset(OutputNumber,0,100);
			memset(OutputString,0,100);
			if (!XuLyXau(buff1,OutputNumber,OutputString))
			{
				strcpy(OutputNumber,"error");
				strcpy(OutputString,"");
			}
			response(server_sock,OutputNumber,OutputString,bytes_received,tmp1,client,client2,sin_size);
		}
	}
	
	close(server_sock);
	return 0;
}
