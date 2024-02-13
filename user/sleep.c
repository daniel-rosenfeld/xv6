#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf("no argument given");
    }
    int time_to_sleep = atoi(argv[1]);
    sleep(time_to_sleep);
    exit(0);
}
