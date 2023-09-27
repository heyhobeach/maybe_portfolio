// clientproject.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// 서버

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#pragma comment(lib,"ws2_32.lib")//해당 줄이 없으면 프로젝트에 라인 추가 해야함
using namespace std;

#define PACKET_SIZE 1024
SOCKET client_sock;

struct cal_data {
    int left_num;
    int right_num;
    char op;
    char text[1024];
    int result;
    short int error;

};

std::vector<char> SerializeMyData(const cal_data& data) {
    std::vector<char> buffer(sizeof(data));
    memcpy(buffer.data(), &data, sizeof(data));
    return buffer;
}

cal_data DeserializeMyData(const std::vector<char>& buffer) {
    cal_data data;
    memcpy(&data, buffer.data(), sizeof(data));
    return data;
}

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    struct cal_data sdata;
    int len;
    string name;
    cin >> name;
    strncpy(sdata.text, name.c_str(), sizeof(sdata.text));
    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4444);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ZeroMemory((void*)&sdata, sizeof(sdata));//데이터 초기화
    sdata.left_num = 100;
    sdata.right_num = 50;
    sdata.op = '+';


    while (1) {
        if (!connect(client_sock, (SOCKADDR*)&addr, sizeof(addr))) {
            break;
        }
    }

    len = sizeof(sdata);




    while (TRUE) {
        cin >> sdata.left_num >> sdata.right_num;
        vector<char> serializedData = SerializeMyData(sdata);
        send(client_sock, serializedData.data(), len, 0);
    }

    closesocket(client_sock);
    WSACleanup();
    std::cout << "Hello client!\n";
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
