#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 8192

int main(){
	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received;
	
	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
	int check = 0;
	//Step 4: Communicate with server			
	while(1){
		//send message
		printf("\nInput username:");
		memset(buff,'\0',(strlen(buff)+1));
		fgets(buff, BUFF_SIZE, stdin);		
		msg_len = strlen(buff);
		if (msg_len == 0) break;
		buff[strlen(buff) -1] = '\0';
		bytes_sent = send(client_sock, buff, msg_len, 0);
		if(bytes_sent <= 0){
			printf("\nConnection closed!\n");
			break;
		}
		
		//receive echo reply
		bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
		if(bytes_received <= 0){
			printf("\nError!Cannot receive data from sever!\n");
			break;
		}
		
		buff[bytes_received] = '\0';
		if (buff[0] == '0')
		{
			printf("Tai khoan khong ton tai!\n");
			break;
		}else{
			printf("Please input password: ");
			memset(buff,'\0',(strlen(buff)+1));
			fgets(buff, BUFF_SIZE, stdin);	
			buff[strlen(buff) -1] = '\0';
			bytes_sent = send(client_sock, buff, strlen(buff), 0);
			if(bytes_sent <= 0){
				printf("\nConnection closed!\n");
				break;
			}	
			bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
			if(bytes_received <= 0){
				printf("\nError!Cannot receive data from sever!\n");
				break;
			}
			buff[bytes_received] = '\0';
			if (buff[0] == '1')
			{
				printf("Login is sccessful!\n");
				check = 1;
				continue;
			}else{
				int t = 0;
				while (1)
				{
					printf("Password is not correct. Please try again!\n ");
					memset(buff,'\0',(strlen(buff)+1));
					fgets(buff, BUFF_SIZE, stdin);	
					buff[strlen(buff) -1] = '\0';
					bytes_sent = send(client_sock, buff, strlen(buff), 0);
					if(bytes_sent <= 0){
						printf("\nConnection closed!\n");
					break;
					}	
					bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
					printf("Buff: %s\n",buff);
					if(bytes_received <= 0){
						printf("\nError!Cannot receive data from sever!\n");
						break;
					}
					buff[bytes_received] = '\0';
					if (buff[0]=='0')
					{
						continue;
					}else if (buff[0]=='2')
					{
						printf("Account is blocked!\n");
						break;
					}else if(buff[0] == '1'){
						printf("Login is successful!\n");
						check = 1;
						break;
					}else if(buff[0] == '3'){
						printf("Account is blocked or inactive!\n");
						check = 1;
						break;
					}
				}
				
			}
			
		}
		// printf("Input username: ");
		// memset(buff,'\0',(strlen(buff)+1));
		// fgets(buff, BUFF_SIZE, stdin);	
		// bytes_sent = send(client_sock, buff, strlen(buff), 0);
		// if(bytes_sent <= 0){
		// 	printf("\nConnection closed!\n");
		// break;
		// }	
		// bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
		// if(bytes_received <= 0){
		// 	printf("\nError!Cannot receive data from sever!\n");
		// 	break;
		// }
		// buff[bytes_received] = '\0';
		// if
	}
	
	//Step 4: Close socket
	close(client_sock);
	return 0;
}
