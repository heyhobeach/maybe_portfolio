//client
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
#include <boost/asio.hpp>
#include<boost/archive/binary_oarchive.hpp>
#include<boost/serialization/vector.hpp>


#define PORT	12345// 예약된 포트를 제외하고 사용해야함  (ex) 21 : FTP포트, 80 : HTTP포트, 8080 : HTTPS포트)
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.219.106"// 서버의 ip로 맞춰줘야함//"192.168.219.100"

#pragma once

using namespace std;
using namespace std::chrono;
using namespace boost::asio;

struct Info {//server와 client두개가 같아야함
    string name;
    string content;
    string teststr;
    int year;
    int month;
    int day;
    vector<double>doubleVector;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& name;
        ar& content;
        ar& teststr;
        ar& year;
        ar& month;
        ar& day;
        ar& doubleVector;
    }
};

void setInfo(Info* info, string name, string content, int year, int month, int day, vector<double> doubleVector = { 3.14,5.2,1.2 }) {
    info->name = name;
    info->content = content;
    info->year = year;
    info->month = month;
    info->day = day;
    info->doubleVector = doubleVector;
}

void printInfo(Info* info) {
    cout << "name :" << info->name << endl;
    cout << "content :" << info->content << endl;
    cout << "year :" << info->year << endl;
    cout << "month :" << info->month << endl;
    cout << "day :" << info->day << endl;
    cout << "vector" << endl;
    for (auto i : info->doubleVector) {
        cout << i << " ";
    }cout << endl;
}


void send_input(string name, int& num, SOCKET& socket) {//cin에서 메세지 보낼걸 굳이 포인터로 접근해야할까?
    char buff[PACKET_SIZE] = { 0 };
    string message = "";
    string strNum;
    const char* cmessage;
    Info userInfo;
    int infosize = 0;
    unsigned char InfoBuffer[sizeof(Info)];
    while (!WSAGetLastError()) {
        getline(cin, message);//공백 포함 입력 받는 과정 string 라이브러리에 들어있음
        const local_time<system_clock::duration> local_now = zoned_time{ current_zone(), system_clock::now() }.get_local_time();//로컬 시간
        const time_point<std::chrono::local_t, std::chrono::days> dp = std::chrono::floor<std::chrono::days>(local_now);//pratice 프로젝트에서 확인

        std::chrono::year_month_day ymd{dp};
        std::chrono::hh_mm_ss time{std::chrono::floor< std::chrono::seconds>(local_now - dp)};



        string s = format("{:%Y년 %m월 %d일}", local_now);
        string sec = format("{:%H: %M :%S}", time);
        setInfo(&userInfo, name, message, static_cast<int>(ymd.year()), static_cast<unsigned int>(ymd.month()), static_cast<unsigned int>(ymd.day()));//구조체에 현재 사용자 이름, 내용, 년, 월, 일 입력
        memcpy(InfoBuffer, &userInfo, sizeof(Info));//InfoBuffer에 userInfo 메모리를 복사 하는 과정
        printInfo(&userInfo);// 구조체에 메세지 들어갔는지 확인하는 print문
        cout << sec << endl;
        message = s + sec + "//  사용자>>" + name + message;
        cmessage = message.c_str();

        cout << "전송 메세지 내용 : " << "\"" << cmessage << "\"" << endl;
        cout << "data size :" << infosize << endl;
        cout << "content size:" << sizeof(userInfo.content) << endl;

    }
}


int main()
{
    try {
        io_context ioContext;//io_context객체 생성 해서 비동기 i/o 작업 관리, io_service와 거의 유사한 작업 수행
        ip::tcp::socket socket(ioContext);//클라이언트 소켓을 생성
        socket.connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), PORT));//endpoint 네트워크 주소 설정, 이 주소로 클라이언트가 접속


        Info sendInfoData;
        setInfo(&sendInfoData, "jungmu", "hello jungmu", 2023, 9, 23);

        std::ostringstream archiveStream;//직렬화변수
        boost::archive::binary_oarchive archive(archiveStream);//얘네 아마 클래스? 함수?
        archive << sendInfoData;//직렬화 할 데이터를 archive에 담는다

        string serializedData = archiveStream.str();//string으로 변환
        boost::asio::write(socket, boost::asio::buffer(serializedData));
        boost::system::error_code error;

        socket.close();
        //size_t length=socket.read_some()

    }
    catch (std::exception& e) {
        cerr << "Exception: " << e.what() << endl;//cerr란?
    }
}
