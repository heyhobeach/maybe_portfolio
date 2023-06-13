// testprojeect.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

#pragma comment(lib, "ws2_32")

#define PORT	4578// 예약된 포트를 제외하고 사용해야함  (ex) 21 : FTP포트, 80 : HTTP포트, 8080 : HTTPS포트)
#define PACKET_SIZE 1024

#pragma once

int main() {
	WSADATA wsaData;// 윈도우 소켓 초기화 정보 저장하기 위한 구조체 이미 선언되어있음
	WSAStartup(MAKEWORD(2, 2), &wsaData);//WSAStartup(소켓버전, WSADATA 구조체 주소); 인데 MAKEWORD를 통해서 정수값으로 변환해서 넣어줌 2번째는 WSADATA의 구조체 포인터 타입

	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//PF_INET IPV4타입 사용 일반적으로 사용하는 주소, SOCK_STREAM 연결지향형 소켓을 만든다 세번째 인자 protocoldms 통신규약 현재는 IPROTO_TCP로 TCP를 사용한다느 말

	SOCKADDR_IN tListenAddr = {};//윈도우 소켓에서 소켓을 연결할 로컬 또는 원격 주소 지정하는데 사용
	tListenAddr.sin_family = AF_INET;//sin_family는 반드시 AF_INET이어야함
	tListenAddr.sin_port = htons(PORT);//포트 번호 설정 2바이트 안에서 표현할 수 있는 숫자여야함
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);//현재 동작되는 컴퓨터의 IP주소 INADDR_ANY면 현재 동작되는 컴퓨터의 IP주소 s.addr은IPv4 internet address의미

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));//bind(소켓구조체, 소켓 구성요소 구조체의 주소, 그 구조체의 크기) bind의 역활은 소켓에 주소정보 연결해주는것 
	listen(hListen, SOMAXCONN);//Listen은 소켓의 연결 요청 대기상태, 두번째 인자인 backlog는 연결 요청 대기 큐의 크기 값이 5라면 클라이언트의 연결 요청 5개만 대기 가능 SOMAXCONN 해당값은 최대치만큼 대기가능

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);//accept(소켓, 소켓 구성요소 구조체 주소,그 구조체를 담고있는 별수 크기

	char cBuffer[PACKET_SIZE] = {};
	recv(hClient, cBuffer, PACKET_SIZE, 0);//대상 소켓으로 보내온 정보를 받아주는 역활
	printf("Recv Mssg : %s\n", cBuffer);

	char cMsg[] = "Server Send";
	send(hClient, cMsg, strlen(cMsg), 0);

	closesocket(hClient);
	closesocket(hListen);

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