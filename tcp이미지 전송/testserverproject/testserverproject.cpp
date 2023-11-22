// testprojeect.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//server


#include "pch.h"
#include <iostream>
#include <stdint.h>
#include <vector>
#include<WS2tcpip.h>
#include<sstream>
#include<fstream>
#include<algorithm>

#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/access.hpp>         

using namespace std;
struct MyStruct {
	int data1;
	double data2;
	char data3[100];
	int arr[10];
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& data1;
		ar& data2;
		ar& data3;
		ar& arr;
	}

};

int main() {
	try {
		boost::asio::io_context io_context;

		// 서버 소켓 생성
		boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345));

		std::cout << "Waiting for client to connect..." << std::endl;

		// 클라이언트 연결 대기
		boost::asio::ip::tcp::socket socket(io_context);
		acceptor.accept(socket);

		std::cout << "Client connected." << std::endl;

		// 데이터 수신
		while (true) {//추가한부분
			std::vector<char> buff(1024);
			boost::asio::streambuf receive_buffer;
			//boost::asio::read_until(socket, receive_buffer, '\0'); 구조체 받는라인
			std::istream archive_stream(&receive_buffer);
			//boost::archive::text_iarchive archive(archive_stream); 해당 라인에서 에러를 발생시킴
			//socket.read_some(boost::asio::buffer(buff));//일반 버퍼가 아닌 boost::asio::buffer에서 데이터를 변환해줌 해당라인은 구조체를 받는게 아닌 그냥 일반 string 받는라인
			std::string str(buff.begin(), buff.end());

			boost::system::error_code error;
			uintmax_t fileSize;//전송 받을 파일 size를 저장할 변수
			boost::asio::read(socket, boost::asio::buffer(&fileSize, sizeof(fileSize)), error);//client로 부터 fileSize를 전송받음

			const std::size_t bufferSize = 1024;//버퍼 받는 크기는 1024
			char buffer[bufferSize];//1024 크기 만큼의 버퍼를 생성
			
			std::ofstream file("received_image.jpg", std::ios::binary);//해당이름으로 파일을 만들어서 작성한다
			if (!file) {//파일이 안 열렸을때
				std::cerr << "Error opening file" << std::endl;
				return 1;
			}

			while (fileSize > 0) {
				//size_t bytesRead = 0;
				std::size_t bytesRead = boost::asio::read(socket, boost::asio::buffer(buffer, min(fileSize, static_cast<uintmax_t>(bufferSize))), error);
				file.write(buffer, bytesRead);
				fileSize -= bytesRead;//버퍼에 1024씩 빼면서 계속 받음 그렇게 사이즈가 0이 된다면 이미지를 모두 수신한것임
			}

			std::cout << "이미지 수신 성공" << std::endl;

			MyStruct received_data;
			//archive >> received_data;

			// 수신된 데이터 출력
			std::cout << "Received Struct:" << std::endl;//현재 이상태로 실행 하게 된다면 사진은 받는데 안의 구조체가 없어서 아래의 내용에서 에러발생
			std::cout << "data1: " << received_data.data1 << std::endl;
			std::cout << "data2: " << received_data.data2 << std::endl;
			std::cout << "data3: " << received_data.data3 << std::endl;
			std::cout << "data3: " << received_data.arr << std::endl;
			for (auto i : received_data.arr) {
				if (i == NULL)break;
				std::cout << i << std::endl;
			}
			std::cout << str << std::endl;
			//for (auto i : buff) {
			//	std::cout << i;
			//}std::cout << std::endl;
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}

/*

#pragma comment(lib, "ws2_32")

#define PORT	4578// 예약된 포트를 제외하고 사용해야함  (ex) 21 : FTP포트, 80 : HTTP포트, 8080 : HTTPS포트)
#define PACKET_SIZE 1024

#pragma once

using namespace std;

SOCKET hListen, hClient;
u_long nonBlockingMode = 1;//굳이 1이 아니더라도 괜찮음 0이면 블로킹 나머지는 논블로킹임

struct Info {//server와 client두개가 같아야함
	string name;
	string content;
	string teststr;
	int year;
	int month;
	int day;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& name;
		ar& content;
		ar& teststr;
		ar& year;
		ar& month;
		ar& day;
	}
};

void func1() {// 연결과 동시에 작동 되어있음
	for (int i = 0; i < 10; i++) {
		cout << "쓰레드 함수 작동중" << endl;
		Sleep(1000);
	}
}

void printInfo(Info* info) {
	cout << "name :" << info->name << endl;
	cout << "content :" << info->content << endl;
	cout << "year :" << info->year << endl;
	cout << "month :" << info->month << endl;
	cout << "day :" << info->day << endl;
}


int main() {
	vector<string> message_log;//현재는 사용하지 않는 상태
	WSADATA wsaData;// 윈도우 소켓 초기화 정보 저장하기 위한 구조체 이미 선언되어있음
	WSAStartup(MAKEWORD(2, 2), &wsaData);//WSAStartup(소켓버전, WSADATA 구조체 주소); 인데 MAKEWORD를 통해서 정수값으로 변환해서 넣어줌 2번째는 WSADATA의 구조체 포인터 타입

	Info rinfo;

	int ip_addr = 0;

	string username = "";
	int year = 0;
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
	//thread t1(func1);



	while (true) {
		hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);//accept(소켓, 소켓 구성요소 구조체 주소,그 구조체를 담고있는 별수 크기

		//ip_addr = getpeername(hClient, (SOCKADDR*)&myaddr, &iClntSize);//이걸 넣으면 지금 자꾸 함수가 그냥 종료됨
		if (hClient == INVALID_SOCKET) {
			int error = WSAGetLastError();
			//if (WSAGetLastError() == WSAEWOULDBLOCK) {
			//	continue;//non block하기위한 부분 block되면 continue해라
			//}
			//break;//block도 아니고 소켓 생성 실패하면 break
			cerr << "소켓 생성 실패 코드 :" << error<<endl;
			return 1;
		}
		char clientIP[20] = { 0 };
		if (inet_ntop(AF_INET, &tListenAddr.sin_addr, clientIP, sizeof(clientIP)) == NULL) {//ip주소를 받아오는 곳
			cout << "에러" << endl;
		}
		cout << "Client Connected" << clientIP << endl;//inet_ntoa도 ip주소를 받아오는데 해당 함수가 getpeername과 차이는?//현재 이 부분 에러 일으킴
		//recv 부분
		while (true) {
			char recvBuffer[PACKET_SIZE];
			int recvLen = recv(hClient, recvBuffer, PACKET_SIZE, 0);//그냥 원래의 코드 문자열을 전송받음 (정상 작동)
			//recv(hClient, (char*)&rinfo, sizeof(rinfo), 0);
			//username = ntohl(rinfo.name);
			//year = ntohl(rinfo.year);
			//int recvLen = recv(hClient, (char*)&rinfo, sizeof(rinfo), 0);//구조체 정보를 전송받음

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


					cout << "send Data: Len" << recvLen << endl;
					cout << "send Data :" << recvBuffer << endl;
					break;
				}



				/*istringstream archive_stream(recvBuffer);
				boost::archive::text_iarchive archive(archive_stream);

				archive >> rinfo;
				cout << "==========================================================================" << endl;
				cout << "rinfo name" << rinfo.name;
				cout << "rinfo message" << rinfo.content << endl;
				//cout << "send Data :" << rinfo.content << endl;
				//cout << "Data size : " << sizeof(rinfo) << endl;
				//cout << year<< endl;
				break;
			}
		}
	}

	WSACleanup();//소켓에서 사용하는 소멸자

	char cBuffer[PACKET_SIZE] = { 0 };

	//t1.join();

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();//소켓에서 사용하는 소멸자

	return 0;
}*/

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.