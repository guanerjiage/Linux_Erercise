#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT ">"

int main()
[
    char *cmdline, *prompt, **arglist;
    int result;
    voud setup();

    prompt = DFL_PROMPT;
    setup();
    while((cmdline = next_cmd(prompt,stdin) != NULL)
    {
        if((arglist = splitline(cmdline) != NULL))
        {
            result = execute(arglist);
            freelist(arglist);
        }
        free(cmdline);
    
    }
    return 0;
]

void setup()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

