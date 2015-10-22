#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include <netdb.h>
#include <thread>

using namespace std;

vector<unique_ptr<thread>> threads;

int port = 80;
int serverport = 51000;

void *ClientHandler(void* threadid){
    int Soc = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("weather.yahooapis.com");
    sockaddr_in SockAddr;
    SockAddr.sin_port=htons(port);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    cout << "Get xml from yahoo on port " << port << endl;
    string input;
    int city;
    if(connect(Soc,(sockaddr*)(&SockAddr),sizeof(SockAddr)) != 0){
            cout << "Could not connect";
    }
    send(clientfd, " [1] Indianapolis\n [2] Evansville\n [3] St. Louis\n Choose a city by number: \n", strlen(" [1] Evansville\n [2] St. Louis\n [3] Indianapolis\n Choose a city by number: \n"), 0);

    while (1) {
        char rebuffer[80];
        char buffer[10000];
        int citynum = 0;
        cout << "Sent prompt to client" << endl;
            recv(clientfd, &rebuffer, 80, 0);
            input = string (rebuffer);
            cout << input;
      city = atoi(input.c_str());
    if (city == 1 || city == 2 || city == 3) {
        char sendbuffer[10000];
        memset(&sendbuffer[0], 0, sizeof(sendbuffer));
        switch(city) {
        case 1: citynum = 2427032;
                strcat(sendbuffer, "Indianapolis\n");
                break;
        case 2: citynum = 2400767;
                strcat(sendbuffer, "Evansville\n");
                break;
        case 3: citynum = 2486982;
                strcat(sendbuffer, "St. Louis\n");
                break;
        }
            string url = "GET /forecastrss?w=" + to_string(citynum) + " HTTP/1.1\n";
            cout << url;
            send(Soc,url.c_str(), strlen(url.c_str()), 0);
            send(Soc, "host: weather.yahooapis.com\n\n", strlen("host: weather.yahooapis.com\n\n"), 0);
            recv(Soc,buffer,10000,0);
            string tmpstr(buffer, strlen(buffer));
            istringstream is(tmpstr);
            string line;
            while (getline(is, line)) {
                if (line == "<b>Current Conditions:</b><br />") {
                    getline(is, line);
                    line.replace(line.find("<BR />"), sizeof("<BR />")-1, "");
                    strcat(sendbuffer, line.c_str());
                    strcat(sendbuffer, "\n");
                }
                if (line == "<BR /><b>Forecast:</b><BR />") {
                    for (int i = 0; i < 5; i++) {
                        getline(is, line);
                        line.replace(line.find("<br />"), sizeof("<br />")-1, "");
                        strcat(sendbuffer, line.c_str());
                        strcat(sendbuffer, "\n");
                    }
                }

            }
            strcat(sendbuffer, " [1] Indianapolis\n [2] Evansville\n [3] St. Louis\n Choose a city by number: ");
            send(clientfd, sendbuffer, sizeof(sendbuffer), 0);
    } else {
        send(clientfd, "Enter one of the options\n", strlen("Enter one of the options\n"), 0);
    }
  }
}


int main () {

    while (1) {
    int server;
    server = socket(AF_INET, SOCK_STREAM,0);
    cout << "My Socket Descriptor:" << server << endl;
    struct sockaddr_in self;
    self.sin_family = AF_INET;
    self.sin_port = htons(serverport);
    self.sin_addr.s_addr = INADDR_ANY;

    bind(server, (struct sockaddr*)&self, sizeof(self));
    cout << "Server is listening on port: " << serverport << endl;

    listen(server, 100);

    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    int clientfd;

    cout << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;

    for(;;){
            clientfd = accept(server,(sockaddr*)&client_addr , (socklen_t*)&addrlen);
            cout << "CONNECTION MADE!" << endl;
            rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
          }
    }
 return 0;
}

