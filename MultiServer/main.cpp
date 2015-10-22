#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>

using namespace std;

void *task1(void *);

static int connFd;
int port = 80;

int main(int argc, char* argv[])
{
    int pId, portNo, listenFd;
    bool loop = false;
    struct sockaddr_in svrAdd, clntAdd;

    pthread_t threadA[3];

    portNo = 51000;

    //create socket
    listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    bzero((char*) &svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);

    //bind socket
    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        cerr << "Cannot bind" << endl;
        return 0;
    }

    listen(listenFd, 200);

    int noThread = 0;

    while (noThread < 3)
    {
        socklen_t len = sizeof(clntAdd);

        cout << "Listening" << endl;

        //this is where client connects. svr will hang in this mode until client conn
        connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

        if (connFd < 0)
        {
            cerr << "Cannot accept connection" << endl;
            return 0;
        }
        else
        {
            cout << "Connection successful" << endl;
        }

        pthread_create(&threadA[noThread], NULL, task1, NULL);

        noThread++;
    }

    for(int i = 0; i < 3; i++)
    {
        pthread_join(threadA[i], NULL);
    }


}

void *task1 (void *dummyPt)
{
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
    send(connFd, " [1] Indianapolis\n [2] Evansville\n [3] St. Louis\n Choose a city by number: \n", strlen(" [1] Evansville\n [2] St. Louis\n [3] Indianapolis\n Choose a city by number: \n"), 0);

    while (1) {
        char rebuffer[80];
        char buffer[10000];
        int citynum = 0;
        cout << "Sent prompt to client" << endl;
            recv(connFd, &rebuffer, 80, 0);
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
            send(connFd, sendbuffer, sizeof(sendbuffer), 0);
    } else {
        send(connFd, "Enter one of the options\n", strlen("Enter one of the options\n"), 0);
    }
  }
}
