#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int port = 61500;

int main()
{
    int SocketD;
    SocketD = socket(AF_INET, SOCK_STREAM,0);
    cout << "My Socket Descriptor:" << SocketD << endl;
    struct sockaddr_in self;
    self.sin_family = AF_INET;
    self.sin_port = htons(port);
    self.sin_addr.s_addr = INADDR_ANY;

    bind(SocketD, (struct sockaddr*)&self, sizeof(self));
    cout << "Server is listening on port: " << port << endl;

    listen(SocketD, 20);

    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    int clientfd = accept(SocketD, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);

    cout << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;

    bool quit = false;

    while(!quit)
    {
        bool endofline = false;
        int received = 0;
        int iter = 0;
        char rebuffer[80];
        char buffer[80];

        while(!endofline) {
            received = recv(clientfd, &rebuffer, 80, 0);
            for (int i=0; i<received; i++, iter++) {
                if (rebuffer[i] == '\n') {
                    endofline = true;
                }
                buffer[iter] = rebuffer[i];
                if(endofline) {
                    buffer[iter + 1] = 0;
                }
            }
            send(clientfd, buffer, received,0);
            string arr = string (rebuffer);
            cout << arr << endl;
            if (arr == "q") {
                quit = true;
                break;
            }
        }
        if (quit) {
            close(clientfd);
            close(SocketD);
            break;
        }
    }
}
