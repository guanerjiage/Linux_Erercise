#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

void main (int ac, char * av[])
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];
    if (ac != 3)
    {
        fprintf(stderr, "usage:%s sourse destination\n", *av);
        exit(1);
    }    
    if((in_fd = open(av[1], O_RDONLY)) == -1)
        oops("Cannot open ", av[1]);
    if((out_fd = open(av[2], O_WRONLY|O_CREAT)) == -1)
        oops("Cannout create ", av[2]);
    while((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
    {
        if(write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to ", av[2] );
    }
    if(n_chars == -1)
        oops("Read error from  ", av[1]);
    if(close(in_fd) == -1 || close(out_fd) == -1)
        oops("Close error", "");
}

void oops(char *s1, char * s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}