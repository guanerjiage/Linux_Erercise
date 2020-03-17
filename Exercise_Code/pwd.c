#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

ino_t get_inode(char*);
void printpathto(ino_t);
void inum2name(ino_t, char *, int);

int main()
{
    printpathto(get_inode("."));
    putchar('\n');
    return 0;
}

void printpathto(ino_t this_inode)
{
    ino_t my_inode;
    char itsname[BUFSIZ];
    if(get_inode("..") != this_inode)
    {
        chdir("..");
        inum2name(this_inode, itsname, BUFSIZ);
        my_inode = get_inode(".");
        printpathto(my_inode);
        printf("/%s", itsname);
    }
}

void inum2name(ino_t inode2find, char * namebuf, int buflen)
{
    DIR *dir_prt;
    struct dirent *direntp;
    dir_prt = opendir(".");
    if (dir_prt == NULL)
    {
        perror(".");
        exit(1);
    }
    while((direntp = readdir(dir_prt)) != NULL)
    {
        if(direntp->d_ino == inode2find)
        {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_prt);
            return;
        }
    }
}

ino_t get_inode(char * fname)
{
    struct stat info;
    if(stat(fname, &info) == -1)
    {
        fprintf(stderr, "Cannot stat");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}