
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <vector>
using namespace std;

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable: 4996)

SOCKET* clientSocket = new SOCKET[16];

void clientsthread(SOCKET clientSocket)
{

    char buf[4096];

    while (true) {

        ZeroMemory(buf, 4096);

        int bytesReceived = recv(clientSocket, buf, 4096, 0);

        if (string(buf, 0, bytesReceived) == "filesend") {
            ZeroMemory(buf, 4096);
            int bytesReceived = recv(clientSocket, buf, 4096, 0);
            string sizename = string(buf, 0, bytesReceived);
            string savepath = "C:\\Users\\Puplic";
            string delimiter = ":";
            size_t pos = 0;
            string resultsizename;
            pos = sizename.find(delimiter);
            string size = sizename.substr(0, pos);
            sizename.erase(0, pos + delimiter.length());
            savepath += "\\";
            savepath += sizename;

            double sizeb = stoi(size);

            FILE* fd = fopen(savepath.c_str(), "wb");
            send(clientSocket, "send", 4, 0);
            double counter = 0;
            while (true) {

                ZeroMemory(buf, 4096);
                int bytesReceived = recv(clientSocket, buf, 4096, 0);
                counter += bytesReceived;
                int resultpercent = (counter / sizeb) * 100;
                cout << "\r" << resultpercent << "%";
                send(clientSocket, "ok", 2, 0);
                if (string(buf, 0, bytesReceived) == "xdd") {
                    break;
                }

                fwrite(buf, 1, bytesReceived, fd);

            }
            cout << "\r" << "100%" << endl;
            fclose(fd);
            cout << "data sucessfully transfered" << endl;
            ZeroMemory(buf, 4096);


        }
        if (bytesReceived == SOCKET_ERROR) {

            break;
        }

        if (bytesReceived == 0) {

            cout << "client has been disconnected" << endl;
        }

        cout << string(buf, 0, bytesReceived) << endl;

    }

}

void getclients() {
    //Activates Winsock32

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int ws0k = WSAStartup(ver, &wsData);
    if (ws0k != 0) {

        std::cout << "Something went really bad!";

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
    std::vector<std::thread> threads;
    for (int i = 0; i < 16; i++) {
        sockaddr_in client;
        int clientSize = sizeof(client);
        clientSocket[1] = accept(listening, (sockaddr*)&client, &clientSize);

        cout << "client " << i << " is connected" << endl;

        threads.emplace_back(clientsthread, clientSocket[i]);

    }
}



int main()
{
    std::thread getclientsthread(getclients);

    while (true) {
        string userInput;
        int clientnumber = 0;
        
        getline(cin, userInput);

        std::string delimiter = " ";

        size_t pos = 0;
        std::string* command = new string[20];


                pos = userInput.find(delimiter);
                command[0] = userInput.substr(0, pos);
                userInput.erase(0, pos + delimiter.length());
                pos = userInput.find(delimiter);
                command[1] = userInput.substr(0, pos);
                userInput.erase(0, pos + delimiter.length());
                command[2] = userInput;
            
            
                if (command[0] == "client") {
                    clientnumber = std::stoi(command[1]);
                    int sendResult = send(clientSocket[clientnumber], command[2].c_str(), command[2].size() + 1, 0);
                }
                else
                {
                    cout << "you have to select a client";
                }


    }
}



    










