// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct tai_khoan
{
	char name[20];
	char pass[20];
	int tmp;
};
struct tai_khoan tk[10];

int doc_file()
{
	FILE *fp = fopen("account.txt", "r");
	for (int i = 0; i <= 1; i++)
	{
		fscanf(fp, "%s %s %d", tk[i].name, tk[i].pass, &tk[i].tmp);
	}
	for (int i = 0; i <= 1; i++)
	{
		printf("%s %s %d\n", tk[i].name, tk[i].pass, tk[i].tmp);
	}
	fclose(fp);
	return 0;
	
}

int ghi_file(){
	FILE *fp = fopen("account.txt", "w");
	for (int i = 0; i <= 1; i++)
	{
		fprintf(fp, "%s %s %d\n", tk[i].name, tk[i].pass, tk[i].tmp);
	}
	fclose(fp);
	return 0;
	
}
// split string into only_string and only_number

int main(int argc, char *argv[])
{
	// catch wrong input
	if (argc == 1)
	{
		printf("Please input port number\n");
		return 0;
	}
	char *port_number = argv[1];
	int port = atoi(port_number);
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	// Forcefully attaching socket to the port
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	doc_file();
	int valread = 0;
	char buffer[1024] = {0};
	char buffer1[1024] = {0};
	char mes4[1024] = "Account is blocked!";
	char mes1[1024] = "Login is successful!";
	char mes2[1024] = "Account is blocked or inactive";
	char mes3[1024] = "Password is not correct. Please try again!";
	char mes5[1024] ="bye ";
	char only_string[100] = "Please enter the login password:";
	char only_number[100] = "tai khoan ko ton tai";
	int k = 0, d = 2;
	while (1)
	{
		memset(buffer, '\0', (strlen(buffer) + 1));
		// nhan tai khoan
		valread = read(new_socket, buffer, 1024);
		for (int i = 0; i <= 1; i++){
			//printf ("%s %s\n", buffer, tk[i].name);
			if (strcmp(buffer, tk[i].name) == 0){
				k = 1;
				d = i;
			}
		}
		if (k == 1){
			// gui thong bao tk
			send(new_socket, only_string, strlen(only_string), 0);
			int b = 1;
			do{
				memset(buffer1, '\0', (strlen(buffer1) + 1));
				read(new_socket, buffer1, 1024); // nhan mat khau
				if (strcmp(buffer1, tk[d].pass) == 0){
					if (tk[d].tmp == 1){
						send(new_socket, mes1, strlen(mes1), 0);
						memset(buffer1, '\0', (strlen(buffer1) + 1));

						read(new_socket, buffer1, 1024);
						printf("%s\n", buffer1);
						strcat(mes5, tk[d].name);
						send(new_socket, mes5,strlen(mes5), 0);
					}
					else{
						send(new_socket, mes2, strlen(mes2), 0);
					}
					break;
				}
				else{
					b = b +1;
					if(b > 3){
						send(new_socket, mes4, strlen(mes4), 0);
						tk[d].tmp = 0;
						ghi_file();
						break;
					}
					else
						send(new_socket, mes3, strlen(mes3), 0);
				}
			} while (b <= 3);
			k = 0;
		}
		else{
			send(new_socket, only_number, strlen(only_number), 0);
		}
	}
	return 0;
}

