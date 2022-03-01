
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <vector>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
    //Activates Winsock32

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int ws0k = WSAStartup(ver, &wsData);
    if (ws0k != 0) {

        std::cout << "Something went really bad!";
        return 0;
    }

    //creating Socket listener

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        std::cout << "Socket is Invalid";
    }

    //binds Socket to IP and PORT

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(4444);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; //listens to any IP Adress

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    listen(listening, SOMAXCONN);


    //Waiting for Client to connect

    sockaddr_in client;
    int clientSize = sizeof(client);
    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    cout << "Client is connected" << endl;
    string input;
    getline(cin, input);

    int sendResult = send(clientSocket, input.c_str(), input.size() + 1, 0);


    

   





    return 0;
}


