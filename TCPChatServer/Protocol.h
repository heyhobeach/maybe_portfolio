#pragma once

//패킷에 관한 설정

const unsigned short PORT_NUMBER = 31400;

const int MAX_RECEIVE_BUFFER_LEN = 512;

const int MAX_NAME_LEN = 17;
const int MAX_MESSAGE_LEN=129;

struct PACKET_HEADER {
	short nID;
	short nSize;
};

// 패킷
const short REQ_IN = 1;

const short RES_IN = 2;

const short REQ_CHAT = 6;

const short NOTICE_CHAT = 7;

struct PKT_REQ_IN :public PACKET_HEADER {//상속, 요청을 받았을때 PACKET_HEADER의 정보를 상황에 맞게 수정함) 접속요청이 왔을때
	void Init() {
		nID = REQ_IN;
		nSize = sizeof(PKT_REQ_IN);
		memset(szName, 0, MAX_NAME_LEN);
	}
	char szName[MAX_NAME_LEN];
};

struct PKT_RES_IN :public PACKET_HEADER {//상속, 요청을 받았을때 PACKET_HEADER의 정보를 상황에 맞게 수정함) 응답할때
	void Init() {
		nID = RES_IN;
		nSize = sizeof(PKT_RES_IN);
		bIsSuccess = false;
	}
	bool bIsSuccess;
};

struct PKT_REQ_CHAT :public PACKET_HEADER {//상속, 요청을 받았을때 PACKET_HEADER의 정보를 상황에 맞게 수정함) CHAT에대한 접속요청이 왔을때
	void Init() {
		nID = REQ_CHAT;
		nSize = sizeof(PKT_REQ_CHAT);
		memset(szMessage, 0, MAX_MESSAGE_LEN);
	}
	char szMessage[MAX_MESSAGE_LEN];
};

struct PKT_NOTICE_CHAT :public PACKET_HEADER {//상속, 요청을 받았을때 PACKET_HEADER의 정보를 상황에 맞게 수정함) chat에 대한 정보
	void Init() {
		nID = NOTICE_CHAT;
		nSize = sizeof(PKT_NOTICE_CHAT);
		memset(szName, 0, MAX_NAME_LEN);
		memset(szMessage, 0, MAX_MESSAGE_LEN);
	}
	char szName[MAX_NAME_LEN];
	char szMessage[MAX_MESSAGE_LEN];
};