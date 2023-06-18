
// testprojeect.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// 여기서 실시간 채팅 프로그램으로 바꿔야함
//실행되는코드


#include "pch.h"
#include <iostream>
#include <string.h>
#include <WS2tcpip.h>
#include <windows.h>


#pragma comment(lib, "ws2_32")

#define PORT	4578// 예약된 포트를 제외하고 사용해야함  (ex) 21 : FTP포트, 80 : HTTP포트, 8080 : HTTPS포트)
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.219.109"// 서버의 ip로 맞춰줘야함

#pragma once

using namespace std;

int main()
{
    WSAData wsaData;
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
        return 0;

    SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
        return 0;

    u_long on = 1;
    if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
        return 0;

    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    ::inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = ::htons(PORT);

    // Connect
    while (true)
    {
        if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        {
            // 원래 블록했어야 했는데 ... 너가 논블로킹으로 하라며?
            if (::WSAGetLastError() == WSAEWOULDBLOCK)
                continue;

            if (::WSAGetLastError() == WSAEISCONN) {
                cout << "여기에 걸림" << endl;
                break;
            }

            // Error
            break;
        }
    }

    cout << "Connected to Sever!" << endl;

    char sendBuffer[100] = "Hello World";

    // Send
    while (true)
    {
        if (::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0) == SOCKET_ERROR)
        {
            // 원래 블록했어야 했는데... 너가 논블로킹으로 하라며?
            if (::WSAGetLastError() == WSAEWOULDBLOCK) {
                Sleep(1000);
                continue;
            }

            // Error
            cout << "here" << endl;
            //break; 자꾸 여기에 걸려서 에러났음 그런데 여기가 에러부분이라서 while에 break를 걸었는데 여기를 break를 안 걸었을때 진짜 에러가 난다면? 해결방법은?
        }

        cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;

        while (true)
        {
            char recvBuffer[1000];
            int recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
            if (recvLen == SOCKET_ERROR)
            {
                // 원래 블록했어야 했는데... 너가 논블로킹으로 하라며?
                if (::WSAGetLastError() == WSAEWOULDBLOCK)
                    continue;

                // Error
                break;
            }
            else if (recvLen == 0)
            {
                // 연결 끊김
                break;
            }

            cout << "Recv Data Len = " << recvLen << endl;
            break;
        }

        this_thread::sleep_for(1s);
    }

    // 소켓 리소스 반환
    ::closesocket(clientSocket);

    // 윈속 종료
    ::WSACleanup();
}