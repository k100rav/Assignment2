#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct OPTAB1{
    char *Mnemonic;
    char *OPCODE;
}OPTAB;

typedef struct SYMTAB1{
    char LABEL[10];
    int LOCCTR;
}SYMTAB;

SYMTAB *symtab=NULL,*more_symtab=NULL;
int symcounter=0;

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

int opcodefind(char *instruction){
    //printf("instruct = %s\n",instruction);
    OPTAB *optab;
    optab = getoptab();
    int i;
    for(i=0;i<5;i++){
        if(!strcmp(optab[i].Mnemonic,instruction))
            return 1;
    }
    return 0;
}

int search(char *Label){
    int i;
    for(i=0;i<symcounter;i++){
        if(!strcmp(symtab[i].LABEL,Label))
            return 1;
    }
    return 0;
}

int _getfile(){
    int a;
    printf("Choose from following input file:\n");
    printf("case 1: Celcius-Farenheit.txt\n");
    printf("case 2: Celcius-Farenheit_error.txt\n");
    printf("case 3: Simple-Interest.txt\n");
    printf("case 4: Simple-Interest_error.txt\n");
    scanf("%d",&a);
    while(a<1||a>4){
        printf("invalid option\n try again\n");
        scanf("%d",&a);
    }
    return a;
}

int main()
{
    int a = _getfile();
    FILE *source,*intermediate,*symtable;
    switch(a){
    case 1:
        source = fopen("Celcius-Farenheit.txt","r");
        intermediate = fopen("CF-intermediate.txt","w");
        symtable = fopen("CF-symtable.txt","w");
        break;
    case 2:
        source = fopen("Celcius-Farenheit_error.txt","r");
        intermediate = fopen("CF_error-intermediate.txt","w");
        symtable = fopen("CF_error-symtable.txt","w");
        break;
    case 3:
        source = fopen("Simple-Interest.txt","r");
        intermediate = fopen("SI-intermediate.txt","w");
        symtable = fopen("SI-symtable.txt","w");
        break;
    case 4:
        source = fopen("Simple-Interest_error.txt","r");
        intermediate = fopen("SI_error-intermediate.txt","w");
        symtable = fopen("SI_error-symtable.txt","w");
        break;
    }

    // test for files not existing.
    if (source == NULL)
    {
        printf("Error! Could not open file\n");
        return 0;
    }

    char *Label,*instruction,*operand,ProgramName[10];
    char *end,line[250];
    int LOCCTR,STARTLOC;

    //This loop read input line from source code and loop end when it encounter 'START' keyword
    do
    {
        //fscanf(source,"%s %s %s",Label,instruction,operand);
        fgets(line, 250, source);
        Label=strtok(line," \t\n");
        instruction=strtok(NULL," \t\n");
        operand=strtok(NULL," \t\n");
        if(strcmp(instruction,"START")==0){
            fprintf(intermediate,"%x\t%s\t%s\t%s\n",strtol(operand,&end,16),Label,instruction,operand);
        }
    }while(strcmp(instruction,"START")!=0);

    //Iniatisation of Location counter
    LOCCTR = strtol(operand,&end,16);
    STARTLOC = LOCCTR;
    strcpy(ProgramName,Label);


    //This while loop read the source code line by line, ends when END keyword in instruction column appear for first time
    //In each loop iteration it will first Label is present or not
    //then it find instruction(opcode) and operand
    //according to opcode found it increase location counter(LOCCTR) by 3 byte(memory add 2 byte + opcode 1 byte)
    //Then at last it print symtable in symtable.txt file and location instruction and error(if any) in intermediate.txt
    while(strcmp(instruction,"END")!=0)
    {
        char *L=NULL,*I=NULL,*O=NULL,*Error= NULL;
        Error =NULL;
        Label = NULL;instruction=NULL;operand= NULL;
        fgets(line, 250, source);
        L=strtok(line," \t\n");
        I=strtok(NULL," \t\n");
        O=strtok(NULL," \t\n");
        //printf("ksdjkl");
        if(O==NULL){

            instruction=L;
            operand=I;
        }
        else{
            Label=L;
            instruction=I;
            operand=O;
            //Check if Label already exist from symtable
            if(search(Label)){
                Error = "Duplicate Label";
            }
            else{
                symcounter++;
                //printf("counter = %d %ld %ld %ld\n",symcounter,sizeof(symtab),sizeof(SYMTAB),sizeof(more_symtab));
                more_symtab = (SYMTAB*) realloc (symtab, symcounter * sizeof(SYMTAB));
                if (more_symtab!=NULL) {
                    symtab=more_symtab;
                //    printf("counter = %d %ld %ld %ld\n",symcounter,sizeof(symtab),sizeof(SYMTAB),sizeof(more_symtab));
                    strcpy(symtab[symcounter-1].LABEL,Label);
                    symtab[symcounter-1].LOCCTR=LOCCTR;
                    fprintf(symtable,"%s\t%x\n",symtab[symcounter-1].LABEL,symtab[symcounter-1].LOCCTR);
                }
                else {
                    puts ("Error (re)allocating memory");
                    exit (1);
                }
                /*
                strcpy(symtab[symcounter].LABEL,Label);
                symtab[symcounter].LOCCTR=LOCCTR;
                //printf("jjds %s %d",symtab[symcounter].LABEL,symtab[symcounter].LOCCTR);
                symcounter++;
                */
            }
        }

        if(opcodefind(instruction)){
            LOCCTR+=3;
        }
        else if(!strcmp(instruction,"WORD")){
            LOCCTR+=3;
        }
        else if(!strcmp(instruction,"RESW")){
            LOCCTR+=atoi(operand)*2;
        }
        else if(!strcmp(instruction,"BYTE")){
            LOCCTR+=1;
        }
        else if(!strcmp(instruction,"END")){
            LOCCTR+=3;
        }
        else{
            Error = "Invalid opcode";
            LOCCTR+=3;
        }
        if(Label==NULL)
            Label=" ";
        if(Error==NULL)
            Error=" ";
        fprintf(intermediate,"%x\t%s\t%s\t%s\t%s\n",LOCCTR-3,Label,instruction,operand,Error);
    }
    free(symtab);
    return 0;
}
