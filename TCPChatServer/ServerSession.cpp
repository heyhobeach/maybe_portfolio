#include "pch.h"
#include "ServerSession.h"
#include"ChatServer.h"



Session::Session(int nSessionID, boost::asio::io_context& io_context, ChatServer* pServer) :m_Socket(io_context), m_nSessionID(nSessionID), m_pServer(pServer) {}


Session::~Session(){}



void Session::Init(){}

void Session::PostReceive() {}

void Session::PostSend(const bool bImmediately, const int nSize, char* pData){
	char* pSendData = nullptr;
	if (bImmediately == false) {
		pSendData = new char[nSize];
		memcpy(pSendData, pData, nSize);//pSendData에 pData내용을 복사
		m_SendDataQueue.push_back(pSendData);//큐에 데이터 삽입
	}
	else {
		pSendData = pData;
	}

	if (bImmediately==false&&m_SendDataQueue.size()>1)
	{
		return;
	}
	boost::asio::async_write(m_Socket, boost::asio::buffer(pSendData, nSize), boost::bind(&Session::handle_write,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
}

void Session::handle_write(const boost::system::error_code& error, size_t bytes_transferred){}

void Session::handle_receive(const boost::system::error_code& error, size_t bytes_transferred){}