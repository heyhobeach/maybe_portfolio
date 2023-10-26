#pragma once
#include"pch.h"
#include<boost/bind.hpp>
#include<boost/asio.hpp>
#include<iostream>

#include"Protocol.h"
#include"ServerSession.h"

using namespace boost::asio;

class ChatServer {
public:
	ChatServer(io_context& Io_context) :m_acceptor(Io_context, ip::tcp::v4(), PORT_NUMBER){
		m_bIsAccepting = false;
	}
	~ChatServer() {
		for (size_t i = 0; i < m_SessionList.size(); ++i) {//i++가 아니라 ++i 인 이유
			if (m_SessionList[i]->Socket().is_open()) {
				m_SessionList[i]->Socket().close();//만약 socket이 열려있다면 socket을 닫아라
			}

			delete m_SessionList[i];//소켓 생성 되어있는거 삭제
		}
	}

	void Init(const int nMaxSessionCount) {
		for (int i = 0; i < nMaxSessionCount; ++i) {
			Session* pSession = new Session(i, (boost::asio::io_context&)m_acceptor.get_executor().context(), this);//여기가 static_cast가 안 되는 이유 
			m_SessionList.push_back(pSession);//Session을 vector에 저장해서 여러 클라이언트 관리
			m_SessionQueue.push_back(i);//index번호 저장
			//Session* pSession = new Session(i, static_cast<boost::asio::io_context>(m_acceptor.get_executor().context()), this);
		}
	}

	void Start() {
		std::cout << "서버 시작" << std::endl;

		PostAccept();
	}

	void closeSession(const int nSessionID) {
		std::cout << "클라이언트 접속 종료 세션 ID:" << nSessionID << std::endl;

		m_SessionList[nSessionID]->Socket().close();

		m_SessionQueue.push_back(nSessionID);//사용하지 않는 세션 ID는 다시 재사용하기 위해 Queue에 대입
		if (m_bIsAccepting == false) {
			PostAccept();//다시 접속 대기
		}
	}

	void ProcessPacket(const int nSessionID, const char* pData) {
		//PACKET_HEADER* pheader = static_cast<PACKET_HEADER*>(pData);
		PACKET_HEADER* pheader = (PACKET_HEADER*)pData;//구조체에 관한 형변환

		switch (pheader->nID)
		{
		case REQ_IN://접속 요청이 왔을때
			{
				PKT_REQ_IN* pPacket = (PKT_REQ_IN*)pData;
				m_SessionList[nSessionID]->SetName(pPacket->szName);
				std::cout << "클라이언트 로그인 성공 :" << m_SessionList[nSessionID]->GetName() << std::endl;
			  
				PKT_RES_IN SendPkt;
				SendPkt.Init();
				SendPkt.bIsSuccess = true;

				m_SessionList[nSessionID]->PostSend(false, SendPkt.nSize, (char*)&SendPkt);//데이터전송

			}
		break;
		case REQ_CHAT://채팅 요청이 왔을때
			{
				PKT_REQ_CHAT* pPacket = (PKT_REQ_CHAT*)pData;

				PKT_NOTICE_CHAT SendPkt;
				SendPkt.Init();
				strncpy_s(SendPkt.szName, MAX_NAME_LEN, m_SessionList[nSessionID]->GetName(), MAX_NAME_LEN - 1);
				strncpy_s(SendPkt.szMessage, MAX_MESSAGE_LEN, pPacket->szMessage, MAX_MESSAGE_LEN - 1);

				size_t nTotalSessionCount = m_SessionList.size();

				for (size_t i = 0; i < nTotalSessionCount; ++i) {
					if (m_SessionList[i]->Socket().is_open()) {
						m_SessionList[i]->PostSend(false, SendPkt.nSize, (char*)&SendPkt);
					}
				}
			}
			break;
		}
		return;
	}


private:

	bool PostAccept() {
		if (m_SessionQueue.empty()) {//안에 아무것도 없다면 클라이언트가 없다면?
			m_bIsAccepting = false;
			return false;

		}
		m_bIsAccepting = true;
		int nSessionID = m_SessionQueue.front();

		m_SessionQueue.pop_front();

		m_acceptor.async_accept(m_SessionList[nSessionID]->Socket(),boost::bind(&ChatServer::handle_accept,this,m_SessionList[nSessionID],boost::asio::placeholders::error)
		);
	}
	void handle_accept(Session* pSession, const boost::system::error_code& error){
		if (!error) {
			std::cout << "클라이언트 접속 성공" << pSession->SessionID() << std::endl;

			pSession->Init();
			pSession->PostReceive();

			PostAccept();
		}
		else
		{
			std::cout << "error No :" << error.value() << "error Message" << error.message() << std::endl;
		}
	}
	int m_nSeqNumber;
	bool m_bIsAccepting;
	boost::asio::ip::tcp::acceptor m_acceptor;

	std::vector<Session*> m_SessionList;
	std::deque<int> m_SessionQueue;

};