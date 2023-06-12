#include "stdafx.h"
#include<iostream>
#include<stdio.h>
#include<WinSock2.h>

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

	WSACleanup();//소켓에서 사용하는 소멸자
	std::cout << "Hello World\n";

	return 0;
}