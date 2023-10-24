#pragma once

#include <deque>

#include<boost/bind.hpp>
#include<boost/asio.hpp>

#include "Protocol.h"
class ChatServer;

class Session {
public:
	Session(int nSessionID, boost::asio::io_context& io_context, ChatServer* pServer);
	~Session();

	int SessionID() { return m_nSessionID; }//Session의 ID를 가져오는 함수

	boost::asio::ip::tcp::socket& Socket() { return m_Socket; }

	void Init();

	void PostReceive();

	void PostSend(const bool bImmediately, const int nSize, char* pData);//데이터 전송함수

	void SetName(const char* pszName) { m_Name = pszName; }//이름 설정함수
	const char* GetName() { return m_Name.c_str(); }//이름 가져오는 함수

	
private:
	void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);

	int m_nSessionID;
	boost::asio::ip::tcp::socket m_Socket;

	std::array<char, MAX_RECEIVE_BUFFER_LEN> m_ReceiveBuffer;

	int m_nPacketBufferMark;
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];

	std::deque<char*>m_SendDataQueue;

	std::string m_Name;

	ChatServer* m_pServer;
};