#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 5550   /* Port that will be opened */ 
#define BACKLOG 2   /* Number of allowed connections */
#define BUFF_SIZE 1024

typedef struct account{
	char username[20];
	char password[20];
	int status;
}account;

account* DocFile(account acc[]){
	FILE *fp = NULL;
	int i = 0;
	fp = fopen("account.txt","r");
	if(fp == NULL){
		return NULL;
	}
	char* a;
	a = (char*)malloc(sizeof(char)*200);
	while (fgets(a, 200, fp) != NULL)
	{
		if(strlen(a) < 3){
			continue;
		}
		sscanf(a,"%s %s %d",acc[i].username,acc[i].password,&acc[i].status);
		printf("%s\t%s\t%d\n",acc[i].username,acc[i].password,acc[i].status);
		i++;
	}
	
	fclose(fp);
	free(a);
	return acc;
}

int checkUserName(account acc[], char username[]){
	printf("%d\n",strlen(username));
	for (int i = 0; i < 200; i++)
	{
		if(acc[i].username[0] == '\0')
		{
			break;
		}
		printf("%s %d\n",acc[i].username,strlen(acc[i].username));
		if(strcmp(username,acc[i].username) == 0)
			return i;
	}
	return -1;
}

int checkPass(account acc, char pass[]){
	if(strcmp(acc.password,pass) == 0){
		return 1;
	}
	return -1;
}

int main(int argc, char* argv[])
{
	account* acc = (account*)malloc(sizeof(account)*200);
	acc = DocFile(acc);
	int listen_sock, conn_sock; /* file descriptors */
	char recv_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	
	//Step 1: Construct a TCP socket to listen connection request
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	
	//Step 2: Bind address to socket
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(PORT);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   
	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	}     
	
	//Step 3: Listen request from client
	if(listen(listen_sock, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}
	
	//Step 4: Communicate with client
	while(1){
		//accept request
		sin_size = sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock,( struct sockaddr *)&client, &sin_size)) == -1) 
			perror("\nError: ");
  
		printf("You got a connection from %s\n", inet_ntoa(client.sin_addr) ); /* prints client's IP */
		
		//start conversation
		while(1){
			//receives message from client
			bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0); //blocking
			if (bytes_received <= 0){
				printf("\nConnection closed");
				break;
			}
			else{
				int check = 0;
				recv_data[bytes_received] = '\0';
				int k = checkUserName(acc,recv_data);
				printf(" k = %d",k);
				if(k == -1){
					strcpy(recv_data,"0");
					bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
				}else{
					strcpy(recv_data,"1");
					bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
					for (int i = 0; i < 3; i++)
					{
						bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0); //blocking
						if (bytes_received <= 0){
							printf("\nConnection closed");
							break;
						}
						recv_data[bytes_received] = '\0';
						if(checkPass(acc[k],recv_data) == 1){
							check = 1;
							break;
						}
						strcpy(recv_data,"0");
						bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
					}
					if(check == 0){
						strcpy(recv_data,"2");
						bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
						acc[k].status = 0;
					}else if(check == 1 && acc[k].status == 1){
						strcpy(recv_data,"1");
						bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
					}else if(check == 1 && acc[k].status == 0){
						strcpy(recv_data,"3");
						bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
					}
				}
			}
			
			//echo to client
			// bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
			// if (bytes_sent <= 0){
			// 	printf("\nConnection closed");
			// 	break;
			// }
		}//end conversation
		close(conn_sock);	
	}
	
	close(listen_sock);
	free(acc);
	return 0;
}
