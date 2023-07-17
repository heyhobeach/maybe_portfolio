
// testprojeect.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// 여기서 실시간 채팅 프로그램으로 바꿔야함 연결이 끊긴 후 다시 연결 했을때 연결 가능함 다만 지금은 고정된 메시지만 보내고 있는데 여기서 채팅이 가능하도록 수정해야함
//실행되는코드 쓰레드 사용?


#include "pch.h"
#include <iostream>
#include <string.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <thread>
#include <string>
#include <chrono>//c++20기준
#include<format>

#pragma comment(lib, "ws2_32")

#define PORT	4578// 예약된 포트를 제외하고 사용해야함  (ex) 21 : FTP포트, 80 : HTTP포트, 8080 : HTTPS포트)
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.219.106"// 서버의 ip로 맞춰줘야함//"192.168.219.100"

#pragma once

using namespace std;
using namespace std::chrono;


void send_input(string name, int& num, SOCKET& socket) {//cin에서 메세지 보낼걸 굳이 포인터로 접근해야할까?
    char buff[PACKET_SIZE] = { 0 };
    string message = "";
    string strNum;
    const char* cmessage;
    while (!WSAGetLastError()) {
        //cout << "스레드와 소켓 연결 성공" << endl;
        //cin >> buff;
        getline(cin, message);//공백 포함 입력 받는 과정 string 라이브러리에 들어있음
        const local_time<system_clock::duration> local_now = zoned_time{ current_zone(), system_clock::now() }.get_local_time();//로컬 시간
        //const time_point<std::chrono::system_clock, std::chrono::days> dp = std::chrono::floor<std::chrono::days>(local_now);//pratice 프로젝트에서 확인
        //std::chrono::year_month_day ymd{dp};
        //seconds local_sec = duration_cast<seconds>(local_now).count();
        //cin >> message;

        string s=format("{}")
        //string s= std::format("{:%Y%m%d%H%M}", local_now);
        //message = s + "사용자>>" + name + message;
        message = "사용자>>" + name + message;
        //cmessage = ""; 필요없는 부분 위에서 어차피 새로 초이화 하면서 넣기때문
        cmessage = message.c_str();
        cout << sizeof(name) << endl << sizeof(buff) << endl;
        cout << sizeof(message) << endl;
        cout << sizeof(cmessage) << endl;// 이게 지금 8로 잘림
        cout << "message 길이" <<strlen(cmessage)<< endl;
        //cout << local_now;
        cout << " - ";
        cout << "전송 메세지 내용 : " << "\"" << cmessage << "\"" << endl;
        //cout << text << endl;
        //num++;
        //strNum = static_cast <string> (strNum);
        //text += strNum;
        //cout << text << " roof : " << num++ << endl;
        send(socket, cmessage, strlen(cmessage)+1, 0);//원래는 sizeof(cmessage)인데 sizeof(cmessage)가 8로 나와서 전송에서 잘리는 현상 발생 해당 size를 딱 맞게 수정 하는 방법을 찾아야함 -> strlen으로 char로 받더라도 길이만큼 받아서 전송함 +1을 해서 뒤에 널을 넣을수 있도록함

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
    string name = "";
    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    ::inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = ::htons(PORT);
    cin >> name;

    thread t1(send_input, name, ref(testNum), ref(clientSocket));//레퍼런스로 전달하려면 ref 함수로 감싸야함

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
    /*while (true)
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

            //cout << "Recv Data Len = " << recvLen << endl;
            break;
        }
        //cout << "check num" << testNum << endl;
        this_thread::sleep_for(1s);
    }*/
    t1.join();
    // 소켓 리소스 반환
    ::closesocket(clientSocket);

    // 윈속 종료
    ::WSACleanup();
}