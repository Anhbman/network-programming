// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
void process_string();
#define BUFF_SIZE 8192
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Please input IP address and port number\n");
		return 0;
	}
	// ip_address of server
	// port number
	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);
	int sock = 0;
	struct sockaddr_in serv_addr;

	// Try catch false when connecting
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	int valread = 0;
	char buffer[1024] = {0};
	char buffer2[1024] = {0};
	int g = 0;
	char message[100];
		char mes1[1024] = "Login is successful!";
		char mes2[1024] = "Account is blocked or inactive";
	do
	{
		
		printf("INPUT: ");
		g = scanf("%[^\n]", message);
		if (g == 0)
			break;
		getchar();
		// nhap tk 
		send(sock, message, strlen(message), 0);
		//valread = recv(sock, buffer2,)
		// get message from server
		memset(buffer, '\0', (strlen(buffer) + 1));
		// doc ket qua tai khoan
		valread = read(sock, buffer, 1024);
		printf("OUTPUT: \n%s\n", buffer);
		char b1[100] = "tai khoan ko ton tai";
		if (strcmp(b1, buffer) == 0)
		{
			puts("-------------------------");
			continue;
		}
		else
		{
			int h = 1;
			do
			{
				g = scanf("%[^\n]", message); // mat khau
				getchar();
				send(sock, message, strlen(message), 0);
				memset(buffer, '\0', (strlen(buffer) + 1));
				valread = read(sock, buffer, 1024);
				printf("\n%s\n", buffer);
				if(strcmp(buffer,mes1 ) == 0 ){
					while(1){
						printf("nhap bye de thoat\n");
						scanf("%[^\n]", message); // mat khau
						getchar();
						if(strcmp(message,"bye") == 0){
							break;
						}
					}
					send(sock, message, strlen(message), 0);
					memset(buffer, '\0', (strlen(buffer) + 1));
					valread = read(sock, buffer, 1024);
					printf("\n%s\n", buffer);
					puts("-------------------------");
					break;
				}
				else if(strcmp(buffer, mes2) == 0){
					puts("-------------------------");
					break;
				}
				else{
					h = h + 1;
					if(h > 3){
						puts("-------------------------");
						break;

					}
						printf("nhap lai lan %d\n", h );
					
				}
				
			} while (h <= 3);
		}

	} while (1);

	return 0;
}

