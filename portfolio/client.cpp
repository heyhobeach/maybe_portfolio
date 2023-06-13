// testprojeect.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include "pch.h"
#include <iostream>

#pragma comment(lib, "ws2_32")

#define PORT	4578// ����� ��Ʈ�� �����ϰ� ����ؾ���  (ex) 21 : FTP��Ʈ, 80 : HTTP��Ʈ, 8080 : HTTPS��Ʈ)
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.219.106"// ������ ip�� ���������
#pragma once

int main() {
	WSADATA wsaData;// ������ ���� �ʱ�ȭ ���� �����ϱ� ���� ����ü �̹� ����Ǿ�����
	WSAStartup(MAKEWORD(2, 2), &wsaData);//WSAStartup(���Ϲ���, WSADATA ����ü �ּ�); �ε� MAKEWORD�� ���ؼ� ���������� ��ȯ�ؼ� �־��� 2��°�� WSADATA�� ����ü ������ Ÿ��

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//PF_INET IPV4Ÿ�� ��� �Ϲ������� ����ϴ� �ּ�, SOCK_STREAM ���������� ������ ����� ����° ���� protocoldms ��űԾ� ����� IPROTO_TCP�� TCP�� ����Ѵٴ� ��

	SOCKADDR_IN tAddr = {};//������ ���Ͽ��� ������ ������ ���� �Ǵ� ���� �ּ� �����ϴµ� ���
	tAddr.sin_family = AF_INET;//sin_family�� �ݵ�� AF_INET�̾����
	tAddr.sin_port = htons(PORT);//��Ʈ ��ȣ ���� 2����Ʈ �ȿ��� ǥ���� �� �ִ� ���ڿ�����
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);//IPv4 �Ҽ��� �ּҰ� ���Ե� ���ڿ� IN_ADDR ����ü�� �ּҷ� ��ȯ�Ѵ� ����� SERVER_IP�� �ּҸ� ��ȯ��

	connect(hSocket, (SOCKADDR*)&tAddr,sizeof(tAddr));//Ŭ���̾�Ʈ ���� bind ��� connect ���



	char cMsg[] = "Client Send";
	send(hSocket, cMsg, strlen(cMsg), 0);


	char cBuffer[PACKET_SIZE] = {};
	recv(hSocket, cBuffer, PACKET_SIZE, 0);
	printf("Recv Mssg : %s\n", cBuffer);


	closesocket(hSocket);


	WSACleanup();//���Ͽ��� ����ϴ� �Ҹ���
	std::cout << "Hello World\n";

	return 0;
}

// ���α׷� ����: <Ctrl+F5> �Ǵ� [�����] > [��������� �ʰ� ����] �޴�
// ���α׷� �����: <F5> Ű �Ǵ� [�����] > [����� ����] �޴�

// ������ ���� ��: 
//   1. [�ַ�� Ž����] â�� ����Ͽ� ������ �߰�/�����մϴ�.
//   2. [�� Ž����] â�� ����Ͽ� �ҽ� ��� �����մϴ�.
//   3. [���] â�� ����Ͽ� ���� ��� �� ��Ÿ �޽����� Ȯ���մϴ�.
//   4. [���� ���] â�� ����Ͽ� ������ ���ϴ�.
//   5. [������Ʈ] > [�� �׸� �߰�]�� �̵��Ͽ� �� �ڵ� ������ ����ų�, [������Ʈ] > [���� �׸� �߰�]�� �̵��Ͽ� ���� �ڵ� ������ ������Ʈ�� �߰��մϴ�.
//   6. ���߿� �� ������Ʈ�� �ٽ� ������ [����] > [����] > [������Ʈ]�� �̵��ϰ� .sln ������ �����մϴ�.