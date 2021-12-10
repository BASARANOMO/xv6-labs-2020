#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXN 1024

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        fprintf(2, "usage: xargs command\n");
        exit(1);
    }
    char *_argv[MAXARG]; // arg array for child process to exec
    for (int i = 1; i < argc; i++)
        _argv[i - 1] = argv[i];
    char buf[MAXN];
    char c = 0;
    int stat = 1;
    // printf("%s %d \n", buf, sizeof(buf));
    while (stat) 
    {
        int buf_cnt = 0;
        int arg_begin = 0;
        int argv_cnt = argc - 1;
        while (1)
        {
            stat = read(0, &c, 1); // read from std input (pipe), char by char
            if (stat == 0) // read error (no data from std input)
                exit(0);
            if (c == ' ' || c == '\n')
            {
                buf[buf_cnt++] = 0; // current arg ends (char list ends with 0)
                _argv[argv_cnt++] = &buf[arg_begin];
                // printf("%s %s %d %d \n", buf, &buf[arg_begin], arg_begin, buf_cnt);
                arg_begin = buf_cnt;
                if (c == '\n')
                    break;
            }
            else // current arg continues
            {
                buf[buf_cnt++] = c;
            }
        }
        _argv[argv_cnt] = 0;
        if (fork() == 0)
        {
            exec(_argv[0], _argv);
        }
        else
        {
            wait(0);
        }
    }
    exit(0);
}