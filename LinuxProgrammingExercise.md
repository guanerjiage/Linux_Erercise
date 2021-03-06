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
- file athority bits: setted when file is created, could be changed by command _chmod_ both in 8-bit number way or symbol way.
- link number: it is automatically changed when new process open of close the file.
- owner and group: setted when created, notice that the owner and group are mostly setted as the user who run the create function, if set_user_id bit is setted, it will set its owner as the owner of program.
- size: it will automatically calculate the size of the file, syscall creat could set the size of file to 0, but no other function to reduce the size of file directly.
- time: three time properties of file is modification time, last access time and last property changed time. Normally, system will change these time automatically. The command _touch_ could reset the modification time and last access time.

## File system

### basic knowledge of file system

From user perspective, the file system orgnized as a directory tree. However, physically, the whole file system stored linearly on the disk and devided into three parts, superblock, inode table, and data section.

- superblock saves the information about file system, which contant size of each section, information of unused blocks.
- inode list saves inodes, which is a structure saving file properities mentioned in the last section of files, each inode have the same size.
- data section saves real data about the file, all the blocks have same size while large file have several blocks to save the data.

### The procedure of creating a new file have four main steps

- save properties: the kernel will find an empty inode and save corresponding information into the inode
- save data: the kernel calculate the require number of block and find them in the free list of blocks, copy the data into free blocks.
- save block distribution: save the index of used block into inode structure, so later we could find the approperate block.
- add to directory: kernal add the file name and inode index to the directory file, which connect the file name with his location and inode.

Using the command "ls -lia dirname" to see the content of directory.
Specially, more than one file name can point to same inode index. All the file read commands are achieved by passing filename to funtion open, and open will search file name in the directory, access inode to get file properties and locating sections containing file data. One additional problem is that the size of inode is fixed, if the file is large, there will not be enough space for section index. The system fix this by introducing secondary index, for the first 10 out of 14 section indexs are saving in the inode and the last 4 are saving in the data section that 11st section index points to.

    ls -lia dirname //see the content of directory
    ls -i //including inode number in ls command
    mkdir path name//creat a new directory
    rmdir path //remove a directory, it has to be empty with only two entries
    rm path //remove an entry from directory, it use syscall unlink to reduce link count so it cannot use to directory
    ln org new //create a file link using syscall link, cannot use on directory
    ln -s org new //creat symbolic link

Unix also support multi-file system, it links one system as a subtree to another. the corresponding problem is that inode index is no longer identical, so Unix forbidden syscall rename and link to create or tansform links between file systems.

### symbolic links

There are two kinds of link in Unix system, the hard link which links the filename to the real file(inode), but it could only link within one file system. Another kind of link could solve this problem is symbolic link. Instead of link filename to inode, it links to the filename, so there is no crash on same inode index. It is a special file type with type representer l. The link number, modification time and size is all different from the original one. But doing reading and writing, it is all the same, and if the original copy is deleted or has a new name, it will points to NULL. Also a symbolic link could point to directory.

## Connection control

Other than file, an operating system needs to interact with other devices like mouse, printer, screen. In this chapter, the content will be about similarity between directory and files, also the control of all these connection.

### devices like a file

Mostly, devices are in the direcory /root/dev and tty* for terminals. Not only do they have file names, they also support file operation functions like open, read, write, lseek, close and stat.For terminal, it does not support lseek as it act like a flow of date.
Also, they have properties the usually file type field shows the unit of data transmition. What's special is the inode index field and the size filed, the inode index field contain the pointer points to the kernel program called driver, and the size data act as the parameter pass to the driver program.

Consider how function read works, it first find the inode index throught the directory, if it is a file, it goes to the disk section and doing read, and if it is a device file, it go to the disk which containt driver program and run the read function in the driver program.

### devices not like a file

Here we use disk and terminal as two examples to show the difference of different file discriber.

Disk:

- disk have the property of buffering set O_SYNC will force the kernel to shut down the buffer mechanisem.
- auto append mode is a solver of multiply writer write to the same file. It set O_APPEND, which will bound lseek and write to an atomoc operation, so the write of more than one writer will not cause race condition.

Terminal:

- Terminal will buffer the input, deal with it when receiving return or space. So there must be an intermedia layer between terminal and process.
- Using the command stty, user could confige terminal driver program, like changing delete symbol, cancling print on. The driver program control the input, output, control and buffer.

## Terminal control and signals

As the difference between device manufacturing driver program and user program, we group progrms into three groups:

- software tools: Programs that does not distinguish between device or file. Most of them will open stdin stdout and stderr at the beginning, and could redirecte to other files.
- special device programs: They are programs that specially communicate to device, cannot redirect mostly.
- user programs: The most genernal type of program most of them have the property of 
  1. response to user event quickly( mouse click and keypoard)
  2. limited set of input
  3. unexpeced time of input leads to block
  4. special defination of Ctrl-C

### Terminal mode

Terminal has two mode normally in canonical processing mode which including buffering and text edition. The experiment shows that backspace will delete the word and will not send to the program and the program received all the character when the enter is met. An the Ctrl-C won't stop the program.

We can change the mode by using command "stty -icanon; ./program". so every input character will directly send to the program without buffered or changed.

### The example of play_again to combine everything

- 1.0: simple responce to user's y or n
  problem: get input only after type enter, and the program will get a whole line to operate.
- 2.0: close the canonical mode to get responce right away, only get one character once a time.
  problem: too much information every time user do a wrong type.
- 3.0: close echo mode and only responce to valid inputs
  problem: need a timeout for input
- 4.0: non-block property, check 3 every second and for 3 times
  problem: when enter Ctrl-C, the program will exit without recovery the mode we set.
- 5.0: after learning the signal

### Signal

Where does signal comes from?

- user, like a Ctrl-C
- kernel, occuring error like divided by zero, overfloating
- process, communication between process like synchronous signals(/usr/include/signal.h)

How does process deal with signals?

- using default handler function
- process could set ignore bit to certain signals using signal(SIGINT, SIG_IGN)
- writing his own callback function signal(signal_number, callback_function)

Then we go back to the last version of play_again function, we capture the signal generated by user's Ctrl-C and set the callback function to recovery every setting before quit.

## Event driven programming simple game progamming

### How to update the screen

- using library curses view the screen as a big array
- function refreash to update the whole screen

### How to set up a timer

- using sleep(time) function to hang up the process
- the mechanisem behind sleep is kernal function alarm and pause. Each process has a alarm clock, it runs alarm(time) to set the alarm which will generate SIGALRM after time seconds, and runs pause() to hang himself up. One important thing is that any signal will wake the process.
  
Now for more time support all process have three clocks, each clock has a initial time and a interval time:

- ITIMER_REAL: counting real time, no matter the process is using cpu or not.
- ITIMER_VIRTUAL: counting virtual time, which the process is actually running
- ITIMER_PROF: runs in user mode or traped into kernal ?????

### How to deal with more than one signal

First of all, the signal handler is like a mouse traper, that need to reset every time it trap a signal. Although the reset can be pretty fast, it still needs time to execute, which may lead the signal to flee.

Try to run signaldemo with different combination of Ctrl-C and Ctrl-\ to see how it works.

- Does the handler stable? Will it keep its handler when interrupt in the handler?
- What happens when sigX interrupt sigY? Jump to handler y in the middel of handler x.
- What happens when sigX interrupt sigX? recurrence, ignore the second or block the second untile the first one is finnished.
- Interrupt when executing system call, e.g. running read to wait for input while a signal comes. How could the read function react?
  
Also there are other weakness of signal. For example, the reason of generated signal or cannot block other signal safely. So we got a better version of signal handler called sigaction. To tell the kernal which mode of signal handler you are used, set the SA_SIGINFO bit in sa_flags.

### Prevent data corruption

When more than one process, threads or signals touch one file, it may cause the file to be writed wrong, we need some strategies to protect critical sectons. One way to deal with it is to use sigprocmask or sigsetops to block some signals.

### Finally the bounce game

a bounce ball on the screen

- an interval timer generate a signal stream to move the ball.
- block to wait for user input and react.

## Process and program: writing sh

In the context of Unix, program is a sequence of machine instructions and data, a process is the configuration and memory space of running program.

### Chencking process state

command _ps_ (process state) shows all the running process in user space. _ps -l_ will list more information about process, including colunm S, UID, PID, PPID, PRi, NI, SZ, WCHAN:

- S: state, S means sleep and R means runnig.
- UID: user id.
- PID: process id.
- PPID: parent pid, the process who create this process.
- PRI: priority, indicate the kernal to schedule their running order.
- NI: niceness, increasing niceness will allow other process cut in.
- SZ: indicate the size of a process.
- WCHAN: shows the reason of sleep, read_c and do_sel is the address of kernal.

Besides user process, the operation system also has kernal process to manage system log, schedule tasks and protect injection.

### What dose shell do

Basic function of shell: running program(progrma launcher), manage input and output(redirect, <,>,|), programable.

The workflow of shell is, first get input from user, build a new process to run the program, load the program from disk and the program will run till it ends.

The fork() function will create a same process as the parent, the only difference is that the return value of child function is 0 and the return value of parent is chile's pid number. And then in the child process, we use execvp() function to execute user declared program. The last thing is that the parent should wait for the child to end, we use code pid = wait(&status) to wait.

### Summary of fork/exec and exit

exec/exit act like a function call, it pass some parameters to another function and get the result when it return. As the parameter passed is always strings, it accidentally support cross-plantform interaction.

With the passing of parameter get deeper, it's hard to share global variables, so Unix using environment to work on it.

exit function will end the life of a process, it will close all the open files, release the memory. A process which has finished the execution but still has entry in the process table to report to its parent process is known as a zombie process. A child process always first becomes a zombie before being removed from the process table. The parent process reads the exit status of the child process which reaps off the child process entry from the process table.

A process whose parent process no more exists i.e. either finished or terminated without waiting for its child process to terminate is called an orphan process.
In the following code, parent finishes execution and exits while the child process is still executing and is called an orphan process now. However, the orphan process is soon adopted by init process, once its parent process dies.

## Shell

First, shell is a program language. Second, what does shell include? it includes commants, variables, user input, control logic and environment. For example, HOME in shell is an environment variable that indicate your directory path.

### The road to shell processing program

1. The first version is to complete the most simple functions of a shell- be able to receive commands and run the commands.
It first received command line to get the command, call malloc to get arbitruary commands and reture until meet NULL. Then it split the command into words and parameters, finally call the execute, in which it will use fork to start a new thread, the child thread use execvp to load the user indicate program, and the parent thread wait until the child returen and then continue his own workflow.
