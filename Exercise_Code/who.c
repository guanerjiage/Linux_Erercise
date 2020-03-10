#include <stdio.h>
#include <utmp.h> // getting user information
#include <fcntl.h> // for system call open close and read
#include <unistd.h> // relate to pclose~close fread~read??
#include <time.h>
void showtime(long);
void showinfo(struct utmp *);

int main()
{
    struct utmp utbuf;
    int utmpfd;
    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == 1)
    {
        perror(UTMP_FILE);
        exit(1);
    }
    while(read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
    {
        showinfo(&utbuf);
    }
    close(utmpfd);
    
    return 0;
}

void showinfo(struct utmp * utbufp)
{
    if(utbufp->ut_type != USER_PROCESS)
        return;
    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    showtime(utbufp->ut_time);
    printf("\n");
}

void showtime(long timeval)
{
    char *cp;
    cp = ctime(&timeval);
    printf("%16.16s", cp);
}