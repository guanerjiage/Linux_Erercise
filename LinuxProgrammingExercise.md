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

### errors in system calls

1. _errno_ is a global variable that indicate the type of errors, it could be accessed by each program. The error code are defined in file <errno.h>.

2. In the file <errno.h>, it also defined different handlers for different types of errors, some needs to warn the users like invalid file name, while others may not need any instruction to the user like too much files have been opern.

3. perror(string) is a function to print out error massage in stderr for corresponding error codes, the string is the discribe of error. For example, the error massage "Cannot open file: No such file or directory", the first part is the message get passed to the function in string, and the second part is the suggestion given by error code.

## Directory and file properties
