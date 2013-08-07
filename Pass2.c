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
    /*printf("this is line 1 \n");
    for(int j=0;line1[j]!='\n';j++)
    printf("%c",line1[j]);
    printf("now line 2 \n");
    for(int j=0;line2[j]!='\n';j++)
    printf("%c",line2[j]);*/
  int i=0,_beg,_end;
  char end[5];
  char * pch,*ret;
  fprintf (output,"%c " ,'H');
  pch = strtok (line1,"		");
  while (pch != NULL)
  { if(i==0)
    {
        strcpy(start,pch);
    }
    if(i==1)
    {
       fprintf (output,"%s",pch);
    }
    if(i==3)
    {
       fprintf (output,"  %s  ",pch);
    }
    pch = strtok (NULL, "	");
    ++i;
  }

  pch = strtok (line2,"		");
  pch = strtok (NULL,"		");
  strcpy(end,pch);
  _beg=atoi(start);
  _end=atoi(end);
  _end=_end-_beg+1;
  fprintf (output,"%x",_end);
    return ;
}

void write_opcode(FILE *output,FILE *symtab,OPTAB optab[],int i,char *pch)
{
     char *sch,line1[100];
           while ( fgets ( line1,40, symtab ) != NULL )
           {
                int flag=0;
                sch = strtok(line1,"		");
                while ( sch!=NULL)
                {
                     printf("this is pch and sch %s  %s ",sch,pch);
                     if(*(pch)==*(sch))
                         {printf("BREAKING!!!");
                     flag=1;
                             break;

                     }
                     sch = strtok (NULL,"		");

                }

                printf("this is flag %d\n",flag);

            if(flag==1)
            {
                if(*(pch)==*(sch))
                {   printf("yoyo sch %s ",sch);
                    printf("yoyo pch %s ",pch);
                    printf("yoyo optab %s ",optab[i].OPCODE);
                    sch = strtok (NULL," ");
                    printf("yoyo next sch %s ",sch);
                    fprintf (output," %s    %s ",optab[i].OPCODE,sch);
                    sch = strtok (NULL," ");

                    break;
                }
           }
           }

}
void write_Text(char line[],FILE *output,FILE *symtab,OPTAB optab[])
{
     char * pch,*sch,line1[100];
     int flag=0;

     pch = strtok (line,"		");
     fseek ( symtab , 0 , SEEK_SET );
     while (pch != NULL)
     {
         if(strcmp(pch,"LDA")==0 ||strcmp(pch,"MUL")==0|| strcmp(pch,"DIV")==0 || strcmp(pch,"ADD")==0 || strcmp(pch,"STA")==0 || strcmp(pch,"RESW")==0 || strcmp(pch,"WORD")==0)
          {flag=1;
          printf("\n %s",pch);
           break;
          }
         pch = strtok (NULL,"		");
     }

     if(flag==0)
         return;

     if(strcmp(pch,"LDA")==0)
     {printf("haha \n %s",pch);
      pch = strtok (NULL,"	");
      printf(" %s ",pch);
         write_opcode(output,symtab,optab,0,pch);

     }
     else if(strcmp(pch,"MUL")==0)
     { pch = strtok (NULL,"		");
         write_opcode(output,symtab,optab,1,pch);
     }
     else if(strcmp(pch,"DIV")==0)
     { pch = strtok (NULL,"		");
         write_opcode(output,symtab,optab,2,pch);
     }
     else if(strcmp(pch,"ADD")==0)
     { pch = strtok (NULL,"		");
         write_opcode(output,symtab,optab,4,pch);
     }
     else if(strcmp(pch,"STA")==0)
     { pch = strtok (NULL,"		");
         write_opcode(output,symtab,optab,3,pch);
     }
     else if(strcmp(pch,"RESW")==0)
     {
        fprintf (output," %s  ","******");
     }
     else if(strcmp(pch,"WORD")==0)
     {
         pch = strtok (NULL,"		");
         fprintf (output," %s  ",pch);
     }


}

void write_End(FILE *output,char *start)
{
    fputs(start,output);
}

int main()
{

    FILE *intermediate = fopen("CF-intermediate.txt","r");
    FILE *symtab = fopen("CF-symtable.txt","r");
    FILE *output = fopen("CF-object.txt","w");
    OPTAB optab[5];
    get_opcode(optab);

    char start[5],line1 [100],line2[100],line3[100];

    if (intermediate == NULL || output== NULL)
        printf("Error opening file");

    else
    { while ( fgets ( line1,100, intermediate ) != NULL )

        {
          if(strstr(line1,"START")!=NULL)
            {  /*printf("This is the source\n");
                for(int j=0;line1[j]!='\n';j++)
                printf("%c",line1[j]);*/
             strcpy(line2,line1);
            }

         if(strstr(line1,"END")!=NULL)
           { /*printf("This is the source\n");
                for(int j=0;line1[j]!='\n';j++)
                printf("%c",line1[j]);*/
             strcpy(line3,line1);
           }

        }
        write_Header(line2,line3,start,output);
        fseek ( intermediate ,0, SEEK_SET );
        fprintf (output,"%c  ",'T');
        while ( fgets ( line1,100, intermediate ) != NULL )
        {
          write_Text(line1,output,symtab,optab);
        }
         fprintf (output,"%c  ",'E');
         write_End(output,start);
    }
 //   getch();
    return 0;
}
