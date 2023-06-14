// testprojeect.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// 여기서 실시간 채팅 프로그램으로 바꿔야함
//06-14 주석처리한 부분이 집에가서 수정 해 볼 부분


#include "pch.h"
#include <iostream>
#include <string.h>


#pragma comment(lib, "ws2_32")

#define PORT	4578// 예약된 포트를 제외하고 사용해야함  (ex) 21 : FTP포트, 80 : HTTP포트, 8080 : HTTPS포트)
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.219.105"// 서버의 ip로 맞춰줘야함

#pragma once

using namespace std;

SOCKET hSocket;

void proc_recv() {

	cout << "thread on" << endl;
	char buff[PACKET_SIZE];
	string cmd;
	while (!WSAGetLastError()) {
		cout << "연결 성공\n";
		ZeroMemory(&buff, PACKET_SIZE);//buff비우기
		recv(hSocket, buff, PACKET_SIZE, 0);
		cmd = buff;
		if (cmd == "hi") {//cmd가 hi라면 즉 입력받은 값이 hi라면 break
			break;
		}
		cout << "server로 부터 받은 메세지 :" << buff << endl;
	}
}

int main() {
	WSADATA wsaData;// 윈도우 소켓 초기화 정보 저장하기 위한 구조체 이미 선언되어있음
	WSAStartup(MAKEWORD(2, 2), &wsaData);//WSAStartup(소켓버전, WSADATA 구조체 주소); 인데 MAKEWORD를 통해서 정수값으로 변환해서 넣어줌 2번째는 WSADATA의 구조체 포인터 타입


	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//PF_INET IPV4타입 사용 일반적으로 사용하는 주소, SOCK_STREAM 연결지향형 소켓을 만든다 세번째 인자 protocoldms 통신규약 현재는 IPROTO_TCP로 TCP를 사용한다느 말

	SOCKADDR_IN tAddr = {};//윈도우 소켓에서 소켓을 연결할 로컬 또는 원격 주소 지정하는데 사용
	tAddr.sin_family = AF_INET;//sin_family는 반드시 AF_INET이어야함
	tAddr.sin_port = htons(PORT);//포트 번호 설정 2바이트 안에서 표현할 수 있는 숫자여야함
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);//IPv4 소수점 주소가 포함된 문자열 IN_ADDR 구조체의 주소로 변환한다 현재는 SERVER_IP의 주소를 변환중

	//connect(hSocket, (SOCKADDR*)&tAddr,sizeof(tAddr));//클라이언트 측은 bind 대신 connect 사용 
	while(1){ //연결을 계속 시도하는 부분 연결 실패시 무한루프를 돌면서 계속 연결을 시도한다 연결 성공시 true를 반환함으로서 while을 벗어난다
		if(!connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr))) break;//connect의 return 값은 성공시 0 실패시 -1이다 따라서 여기에 not을 붙임으로서 성공은 (!0)으로 True가 되고 실패는 (!-1)로 false를 만든다
		cout << "연결실패";

	}
	char cMsg[PACKET_SIZE] = { 0 };
	std::thread proc1(proc_recv);//procv_recv함수 정의해야함
	//char cMsg[PACKET_SIZE] = "Client say hi";
	

	while (!WSAGetLastError()) {
		cin >> cMsg;
		send(hSocket, cMsg, strlen(cMsg), 0);//전송하고자 하는 문자의 길이만큼
	}




	//send(hSocket, cMsg, strlen(cMsg), 0);


	//char cBuffer[PACKET_SIZE] = {};
	//recv(hSocket, cBuffer, PACKET_SIZE, 0);
	//printf("Recv Mssg : %s\n", cBuffer);
	//std::cout<<"Recv Msg :%s<<std::endl;

	proc1.join();
	closesocket(hSocket);


	WSACleanup();//소켓에서 사용하는 소멸자
	std::cout << "Hello World\n";

	return 0;
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