#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stdbool.h>
#define UPTO 35

int main(int argc, char* argv[])
{
    int p[2];
    int prime = 0;
    int getting_numbers, sending_numbers;
    int num[1];
    pipe(p);
    
    if (fork() == 0)
    {
        close(p[1]);
        while (true)
        {
            getting_numbers = dup(p[0]);
            close(p[0]);
            pipe(p);
            sending_numbers = dup(p[1]);
            close(p[1]);

            if (read(getting_numbers, num, 1) != 0)
            {
                prime = num[0];
                printf("prime %d\n", prime);
            }
            else
            {
                break;
            }

            if (fork() == 0)
            {
                close(getting_numbers);
                close(sending_numbers);
                continue;
            }
            
            while (read(getting_numbers, num, 1) != 0)
            {
                if (num[0] % prime != 0)
                {
                    write(sending_numbers, num, 1);
                }
            }

            close(getting_numbers);
            close(sending_numbers);
            wait(0);
            exit(0);
        }
        close(p[0]);
        exit(0);
    }
    else
    {
        close(p[0]);
        for (int i = 0; i < UPTO - 1; i++)
        {
            num[0] = i + 2;
            write(p[1], num, 1);
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
}