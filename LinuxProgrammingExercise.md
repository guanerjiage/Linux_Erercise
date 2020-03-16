# Unix/Linux Programming Exercise

## System programming from a user perspective

### System Resources

- I/O
- process management
- memory
- device
- timer
- interprocess communication
- networking

### Getting file contents

    cat filename //showing all the content
    more filename //showing only one screen of content
    mv filenamea filenameb //change name a to b
    mv filenamea directoryd //move a to directory d
    sl -l filename //list authority
    ps //list all working process

### exercise on command _more_

It will cause problem when we redirected the source into standered IO, as the source of content and the source of input are the same. So we need to force the user input from keyboard using the special file describer "/dev/tty".

There are also other terminal configurations allowed the input sent directly without waiting for Enter or not showing on the screen.

- how to not showing the more between pages?
- how to calculate percentage of file?
- how to automate the enter after space and 'q'?

## User, File operation and Unix helper

Adding new command: put the executable file under /bin, /usr/bin, /usr/local/bin

    man -k open //search for open in the man
    man -k | grep read for second key words read 

### exercise on command _who_, _cp_

Using system call open, write and close.

Command _cp_ using the system call creat to generate a new file and then use system call write to copy the data.

If we want to change a certain entry of one file, we need to move the pointer of current location. The pointer are linked to the file describer, which means if two process open the same file, they will have two different file describers and two different pointers. We use the system call lseek to achieve this function.

An important technology to speed up the system is buffering, as the switch between kernel mode and user mode is costly, and reading writing on disk is extrem time consuming. When writing data on to the disk, instead of write it directly back, the data has been copied from process buffer to kernel buffer or verse vice, the the disk decide on his own when to write back. Most c functions like fopen, getc, fclose and fgets contains kernel buffer. And fflush (for FILE*), std::flush (for IOStream) will force your program to send to the OS.

### errors in system calls

1. _errno_ is a global variable that indicate the type of errors, it could be accessed by each program. The error code are defined in file <errno.h>.

2. In the file <errno.h>, it also defined different handlers for different types of errors, some needs to warn the users like invalid file name, while others may not need any instruction to the user like too much files have been opern.

3. perror(string) is a function to print out error massage in stderr for corresponding error codes, the string is the discribe of error. For example, the error massage "Cannot open file: No such file or directory", the first part is the message get passed to the function in string, and the second part is the suggestion given by error code.

## Directory and file properties

### exercise on command _ls_

what does ls command do?

- show content of directory( read directory file )
- show file name and properties when given file names
- show directory contents when given directory names
- need to distinguish file and directory
  
The logic of writing _ls_ is similar to command _who_, we only need to read from certain file. The special property of directory is that it never empty, at least contains . and ..
We could use open, close and read to read directory, as it is also file.More convenicely we have function opendir, readdir, telldir, seekdir. However, you cannot write directory easily.

When writing ls -l we need more information about file states which include mode(rwx), links, owner, group, size, last modified time, file name. And all these information comes form kernel. The process could define a structure state and call the system call let the kernal pit state information about file into that structure.

Another challenge is to map the user number to real user name, we need to check /etc/password to get the id, name pair. Another detail that worth to mention is that not all users are included into /etc/password file. Imaging a network system which consist of large number of machines, all machines need to update each time one user change and it's lot of work to keep them all synchorized. Therefor, a machine called NIS contains all information and all identification are done through NIS, all local /etc/password files only keep a copy of useful subset for offline operation.

### going deeper into file modes

The file mode has in total 16 bits

tttt  u  g  s  ur  uw  ux  gr  gw  gx  or  ow  ox

The first 4 bits indicate the type of file, for now only 7 of them are used.

The next three bits is special properties bits, u for set_user_id, when it is setted, it tells the kernel, when running the program, assuming that is the owner of file is running the program. For example, each user should be allowed to reset their own passwords but not others, all the password are saved in file /etc/passwd, clearly, users don't own the right to write this file and only root does. But the program usr/bin/passwd has the property set_user_id, so when user using the command passwd, it will assume it's the owner of usr/bin/passwd is running the program instead of user, which is root. As the result, /etc/passwd are allowed to be changed. This properties gives the program to be called by all user but run as root to change some kernal information. g for set_group_id is the same.

s bit is called sticky bit, which used to stick some file into swap space even through it is not used. This technic allowed some program to load to memory faster. But nowadays, most system use the virtual memory, which allowed files to swap in a smaller unit(page), and this sticky bit are no longer that important.

The following 9 bit show the properties for user, group and others whether they could read, write or execute.

### how to change file properties

- file type: it is setted when the file is created and could never be changed.
- file athority bits: setted when file is created, could be changed by command _chmod_ both in 8-bit number way or symbol way
- link number: it is automatically changed when new process open of close the file
- owner and group: setted when created, notice that the owner and group are mostly setted as the user who run the create function, if set_user_id bit is setted, it will set its owner as the owner of program. 