// testprojeect.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//현재 서버에서 길게 입력을 못 받고 있음

#include "pch.h"
#include <iostream>
#include <stdint.h>
#include <vector>
#pragma comment(lib, "ws2_32")

#define PORT	4578// 예약된 포트를 제외하고 사용해야함  (ex) 21 : FTP포트, 80 : HTTP포트, 8080 : HTTPS포트)
#define PACKET_SIZE 10000

#pragma once

using namespace std;

SOCKET hListen, hClient;
u_long nonBlockingMode = 1;//굳이 1이 아니더라도 괜찮음 0이면 블로킹 나머지는 논블로킹임

void func1() {// 연결과 동시에 작동 되어있음
	for (int i = 0; i < 10; i++) {
		cout << "쓰레드 함수 작동중" << endl;
		Sleep(1000);
	}
}

void proc_recvs() {
	char buff[PACKET_SIZE] = { 0 };
	//string cmd;

	while (!WSAGetLastError()) {
		cout << "연결성공\n";
		ZeroMemory(&buff, PACKET_SIZE);//ZeroMemory는 함수가 아닌 매크로
		//ioctlsocket(hClient, FIONBIO, &nonBlockingMode);
		recv(hClient, buff, PACKET_SIZE, 0);//flag 0로 일반 데이터 수신
		//소캣으로 연결하고 buff에 전달받은 데이터 저장, PACKET_SIZE는 읽을 데이터의 크기
		cout << "client로부터 받은 메세지 : " << buff << endl;
	}
	cout << "연결 종료";

}

int main() {
	vector<string> message_log;
	WSADATA wsaData;// 윈도우 소켓 초기화 정보 저장하기 위한 구조체 이미 선언되어있음
	WSAStartup(MAKEWORD(2, 2), &wsaData);//WSAStartup(소켓버전, WSADATA 구조체 주소); 인데 MAKEWORD를 통해서 정수값으로 변환해서 넣어줌 2번째는 WSADATA의 구조체 포인터 타입

	//vector<int> ip_vec;

	int ip_addr = 0;

	//SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//PF_INET IPV4타입 사용 일반적으로 사용하는 주소, SOCK_STREAM 연결지향형 소켓을 만든다 세번째 인자 protocoldms 통신규약 현재는 IPROTO_TCP로 TCP를 사용한다느 말
	if (ioctlsocket(hListen, FIONBIO, &nonBlockingMode) == INVALID_SOCKET) {//SOCKET socket 실패시 INVALD_SOCKET 리턴 즉 ioctlsocket
		return 0;
	}

	SOCKADDR_IN tListenAddr = {};//윈도우 소켓에서 소켓을 연결할 로컬 또는 원격 주소 지정하는데 사용
	SOCKADDR_IN myaddr;
	tListenAddr.sin_family = AF_INET;//sin_family는 반드시 AF_INET이어야함
	tListenAddr.sin_port = htons(PORT);//포트 번호 설정 2바이트 안에서 표현할 수 있는 숫자여야함
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);//현재 동작되는 컴퓨터의 IP주소 INADDR_ANY면 현재 동작되는 컴퓨터의 IP주소 s.addr은IPv4 internet address의미

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));//bind(소켓구조체, 소켓 구성요소 구조체의 주소, 그 구조체의 크기) bind의 역활은 소켓에 주소정보 연결해주는것 
	listen(hListen, SOMAXCONN);//Listen은 소켓의 연결 요청 대기상태, 두번째 인자인 backlog는 연결 요청 대기 큐의 크기 값이 5라면 클라이언트의 연결 요청 5개만 대기 가능 SOMAXCONN 해당값은 최대치만큼 대기가능

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	thread t1(func1);



	while (true) {
		hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);//accept(소켓, 소켓 구성요소 구조체 주소,그 구조체를 담고있는 별수 크기

		//ip_addr = getpeername(hClient, (SOCKADDR*)&myaddr, &iClntSize);//이걸 넣으면 지금 자꾸 함수가 그냥 종료됨
		if (hClient == INVALID_SOCKET) {
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;//non block하기위한 부분 block되면 continue해라
			break;//block도 아니고 소켓 생성 실패하면 break
		}
		cout << "Client Connected" << inet_ntoa(tClntAddr.sin_addr) << endl;//inet_ntoa도 ip주소를 받아오는데 해당 함수가 getpeername과 차이는?
		//recv 부분
		while (true) {
			char recvBuffer[10000];
			int recvLen = recv(hClient, recvBuffer, PACKET_SIZE, 0);
			//recv(hClient, recvBuffer, PACKET_SIZE, 0);
			if (recvLen == SOCKET_ERROR) {
				if (WSAGetLastError() == WSAEWOULDBLOCK) {
					continue;//non block하기위한 부분 block되면 continue해라
				}
				break;
			}
			else if (recvLen == 0) {
				break;
			}

			//cout << "Recv Data Len=" << recvLen << endl;

			while (true) {
				if (send(hClient, recvBuffer, strlen(recvBuffer), 0) == SOCKET_ERROR) {
					if (WSAGetLastError() == WSAEWOULDBLOCK) {
						continue;//기본적으로 continue
					}
					break;
				}
				//cout << "send Data: Len" << recvLen << endl;
				cout << "send Data :" << recvBuffer << endl;
				break;
			}
		}
	}

	WSACleanup();//소켓에서 사용하는 소멸자

	char cBuffer[PACKET_SIZE] = { 0 };
	//thread proc2(proc_recvs);
	//recv(hClient, cBuffer, PACKET_SIZE, 0);//대상 소켓으로 보내온 정보를 받아주는 역활
	//printf("Recv Mssg : %s\n", cBuffer);

	/*while (!WSAGetLastError()) {
		cout << "입력 :";
		cin >> cBuffer;
		send(hClient, cBuffer, strlen(cBuffer), 0);
	}proc2.join();*/

	//char cMsg[] = "Server Send";
	//send(hClient, cMsg, strlen(cMsg), 0);

	t1.join();

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();//소켓에서 사용하는 소멸자
	//std::cout << "Hello World\n";

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
