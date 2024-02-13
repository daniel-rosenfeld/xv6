#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char* path)
{
    char* p;

    for(p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    return p;
}

void find(char* path, char* to_find)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        exit(0);
    }

    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(0);
    }

    switch(st.type)
    {
        case T_DEVICE:
        case T_FILE:
            if (strcmp(fmtname(path), to_find) == 0)
            {
                printf("%s\n", path);
            }
            break;

        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
            {
                printf("find: path too long\n");
                break;
            }


            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            while (read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if ((de.inum == 0) || (strcmp(de.name, "..") == 0) || (strcmp(de.name, ".") == 0))
                    continue;
                
                memmove(p, de.name, strlen(de.name));
                p[strlen(de.name)] = 0;
                find(buf, to_find);
            }
            break;
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("not enough arguments given\n");
        exit(0);
    }
    
    find(argv[1], argv[2]);
    exit(0);
}
