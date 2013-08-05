#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct OPTAB1{
    char *Mnemonic;
    char *OPCODE;
}OPTAB;

OPTAB* getoptab()
{
    OPTAB optab[5];
    optab[0].Mnemonic = "LDA";
    optab[0].OPCODE = "00";
    optab[1].Mnemonic = "MUL";
    optab[1].OPCODE = "20";
    optab[2].Mnemonic = "DIV";
    optab[2].OPCODE = "24";
    optab[3].Mnemonic = "STA";
    optab[3].OPCODE = "0C";
    optab[4].Mnemonic = "ADD";
    optab[4].OPCODE = "18";
    return optab;
}

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

    OPTAB *optab;
    optab= getoptab();

    char *Label,*instruction,*operand,ProgramName[10];
    char *end,line[250];
    int LOCCTR,STARTLOC;
    do
    {
        fscanf(source,"%s %s %s",Label,instruction,operand);
        //fgets(line, sizeof(line), source);
    }while(strcmp(instruction,"START")!=0);
    LOCCTR = strtol(operand,&end,10);
    STARTLOC = LOCCTR;
    strcpy(ProgramName,Label);
    //fgets(line, sizeof(line), source);
    while(1)
    {
        char *L,*I,*O;
        //fgets(line, sizeof(line), source);
        //puts(line);
        //fscanf(source,"%[^\n]s",line);
        L=strtok(line," ");
        I=strtok(NULL," ");
        O=strtok(NULL," ");
        if(O==NULL){
            strcpy(instruction,L);
            strcpy(operand,I);
            printf("NOL %s %s\n",instruction,operand);
        }
        else{
            strcpy(Label,L);
            strcpy(instruction,I);
            strcpy(operand,O);
            printf("%s %s %s\n",Label,instruction,operand);
        }
    }
    return 0;
}
