#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main()
{
    int SocketD;
    SocketD = socket{(AF_INET,SOCK_STREAM,0)}

    printf("My Socket Descriptor: %d")
    return 0;
}
