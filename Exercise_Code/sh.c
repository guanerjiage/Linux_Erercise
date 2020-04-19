#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define MAXARGS 20
#define ARGLEN 100


execute(char *arglist[])
{
    int pid, exitstatus;
    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork failed");
        break;
    case 0:
        printf(arglist[0]);
        execvp(arglist[0], arglist);
        perror("execvp failed");
        exit(1);
    default:
        while (wait(&exitstatus) != pid)
        {
            ;  
        }
        printf("\nchild exited with status %d, %d\n", exitstatus>>8,exitstatus&0377);
    }
}

void main()
{
    char *arglist[MAXARGS+1];
    int numargs;
    char argbuf[ARGLEN];
    char *makestring();

    numargs = 0;
    while (numargs < MAXARGS)
    {
        printf("Arg[%d]?", numargs);
        if(fgets(argbuf, ARGLEN, stdin) && *argbuf !='\n')
            arglist[numargs++] = makestring(argbuf);
        else
        {
            if(numargs>0)
            {
                arglist[numargs] = NULL;
                signal(SIGINT, SIG_IGN);
                execute(arglist);
                signal(SIGINT, SIG_DFL);
                numargs = 0;
            }
        }
    }
}


char * makestring(char *buf)
{
    char *cp, *malloc();
    buf[strlen(buf)-1] = '\0';
    cp = malloc(strlen(buf) + 1);
    if(cp == NULL)
    {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}