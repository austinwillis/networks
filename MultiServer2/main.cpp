#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<iostream>
#include<pthread.h> //for threading , link with lpthread
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<sstream>

using namespace std;
int port = 80;

void *connection_handler(void *);

int main(int argc , char *argv[])
{
    int socket_desc , new_socket , c , *new_sock;
    struct sockaddr_in server , client;
    char *message;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 100);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        //Reply to the client

        pthread_t sniffer_thread;
        new_sock = (int*)malloc(1);
        *new_sock = new_socket;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }

    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];

    //Send some messages to the client
    message = " [1] Indianapolis\n [2] Evansville\n [3] St. Louis\n Choose a city by number: \n";
    send(sock , message , strlen(message), 0);

    //Receive a message from client
        //Send the message back to client
        int Soc = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        char buffer[10000];
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
        int citynum = 0;
        cout << "Sent prompt to client" << endl;
    while (read_size = recv(sock , client_message , 20 , 0) > 0) {
    input = string (client_message);
    cout << input << endl;
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
            send(sock, sendbuffer, sizeof(sendbuffer), 0);
            cout << sendbuffer << endl;
    } else {
        send(sock, "Enter one of the options\n", strlen("Enter one of the options\n"), 0);
    }
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
