#include "pch.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/filesystem.hpp>
#include<algorithm>


using namespace std;

struct MyStruct {
	int data1;
	double data2;
	char data3[100];
	int arr[10] = { 1,2,3 ,NULL };
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& data1;
		ar& data2;
		ar& data3;
		ar& arr;
	}

};



class chatClient {//클래스로 소켓이나 이런거 관리하면 메인문이 아닌 밖에서 모듈로 관리 할때 내가 socket같은 정보를 받아올때 다른 함수 호출시 막 socket정보 넘겨주고 그럴 필요없이 클래스 내에서 사용하면 된다
public:
	chatClient(boost::asio::io_context& io_context) :Io_context(io_context),m_Socket(io_context) {//기본 생성자 모양때문에 두개인듯?
		m_bIsLogin = false;
	}
	void Connect(boost::asio::ip::tcp::endpoint endpoint) {
		m_Socket.connect(endpoint);
	}

	void sendMessage() {
		getline(cin, message);
		boost::asio::write(m_Socket, boost::asio::buffer(message + '\n',sizeof(message)+sizeof('\n')));//buffer의 형태에 대해서 아래는 error와 이미지 사이즈, buffer전부 들어가는데 여기는 메시지 내용만 있음 굳이 error코드를 안 넣을 필요가 있을까
	}

	void sendImage(string file_name) {
		std::ifstream imageFile(file_name, std::ios::binary);
		if (!imageFile) {
			//std::cout << "error opening file" << std::endl;//cerr와 cout의 차이는?
			std::cerr << "error opening file" << std::endl;
		}
		else {//파일이 제대로 열렸을경우
			boost::system::error_code error;
			uintmax_t filesize = boost::filesystem::file_size(file_name);
			std::cout << filesize << std::endl;
			boost::asio::write(m_Socket, boost::asio::buffer(&filesize, sizeof(filesize)), error);//boost buffer에 대해 다시 알 필요있음
			char buffer[buffersize];

			while (!imageFile.eof()) {
				imageFile.read(buffer, buffersize);//buffer
				boost::asio::write(m_Socket, boost::asio::buffer(buffer, imageFile.gcount()), error);//gcount란? 입력받은 문자의 수를 가지고옴 현재 imageFile은 1024로 받고 있기에 gcount는 1024가 된다
			}

		}
	}


private:
	boost::asio::io_context& Io_context;
	boost::asio::ip::tcp::socket m_Socket;

	std::string message = "";

	bool m_bIsLogin;

	static const size_t buffersize = 1024;

};
int main(void) {


	//std::cout << st.arr << std::endl;
	
	try {
		boost::asio::io_context io_context;
		chatClient client(io_context);//클래스로 연결 부분
		// 서버에 연결
		//boost::asio::ip::tcp::socket socket(io_context);

		boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345);
		//auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 12345);// 위와 아래 라인의 차이에 대해서
		client.Connect(endpoint);

		// 데이터 생성
		MyStruct st{ 1,1.2,"김정무" };
		while (true) {
			// 데이터 직렬화
			std::ostringstream archive_stream;
			boost::archive::text_oarchive archive(archive_stream);
			archive << st;
			string temp = "";
			// 직렬화된 데이터를 소켓을 통해 전송
			std::string serialized_data = archive_stream.str();
			getline(std::cin, temp);
			//std::cin >> temp;//해당 부분 수정 필요
			if (temp == "!img") {//넘어오고 하는데 현재 데이터를 받지를 못함 그래서 강제 종료 되는 부분
				std::cout << "input !img - image send" << std::endl;
				std::ifstream imageFile("20180530_155458.jpg", std::ios::binary);//binary형태 즉 2진 데이터로 데이터를 받아온다는것
				if (!imageFile) {
					std::cerr << "Error opening file" << std::endl;
					return 1;
				}
				boost::system::error_code error;
				uintmax_t fileSize = boost::filesystem::file_size("20180530_155458.jpg");
				std::cout << fileSize << std::endl;
				boost::asio::write(socket, boost::asio::buffer(&fileSize, sizeof(fileSize)), error);//파일 크기 전송
				//std::ifstream imageFile("", std::ios::binary);
				const size_t bufferSize = 1024;
				char buffer[bufferSize];

				while (!imageFile.eof()) {
					imageFile.read(buffer, bufferSize);
					boost::asio::write(socket, boost::asio::buffer(buffer, imageFile.gcount()), error);
				}
			}
			

			st.data1++;

			//boost::asio::write(socket, boost::asio::buffer(serialized_data + '\0'));

			//boost::asio::write(socket, boost::asio::buffer(temp + '\0'));

			std::cout << "Struct sent successfully." << std::endl;
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
