#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>



char buf1[] = "qwertyuiop";
char buf2[] = "QWERTYUIOP";

int main(void)
{
    int fd;

    if ((fd = open("file.hole", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU|S_IRWXG)) == -1)
    {
        perror("Open file error.\n");
        exit(EXIT_FAILURE);
    }
    
    if (write(fd, buf1, 10) != 10)
    {
        perror("buf1 write error.\n");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd, 16384, SEEK_SET) == -1)
    {       
        perror("seek error.\n");
        exit(EXIT_FAILURE);
    }

    if (write(fd, buf1, 10) != 10)
    {
        perror("buf1 write error.\n");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd, 156384, SEEK_SET) == -1)
    {       
        perror("seek error.\n");
        exit(EXIT_FAILURE);
    }

    if (write(fd, buf2, 10) != 10)
    {
        perror("buf1 write error.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}