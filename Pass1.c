#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *source = fopen("Celcius-Farenheit.txt","r");
    FILE *intermediate = fopen("CF-intermediate.txt","w");

    // test for files not existing.
    if (source == NULL)
    {
        printf("Error! Could not open file\n");
        return 0;
    }

    char Label[10],instruction[10],operand[10],ProgramName[10];
    char *end;
    int LOCCTR;
    do
    {
        fscanf(source,"%s %s %s",Label,instruction,operand);
    }while(strcmp(instruction,"START")!=0);
    LOCCTR = strtol(operand,&end,16);
    strcpy(ProgramName,Label);
    return 0;
}
