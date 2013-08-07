#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct OPTAB1
{
    char *OPCODE;
}OPTAB;

void get_opcode(OPTAB optab[])
{
    optab[0].OPCODE = "00";
    optab[1].OPCODE = "20";
    optab[2].OPCODE = "24";
    optab[3].OPCODE = "0C";
    optab[4].OPCODE = "18";
}



void write_Header(char line1[],char line2[],char start[],FILE *output)
{
  int i=0,_beg,_end;
  char end[5];
  char * pch,*ret;
  fprintf (output,"H");
  pch = strtok (line1,"	\n\t");
  while (pch != NULL)
  { if(i==0)
    {
        strcpy(start,pch);
    }
    if(i==1)
    {
       fprintf (output,"%06s",pch);
    }
    if(i==3)
    {
       fprintf (output,"%06s",pch);
    }
    pch = strtok (NULL, " \n\t");
    ++i;
  }

  pch = strtok (line2,"	\n\t");
  strcpy(end,pch);
  char *endtol;
  _beg=strtol(start,&endtol,16);
  _end=strtol(end,&endtol,16);
  //printf("%s %s %d %d\n",start,end,_beg,_end);
  _end=_end-_beg+1;
  fprintf (output,"%06x",_end);
   //fputs()
   return ;
}

int instruction_len(FILE *intermediate)
{
    int i=0,flag=0;;
    char *pch,line[100];
    char sch[4]={'R','E','S','W'};
    while(fgets(line,100,intermediate)!=NULL)
    {
        pch = strtok(line," \n\t");
        while ( pch!=NULL)
        {

            pch = strtok(NULL," \n\t");
            if(pch==NULL)
                break;
            if(*(pch)==*(sch))
            {
               flag=1;
               break;
            }

        }
        if(flag==1)
        {
            flag=0;
            continue;
        }
        else
            i++;
    }

    return i-2;
}

void write_opcode(FILE *output,FILE *symtab,OPTAB optab[],int i,char *pch)
{
     char *sch,line1[100];
     int flag;
           while ( fgets ( line1,40, symtab ) != NULL )
           {
                flag=0;
                sch = strtok(line1," \n\t");
                while ( sch!=NULL)
                {
                     if(*(pch)==*(sch))
                         {   flag=1;
                             break;
                         }
                     sch = strtok (NULL," \n\t");

                }

            if(flag==1)
            {
                if(*(pch)==*(sch))
                {
                    sch = strtok (NULL," \n\t");
                    fprintf (output,"%s%s",optab[i].OPCODE,sch);
                    sch = strtok (NULL," \n\t");

                    break;
                }
           }
           }

           if(flag==0)
               {printf("ERROR, NO Variable in symbol table");
                exit(0);
               }

}
void write_Text(char line[],FILE *output,FILE *symtab,OPTAB optab[])
{
     char * pch,*sch,*err,line1[100];
     int flag=0;

     pch = strtok (line," \n\t");
     fseek ( symtab , 0 , SEEK_SET );
     while (pch != NULL)
     {
         if(strcmp(pch,"LDA")==0 ||strcmp(pch,"MUL")==0|| strcmp(pch,"DIV")==0 || strcmp(pch,"ADD")==0 || strcmp(pch,"STA")==0 || strcmp(pch,"RESW")==0 || strcmp(pch,"WORD")==0)
         {
             flag=1;
             break;
          }
         if(strcmp(pch,"Invalid")==0 || strcmp(pch,"Duplicate")==0){
             //strcpy(err,pch);
             pch = strtok (NULL," \n\t");
             if(strcmp(pch,"opcode")==0) {
                printf("Invalid opcode\n");
                exit(0);
             }
             else if(strcmp(pch,"Label")==0 ){
                 printf("Duplicate Label\n");
                exit(0);
             }
         }
         pch = strtok (NULL," \n\t");
     }

     if(flag==0){
         return;
     }

     if(strcmp(pch,"LDA")==0)
     {
      pch = strtok (NULL," \n\t");
      err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }
         write_opcode(output,symtab,optab,0,pch);

     }
     else if(strcmp(pch,"MUL")==0)
     { pch = strtok (NULL,"	\n\t");
      err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }

         write_opcode(output,symtab,optab,1,pch);
     }
     else if(strcmp(pch,"DIV")==0)
     { pch = strtok (NULL,"	\n\t");
      err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }
         write_opcode(output,symtab,optab,2,pch);
     }
     else if(strcmp(pch,"ADD")==0)
     { pch = strtok (NULL,"	\n\t");
      err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }
         write_opcode(output,symtab,optab,4,pch);
     }
     else if(strcmp(pch,"STA")==0)
     { pch = strtok (NULL,"	\n\t");
      err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }
         write_opcode(output,symtab,optab,3,pch);
     }
     else if(strcmp(pch,"RESW")==0)
     {
          err = strtok (NULL," \n\t");
          err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }
        fprintf (output,"%s","******");
     }
     else if(strcmp(pch,"WORD")==0)
     {
         pch = strtok (NULL," \n\t");
         err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }
         fprintf (output,"%06s",pch);
     }
      else if(strcmp(pch,"START")==0)
     {
         err = strtok (NULL," \n\t");
         err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }
     }
     else if(strcmp(pch,"END")==0)
     {
         err = strtok (NULL," \n\t");
         err = strtok (NULL," \n\t");
      if(err!=NULL)
         { printf("ERROR %s\n",err);
          fopen("CF-object.txt","w");
          exit(0);
      }
      }
}

void write_End(FILE *output,char *start)
{
    fputs(start,output);
}

int getfile(){
    int a;
    printf("Choose from following intermediate file:\n");
    printf("case 1: Celcius-Farenheit.txt\n");
    printf("case 2: Celcius-Farenheit_error.txt\n");
    printf("case 3: Simple-Interest.txt\n");
    printf("case 4: Simple-Interest_error.txt\n");
    scanf("%d",&a);
    do{
        printf("invalid option\n try again\n");
        scanf("%d",&a);
    }while(a<1||a>4);
    return a;
}

int amain()
{
    int a = getfile();
    FILE *output,*intermediate,*symtab;
    switch(a){
    case 1:
        output = fopen("CF-object.txt","w");
        intermediate = fopen("CF-intermediate.txt","r");
        symtab = fopen("CF-symtable.txt","r");
        break;
    case 2:
        output = fopen("CF_error-object.txt","w");
        intermediate = fopen("CF_error-intermediate.txt","r");
        symtab = fopen("CF_error-symtable.txt","r");
        break;
    case 3:
        output = fopen("SI-object.txt","w");
        intermediate = fopen("SI-intermediate.txt","r");
        symtab = fopen("SI-symtable.txt","r");
        break;
    case 4:
        output = fopen("SI_error-object.txt","w");
        intermediate = fopen("SI_error-intermediate.txt","r");
        symtab = fopen("SI_error-symtable.txt","r");
        break;
    }

    OPTAB optab[5];
    get_opcode(optab);

    char start[5],line1 [100],line2[100],line3[100];
    int opcode_len;

    if (intermediate == NULL || output== NULL)
        printf("Error opening file");

    else
    { while ( fgets ( line1,100, intermediate ) != NULL )

        {
          if(strstr(line1,"START")!=NULL)
            {
             strcpy(line2,line1);
            }

         if(strstr(line1,"END")!=NULL)
           {
             strcpy(line3,line1);
           }

        }

        write_Header(line2,line3,start,output);
        fseek ( intermediate ,0, SEEK_SET );

        fprintf (output,"\n%c",'T');
        opcode_len=instruction_len(intermediate);
        opcode_len*=3;
        fprintf (output,"%02x",opcode_len);
        fseek ( intermediate ,0, SEEK_SET );

        while ( fgets ( line1,100, intermediate ) != NULL )
        {
            write_Text(line1,output,symtab,optab);
        }

         fprintf (output,"\nE00");
         write_End(output,start);
    }
    return 0;
}
