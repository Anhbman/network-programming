#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int checkCode(char *code){
    if (code[0] != '1' && code[0] != '2' && code[0] != '3' && code[0] != '4' )
    {
        printf("0Mã HAZCHEM sai\n");
        return 0;
    }
    if (code[1] != 'P' && code[1] != 'R' && code[1] != 'S' && code[1] != 'T' 
        && code[1] != 'W' && code[1] != 'X' && code[1] != 'Y' && code[1] != 'Z' )
    {
        printf("1Mã HAZCHEM sai\n");
        return 0;
    }

    if (code[2] != '\0' && code[2] != 'E')
    {
        printf("2Mã HAZCHEM sai\n");
        return 0;
    }

    return 1;
}

int main(){

    char* code = (char*)malloc(sizeof(char)*4);
    char* check = (char*)malloc(sizeof(char)*4);
    int t = 0;
    printf("\nEnter HAZCHEM code:");
    scanf("%s",code);
    
    if (!checkCode(code))
    {
        return 0;
    }
    
    if (code[2] != '\0' && code[2] == 'E')
    {
        t = 1;
    }
    
    if (code[1] == 'S' || code[1] == 'T' || code[1] == 'Y' || code[1] == 'Z' )
    {
        printf("Is the %c reverse coloured? ",code[1]);
        scanf("%s",check);
    }
    
    printf("\n\n***Emergency action advice***\n\n");
    printf("Material:\t");
    switch (code[0])
    {
    case '1':
        printf("Jets\n");
        break;
    case '2':
        printf("Fog\n");
        break;
    case '3':
        printf("Foam\n");
        break;
    case '4':
        printf("Dry agent\n");
        break;
    }

    switch (code[1])
    {
    case 'P':
    case 'S':
    case 'W':
    case 'Y':
    case 'Z':
        printf("Reactivity:\t");
        printf("can be violently reactive\n");
        break;
    default:
        printf("\n");
        break;
    }
    printf("Protection:\t");
    switch (code[1])
    {
    case 'P':
    case 'R':
    case 'W':
    case 'X':
        printf("full protective clothing must be worn\n");
        break;
    default:
        if(strcmp("yes",check) == 0)
            printf("breathing apparatus, protective gloves for fire only\n");
        else
            printf("breathing apparatus, protective gloves\n");
        break;
    }

    printf("Containment:\t");

    switch (code[1])
    {
    case 'P':
    case 'R':
    case 'S':
    case 'T':
        printf("may be diluted and washed down the drain\n");
        break;
    default:
        printf("a need to avoid spillages from entering drains or water courses\n");
        break;
    }
    if (t == 1)
    {
        printf("Evacuation:\tconsider evacuation\n");
    }
    
    printf("\n*****************************\n");
    free(code);
    free(check);
}