#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void execute(char input_buff[], int argc, char* argv[])
{
    char* p = input_buff;
    int word_length = 0;
    int argv_length = argc;

    while (*(p + word_length) != '\0')
    {
        if (*(p + word_length) == '\n')
        {
            if (fork() == 0)
            {
                *(p + word_length) = '\0';
                argv[argv_length] = p;
                argv_length++;
                argv[argv_length] = 0;
                exec(argv[0], argv);
                exit(0);
            }
            wait(0);
            argv_length = argc;
            p += ++word_length;
            word_length = 0;
        }
        if (*(p + word_length) == ' ')
        {
            *(p + word_length) = '\0';
            argv[argv_length] = p;
            argv_length++;
            p += ++word_length;
            word_length = 0;
        }
        word_length++;
    }
}
int main(int argc, char* argv[])
{
    char* argv2[MAXARG];
    char buffer[562];
    char buff;
    int check = 0;
    int i = 0;
    int input_length = 0;

    for (i = 1; i < argc; i++)
    {
        argv2[i - 1] = argv[i];
    }

    while ((check = read(0, &buff, sizeof(buff))) > 0)
    {
        buffer[input_length] = buff;
        input_length++;
    }
    
    if (check < 0)
    {
        printf("error occurred while reading input\n");
        exit(0);
    }
    
    buffer[input_length] = 0;
    
    execute(buffer, i - 1, argv2);

    exit(0);
}