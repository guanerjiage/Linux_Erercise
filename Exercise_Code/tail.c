#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZE 100 

void tail(FILE *, int);
int main(int ac, char* av[])
{
    FILE* fp;
    //printf("ac = %d, av[1] = %s, av[2] = %s", ac, av[1], av[2]);
    if (ac != 3)
    {
        fprintf(stderr, "usage:%s sourse destination\n", *av);
        exit(1);
    }    
    else
    {
        if((fp = fopen(av[1], "r")) == NULL)
        {
            fprintf(stderr, "Error: %s ", "cannot open");
            perror(av[1]);
            exit(1);
        }
        else
        {
            int linenum = atoi(av[2]);
            tail(fp, linenum);
        }
            
    }
    return 0;
}

void tail(FILE* in, int n) 
{ 
    int count = 0;  //count '\n' characters 
  
    unsigned long long pos; 
    char str[2*SIZE]; 
  
    if (fseek(in, 0, SEEK_END)) 
        perror("fseek() failed"); 
    else
    {  
        pos = ftell(in); 
        // search for '\n' characters 
        while (pos) 
        { 
            // Move 'pos' away from end of file. 
            if (!fseek(in, --pos, SEEK_SET)) 
            { 
                if (fgetc(in) == '\n') 
  
                    // stop reading when n newlines 
                    // is found 
                    if (count++ == n) 
                        break; 
            } 
            else
                perror("fseek() failed"); 
        } 
        // print last n lines 
        //printf("Printing last %d lines -\n", n); 
        while (fgets(str, sizeof(str), in)) 
            printf("%s", str); 
    } 
    printf("\n\n"); 
} 