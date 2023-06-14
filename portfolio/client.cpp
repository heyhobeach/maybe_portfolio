// testprojeect.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
// ���⼭ �ǽð� ä�� ���α׷����� �ٲ����
//06-14 �ּ�ó���� �κ��� �������� ���� �� �� �κ�


#include "pch.h"
#include <iostream>
#include <string.h>


#pragma comment(lib, "ws2_32")

#define PORT	4578// ����� ��Ʈ�� �����ϰ� ����ؾ���  (ex) 21 : FTP��Ʈ, 80 : HTTP��Ʈ, 8080 : HTTPS��Ʈ)
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.219.105"// ������ ip�� ���������

#pragma once

using namespace std;

SOCKET hSocket;

void proc_recv() {

	cout << "thread on" << endl;
	char buff[PACKET_SIZE];
	string cmd;
	while (!WSAGetLastError()) {
		cout << "���� ����\n";
		ZeroMemory(&buff, PACKET_SIZE);//buff����
		recv(hSocket, buff, PACKET_SIZE, 0);
		cmd = buff;
		if (cmd == "hi") {//cmd�� hi��� �� �Է¹��� ���� hi��� break
			break;
		}
		cout << "server�� ���� ���� �޼��� :" << buff << endl;
	}
}

int main() {
	WSADATA wsaData;// ������ ���� �ʱ�ȭ ���� �����ϱ� ���� ����ü �̹� ����Ǿ�����
	WSAStartup(MAKEWORD(2, 2), &wsaData);//WSAStartup(���Ϲ���, WSADATA ����ü �ּ�); �ε� MAKEWORD�� ���ؼ� ���������� ��ȯ�ؼ� �־��� 2��°�� WSADATA�� ����ü ������ Ÿ��


	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//PF_INET IPV4Ÿ�� ��� �Ϲ������� ����ϴ� �ּ�, SOCK_STREAM ���������� ������ ����� ����° ���� protocoldms ��űԾ� ����� IPROTO_TCP�� TCP�� ����Ѵٴ� ��

	SOCKADDR_IN tAddr = {};//������ ���Ͽ��� ������ ������ ���� �Ǵ� ���� �ּ� �����ϴµ� ���
	tAddr.sin_family = AF_INET;//sin_family�� �ݵ�� AF_INET�̾����
	tAddr.sin_port = htons(PORT);//��Ʈ ��ȣ ���� 2����Ʈ �ȿ��� ǥ���� �� �ִ� ���ڿ�����
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);//IPv4 �Ҽ��� �ּҰ� ���Ե� ���ڿ� IN_ADDR ����ü�� �ּҷ� ��ȯ�Ѵ� ����� SERVER_IP�� �ּҸ� ��ȯ��

	//connect(hSocket, (SOCKADDR*)&tAddr,sizeof(tAddr));//Ŭ���̾�Ʈ ���� bind ��� connect ��� 
	while(1){ //������ ��� �õ��ϴ� �κ� ���� ���н� ���ѷ����� ���鼭 ��� ������ �õ��Ѵ� ���� ������ true�� ��ȯ�����μ� while�� �����
		if(!connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr))) break;//connect�� return ���� ������ 0 ���н� -1�̴� ���� ���⿡ not�� �������μ� ������ (!0)���� True�� �ǰ� ���д� (!-1)�� false�� �����
		cout << "�������";

	}
	char cMsg[PACKET_SIZE] = { 0 };
	std::thread proc1(proc_recv);//procv_recv�Լ� �����ؾ���
	//char cMsg[PACKET_SIZE] = "Client say hi";
	

	while (!WSAGetLastError()) {
		cin >> cMsg;
		send(hSocket, cMsg, strlen(cMsg), 0);//�����ϰ��� �ϴ� ������ ���̸�ŭ
	}




	//send(hSocket, cMsg, strlen(cMsg), 0);


	//char cBuffer[PACKET_SIZE] = {};
	//recv(hSocket, cBuffer, PACKET_SIZE, 0);
	//printf("Recv Mssg : %s\n", cBuffer);
	//std::cout<<"Recv Msg :%s<<std::endl;

	proc1.join();
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