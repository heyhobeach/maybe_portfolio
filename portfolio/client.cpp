
// testprojeect.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// 여기서 실시간 채팅 프로그램으로 바꿔야함 연결이 끊긴 후 다시 연결 했을때 연결 가능함 다만 지금은 고정된 메시지만 보내고 있는데 여기서 채팅이 가능하도록 수정해야함
//실행되는코드 쓰레드 사용?


#include "pch.h"
#include <iostream>
#include <string.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <thread>


#pragma comment(lib, "ws2_32")

#define PORT	4578// 예약된 포트를 제외하고 사용해야함  (ex) 21 : FTP포트, 80 : HTTP포트, 8080 : HTTPS포트)
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.219.100"// 서버의 ip로 맞춰줘야함

#pragma once

using namespace std;



void send_input(string text, int& num, SOCKET& socket) {//cin에서 메세지 보낼걸 굳이 포인터로 접근해야할까?
     char buff[PACKET_SIZE] = {0};
     string message;
     string strNum;
     while (!WSAGetLastError()) {
         //cout << "스레드와 소켓 연결 성공" << endl;
         cin >> buff;
         //cout << text << endl;
         //num++;
         //strNum = static_cast <string> (strNum);
         //text += strNum;
         //cout << text << " roof : " << num++ << endl;
         send(socket,buff,sizeof(buff), 0);

         Sleep(1000);
     }
}


int main()
{
    WSAData wsaData;
    int testNum = 0;
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

    thread t1(send_input, "hello", ref(testNum), ref(clientSocket));//레퍼런스로 전달하려면 ref 함수로 감싸야함

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

    cout << "Connected to Sever!" << endl;//서버가 연결 되지 않았는데 연결 되었다고 함

    char sendBuffer[100] = "Hello World";

    // Send
    while (true)
    {
        if (::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0) == SOCKET_ERROR)
        {
            // 원래 블록했어야 했는데... 너가 논블로킹으로 하라며?
            if (::WSAGetLastError() == WSAEWOULDBLOCK) {
                //Sleep(1000);
                continue;
            }

            // Error
            cout << "send fail" << endl;// 전송하는 부분인데 현재 서버와 연결이 되어있지 않은 상태라서 연결 실패
            //break; //자꾸 여기에 걸려서 에러났음 그런데 여기가 에러부분이라서 while에 break를 걸었는데 여기를 break를 안 걸었을때 진짜 에러가 난다면? 해결방법은?
        }

        //cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;

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
                cout << "disconnect" << endl;//현재 이 지점이 서버와 연결이 안 되어서 recv를 하지 못 하는 부분
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
        //cout << "check num" << testNum << endl;
        this_thread::sleep_for(1s);
    }
    t1.join();
    // 소켓 리소스 반환
    ::closesocket(clientSocket);

    // 윈속 종료
    ::WSACleanup();
}