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
		for (size_t i = 0; i < m_SessionList.size(); ++i) {//i++�� �ƴ϶� ++i �� ����
			if (m_SessionList[i]->Socket().is_open()) {
				m_SessionList[i]->Socket().close();//���� socket�� �����ִٸ� socket�� �ݾƶ�
			}

			delete m_SessionList[i];//���� ���� �Ǿ��ִ°� ����
		}
	}

	void Init(const int nMaxSessionCount) {
		for (int i = 0; i < nMaxSessionCount; ++i) {
			Session* pSession = new Session(i, (boost::asio::io_context&)m_acceptor.get_executor().context(), this);//���Ⱑ static_cast�� �� �Ǵ� ���� 
			m_SessionList.push_back(pSession);//Session�� vector�� �����ؼ� ���� Ŭ���̾�Ʈ ����
			m_SessionQueue.push_back(i);//index��ȣ ����
			//Session* pSession = new Session(i, static_cast<boost::asio::io_context>(m_acceptor.get_executor().context()), this);
		}
	}

	void Start() {
		std::cout << "���� ����" << std::endl;

		PostAccept();
	}

	void closeSession(const int nSessionID) {
		std::cout << "Ŭ���̾�Ʈ ���� ���� ���� ID:" << nSessionID << std::endl;

		m_SessionList[nSessionID]->Socket().close();

		m_SessionQueue.push_back(nSessionID);//������� �ʴ� ���� ID�� �ٽ� �����ϱ� ���� Queue�� ����
		if (m_bIsAccepting == false) {
			PostAccept();//�ٽ� ���� ���
		}
	}

	void ProcessPacket(const int nSessionID, const char* pData) {
		//PACKET_HEADER* pheader = static_cast<PACKET_HEADER*>(pData);
		PACKET_HEADER* pheader = (PACKET_HEADER*)pData;//����ü�� ���� ����ȯ

		switch (pheader->nID)
		{
		case REQ_IN://���� ��û�� ������
			{
				PKT_REQ_IN* pPacket = (PKT_REQ_IN*)pData;
				m_SessionList[nSessionID]->SetName(pPacket->szName);
				std::cout << "Ŭ���̾�Ʈ �α��� ���� :" << m_SessionList[nSessionID]->GetName() << std::endl;
			  
				PKT_RES_IN SendPkt;
				SendPkt.Init();
				SendPkt.bIsSuccess = true;

				m_SessionList[nSessionID]->PostSend(false, SendPkt.nSize, (char*)&SendPkt);//����������

			}
		break;
		case REQ_CHAT://ä�� ��û�� ������
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
		if (m_SessionQueue.empty()) {//�ȿ� �ƹ��͵� ���ٸ� Ŭ���̾�Ʈ�� ���ٸ�?
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
			std::cout << "Ŭ���̾�Ʈ ���� ����" << pSession->SessionID() << std::endl;

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