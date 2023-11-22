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



/*class chatClient {
public:
	chatClient(boost::asio::io_context& io_context) :Io_context(io_context) {
		m_bIsLogin = false;
	}


private:
	boost::asio::io_context& Io_context;
	boost::asio::ip::tcp::socket m_Socket;

	bool m_bIsLogin;

};*/
int main(void) {


	//std::cout << st.arr << std::endl;
	try {
		boost::asio::io_context io_context;

		// ������ ����
		boost::asio::ip::tcp::socket socket(io_context);
		socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));

		// ������ ����
		MyStruct st{ 1,1.2,"������" };
		while (true) {
			// ������ ����ȭ
			std::ostringstream archive_stream;
			boost::archive::text_oarchive archive(archive_stream);
			archive << st;
			string temp = "";
			// ����ȭ�� �����͸� ������ ���� ����
			std::string serialized_data = archive_stream.str();
			getline(std::cin, temp);
			//std::cin >> temp;//�ش� �κ� ���� �ʿ�
			if (temp == "!img") {//�Ѿ���� �ϴµ� ���� �����͸� ������ ���� �׷��� ���� ���� �Ǵ� �κ�
				std::cout << "input !img - image send" << std::endl;
				std::ifstream imageFile("20180530_155458.jpg", std::ios::binary);
				if (!imageFile) {
					std::cerr << "Error opening file" << std::endl;
					return 1;
				}
				boost::system::error_code error;
				uintmax_t fileSize = boost::filesystem::file_size("20180530_155458.jpg");
				std::cout << fileSize << std::endl;
				boost::asio::write(socket, boost::asio::buffer(&fileSize, sizeof(fileSize)), error);//���� ũ�� ����
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