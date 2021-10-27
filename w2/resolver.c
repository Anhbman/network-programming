#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/socket.h>


int main(int argc, char const *argv[]) {
    if(argc == 1){
        printf("No input\n");
        return 0;
    }
    int i;
    int k = 0;
    //unsigned char ip[] = {126, 58, 99, 199};
    //printf("%d\n",sizeof(argv[1]));
    char* name= (char*)malloc(sizeof(argv[1]));
    strcpy(name,argv[1]);
    struct in_addr addr;
    addr.s_addr = inet_addr(name);
    struct hostent *initial;
    struct in_addr IP;
   if(name[0] >= '0' && name[0] <= '9'){
       k = 1;
   }
    
    if (k == 1 && (initial = gethostbyaddr((char *) &addr, 4, AF_INET)) != NULL){
        printf("Official name: %s\n", initial->h_name);
        printf("Alias name:\n");
        for (i = 0; initial->h_aliases[i] != NULL; i++)
            printf("%s\n",initial -> h_aliases[i]);
    }else if (k==0 && (initial = gethostbyname(name)) != NULL){
        IP.s_addr = *((uint32_t*) initial->h_addr_list[0]);
        printf("Offical IP: %s\n",inet_ntoa(IP));
        printf("Alias IP: \n");
        for (i = 1; initial->h_addr_list[i] != NULL; i++) {
            IP.s_addr = *((uint32_t*) initial->h_addr_list[i]);
            printf("%s\n", inet_ntoa(IP));
        }
    }else{
        printf("No Information found\n");
    }
    
    free(name);

}