#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Usage: simpleclient hostname port";
        return(-1);
    }
    int read_size;
    int SocketD = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *host;
    char client_message[2000];
    host = gethostbyname(argv[1]);
    sockaddr_in SockAddr;
    SockAddr.sin_port=htons(atoi(argv[2]));
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (!connect(SocketD, (sockaddr*)&SockAddr, sizeof(SockAddr))) {
    string input;
    char buffer[10000];
    recv(SocketD,client_message,10000,0);
    cout << client_message << endl;
    cin >> input;
    strcpy(buffer, input.c_str());
    send(SocketD, buffer, 20, 0);
      while (1) {
        memset(&client_message[0], 0, sizeof(client_message));
        cin >> input;
        strcpy(buffer, input.c_str());
        send(SocketD, buffer, 20, 0);
        usleep(500);
        recv(SocketD, client_message, 10000, 0);
        cout << client_message << endl;
      }
    } else cout << "Could not connect to server. Try again later." << endl;
    close(SocketD);
}
