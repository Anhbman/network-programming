#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//#define PORT 5550   /* Port that will be opened */ 
#define BACKLOG 2   /* Number of allowed connections */
#define BUFF_SIZE 1024

typedef struct account{
	char username[20];
	char password[20];
	int status;
}account;

int n = 0;

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
		sscanf(a,"%s %s %d",acc[n].username,acc[n].password,&acc[n].status);
		n ++;
	}
	//printf("%d\n",i);
	fclose(fp);
	free(a);
	return acc;
}

int checkUserName(account acc[], char username[]){
	for (int i = 0; i < n; i++)
	{
		if(acc[i].username[0] == '\0')
		{
			break;
		}
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

int ghi_file(account tk[]){
	FILE *fp = fopen("account.txt", "w");
	for (int i = 0; i < n; i++)
	{
		fprintf(fp, "%s %s %d\n", tk[i].username, tk[i].password, tk[i].status);
	}
	fclose(fp);
	return 0;
	
}

int main(int argc, char* argv[])
{
	if(argc == 1){
		printf("Please input port number\n");
		return 0;
	}
	int PORT = atoi(argv[1]);
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
	int f = 0;
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
		int k = -1;
		char a[30];
		strcpy(a,"Goodbye ");
		int aa = -1;
		while(1){
			//receives message from client
			// Nhan username
			bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0); //blocking
			if (bytes_received <= 0){
				printf("\nConnection closed");
				break;
			}
			int check = 0;
			recv_data[bytes_received] = '\0';
			if(strncmp("bye",recv_data,3) == 0 && k != -1 && acc[k].status != 0 && aa == 0){
				strcat(a,acc[k].username);
				send(conn_sock, a, strlen(a), 0);
				f=-1;
				break;
			}
			k = checkUserName(acc,recv_data);
			if(k == -1){
				strcpy(recv_data,"0");
				bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
				continue;
			}else{
				aa = -1;
				//Thong bao co tk ton tai
				strcpy(recv_data,"1");
				bytes_sent = send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
				//Nhan mat khau
				int tmp = 0;
				for(int i =0; i <3; i++)
				{
					bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0); //blocking
					if (bytes_received <= 0){
						printf("\nConnection closed");
						break;
					}
					recv_data[bytes_received] = '\0';
					if(checkPass(acc[k],recv_data) == 1){
						if(acc[k].status == 1){
							strcpy(recv_data,"1");
							aa= 0;
							send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
						}else{
							strcpy(recv_data,"3");
							send(conn_sock, recv_data, bytes_received, 0); 
						}
						break;
					}else{
						if(i == 2){
							strcpy(recv_data,"2");
							acc[k].status = 0;
							send(conn_sock, recv_data, bytes_received, 0);
						}else{
							strcpy(recv_data,"0");
							send(conn_sock, recv_data, bytes_received, 0); /* send to the client welcome message */
						}
					}
				}
				
			}
		}
		close(conn_sock);	
		if(f==-1)
			break;
	}
	ghi_file(acc);
	close(listen_sock);
	free(acc);
	return 0;
}
