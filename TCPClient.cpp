
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <windows.h>
#include <lmcons.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    string ipAdress = "127.0.0.1"; //local IP adress
    int port = 4444; //port used by our TCP server

    //Activates Winsock

    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0) {
        cout << "something went really bad, exiting programm";
        return 0;
    }

    //creates socket

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); //little notice here => AF_INET stands for IPv4 and SOCK_STREAM for the TCP_Product
    if (sock == INVALID_SOCKET) {
        cout << "socket could not be created, exiting";
        WSACleanup();
        return 0;                                 //ends opertation if socket creation has failed

    }

    //Fill in hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

    //connect the server


    int connectResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectResult == SOCKET_ERROR)
    {
        cout << "Connection to server could not be created";
        closesocket(sock);
        return 0;                           //closes down if the connection the server could not be created
    }

    //loop where data gets retrieved

    char buf[4096];
    while (true) {

        ZeroMemory(buf, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived > 0) {

            cout << "server says:" << string(buf, 0, bytesReceived) << endl;

        }


    }




}

