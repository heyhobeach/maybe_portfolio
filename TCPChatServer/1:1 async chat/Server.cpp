#include"pch.h"

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include<sstream>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>
#include<boost/serialization/vector.hpp>
#include<thread>
//일단은 1:1로 만들어보기
using namespace boost::asio;
using namespace std;
struct MyStruct {
    int intValue;
    string stringValue;
    vector<double> doubleVector;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned intversion) {
        ar& intValue;
        ar& stringValue;
        ar& doubleVector;
    }
};


class Server {//현재 1:1 단방향 프로그래밍
public:
    Server(io_context& ioContext) :m_Socket(ioContext) {}//이후 io_context로 변경
    ip::tcp::socket& Socket() {
        return m_Socket;
    }

    void PostReceive() {
        memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));
        m_Socket.async_read_some(buffer(m_ReceiveBuffer), 
            bind(&Server::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }
private:
    void handle_write(const boost::system::error_code error,size_t bytes_transferred){

    }
    void handle_receive(const boost::system::error_code error, size_t bytes_transferred){
        if (error) {
            if (error == boost::asio::error::eof) {
                cout << "클라이언트와 연결이 끊어졌습니다" << endl;
            }
            else {
                cerr << "Error :" << error.value()<<"Error message:"<<error.message() << endl;
            }
        }
        else {//성공적으로 receive했을때
            string receiveMessage = m_ReceiveBuffer.data();
            cout << "클라이언트 :" << receiveMessage << ", 받은 크기 :" << bytes_transferred << endl;

            char messageSize[128];
            sprintf_s(messageSize, sizeof(messageSize)-1, "RE:%s", receiveMessage.c_str());//sprintf_s의 역할?
            m_WriteMessage = messageSize;

            cout << "messageSize" << messageSize << endl;


            cout << "m_WriteMessaage" << m_WriteMessage << endl;

            async_write(m_Socket, buffer(m_WriteMessage), bind(&Server::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

            PostReceive();//Server::PostReceive
        }
    }
    ip::tcp::socket m_Socket;   
    string m_WriteMessage;
    array<char, 128> m_ReceiveBuffer;
};

const unsigned short PORT_NUMBER = 31400;//port번호 설정

class TCP_Server {
public:
    TCP_Server(io_context& ioContext) :m_io_context(ioContext), m_acceptor(ioContext, ip::tcp::endpoint(ip::tcp::v4(), PORT_NUMBER)) {
        m_pServer = nullptr;
        StartAccept();
    }
    ~TCP_Server() {
        if (m_pServer != nullptr) {
            delete m_pServer;
        }
    }
private:
    void StartAccept() {
        cout << "클라이언트 접속 대기...." << endl;
        cout << "acceptor  " << typeid(m_io_context).name() << ",  m_acceptor  " << typeid(m_acceptor.get_executor().context()).name() << endl;

        io_context io_context;
        //io_context& context = (&m_acceptor.get_executor().context();a~!M,LK]=
        //|><?l+:y?+:?|>
        //boost::asio::io_context io_context{ m_acceptor.get_executor()};
        m_pServer = new Server(io_context);


        //m_pServer = new Server(boost::asio::io_context{ m_acceptor.get_executor().context() });//m_acceptor.get_executor().context()
        m_acceptor.async_accept(m_pServer->Socket(), bind(&TCP_Server::handle_accept, this, m_pServer, boost::asio::placeholders::error));
    }
    void handle_accept(Server* pServer,const boost::system::error_code &error){
        if (!error) {
            cout << "클라이언트 접속 성공" << endl;

            pServer->PostReceive();
        }
    }

    int m_nSeqNumber;
    ip::tcp::acceptor m_acceptor;
    io_context& m_io_context;
    //io_service& m_io_service;
    Server* m_pServer;
};

int main() {

    io_context io_service;
    TCP_Server server(io_service);
    io_service.run();

    cout << "네트워크 접속 종료" << endl;
    //getchar();//이게 왜 있는지 모르겠음
    return 0;
}
