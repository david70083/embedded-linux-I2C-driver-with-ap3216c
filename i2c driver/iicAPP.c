#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "linux/ioctl.h"
#include <linux/input.h>

#define KEY0VALUE 0XF0
#define INVAKEY 0X00
//./chrdevbaseAPP filename 1,2

#define CLOSE   _IO(0xef, 1)
#define OPEN    _IO(0xef, 2)
#define SET     _IOW(0xef, 3, int)

static struct input_event inputevent;


int main(int argc, char *argv[])
{
    int err;
    int fd = 0;
    char *filename;
    filename = argv[1];
    short buf;
    short ps;
    int ret;


    #define on 1
    #define off 0

    fd = open(filename, O_RDWR);
    if (ret < 0)
    {
        printf("can*t open file\r\n");
        return -1;
    }
    while(1)
    {
        err = read(fd, &buf, sizeof(buf));
        ps = buf;
        printf("read ps %d\r\n", ps);
    }
    
    ret = close(fd);
    if (ret < 0)
    {
        printf("can*t close file\r\n");
        return -1;
    }
}
