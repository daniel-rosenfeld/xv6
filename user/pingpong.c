#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int p[2];
    int process_id;
    char buffer;
    pipe(p);
    
    if (fork() == 0)
    {
        process_id = getpid();
        read(p[0], &buffer, 1);
        close(p[0]);
        printf("%d: received ping\n", process_id);
        write(p[1], &buffer, 1);
        close(p[1]);
    }
    else 
    {
        buffer = '1';
        process_id = getpid();
        write(p[1], &buffer, 1);
        close(p[1]);
        read(p[0], &buffer, 1);
        close(p[1]);
        printf("%d: received pong\n", process_id);
    }

    exit(0);
}
