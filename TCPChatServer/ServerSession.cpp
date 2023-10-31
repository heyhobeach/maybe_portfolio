#include "pch.h"
#include "ServerSession.h"
#include"ChatServer.h"



Session::Session(int nSessionID, boost::asio::io_context& io_context, ChatServer* pServer) //줄 띄우는 이유 디버깅 하기 쉬우려고?
	:m_Socket(io_context), 
	m_nSessionID(nSessionID), 
	m_pServer(pServer) {
}


Session::~Session(){
	while (m_SendDataQueue.empty() == false) {//만약 큐 안에 데이터가 있다면
		delete[] m_SendDataQueue.front();//큐 앞부분 삭제
		m_SendDataQueue.pop_front();//메모리 삭제 delete와 pop_front차이?
	}
}



void Session::Init(){
	m_nPacketBufferMark = 0;//m_nPacketBufferMark초기화
}

void Session::PostReceive() {
	m_Socket.async_read_some(
		boost::asio::buffer(m_ReceiveBuffer), 
		boost::bind(&Session::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));//데이터 수신
}

void Session::PostSend(const bool bImmediately, const int nSize, char* pData){
	char* pSendData = nullptr;
	if (bImmediately == false) {//데이터를 복사하는 과정
		pSendData = new char[nSize];
		memcpy(pSendData, pData, nSize);//pSendData에 pData내용을 복사
		m_SendDataQueue.push_back(pSendData);//큐에 데이터 삽입
	}
	else {
		pSendData = pData;
	}

	if (bImmediately==false&&m_SendDataQueue.size()>1)//에러사항
	{
		return;
	}
	boost::asio::async_write(m_Socket, boost::asio::buffer(pSendData, nSize), boost::bind(&Session::handle_write,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));//복사한 데이터를 전송
}

void Session::handle_write(const boost::system::error_code& error, size_t bytes_transferred){
	delete[] m_SendDataQueue.front();
	m_SendDataQueue.pop_front();

	if (m_SendDataQueue.empty() == false) {
		char* pData = m_SendDataQueue.front();

		PACKET_HEADER* pHeader = (PACKET_HEADER*)pData;

		PostSend(true, pHeader->nSize, pData);
	}
}

void Session::handle_receive(const boost::system::error_code& error, size_t bytes_transferred){
	if (error) {
		if (error == boost::asio::error::eof) {
			std::cout << "클라이언트와의 연결 끊김" << std::endl;
		}
		else {
			std::cout << "error No :" << error.value() << "error Message : " << error.message() << std::endl;
		}
		m_pServer->closeSession(m_nSessionID);
	}
	else {
		memcpy(&m_PacketBuffer[m_nPacketBufferMark], m_ReceiveBuffer.data(), bytes_transferred);

		int nPacketData = m_nPacketBufferMark + bytes_transferred;//m_nPacketbufferMark의 위치를 계속 옮겨줌
		int nReadData = 0;

		while (nPacketData > 0) {
			if (nPacketData <sizeof(PACKET_HEADER)) {//패킷 사이즈가 헤더보다 작다면 -> 오류
				break;
			}

			PACKET_HEADER* pHeader = (PACKET_HEADER*)&m_PacketBuffer[nReadData];

			if (pHeader->nSize <= nPacketData) {
				m_pServer->ProcessPacket(m_nSessionID, &m_PacketBuffer[nReadData]);

				nPacketData -= pHeader->nSize;//읽은만큼 테이터 위치 줄임
				nReadData += pHeader->nSize;
			}
			else {
				break;
			}
		}

		if (nPacketData > 0) {
			char TempBuffer[MAX_RECEIVE_BUFFER_LEN ] = { 0, };
			memcpy(&TempBuffer[0], &m_PacketBuffer[nReadData], nPacketData);
			memcpy(&m_PacketBuffer[0], &TempBuffer[0], nPacketData);
		}

		m_nPacketBufferMark = nPacketData;

		PostReceive();

	}
}
