
/*int main() {
    try{//전송받은 데이터를 xml로 작성하는부분
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::acceptor acceptor(ioContext, ip::tcp::endpoint(ip::tcp::v4(), 12345));
        boost::asio::ip::tcp::socket socket(ioContext);

        //acceptor.accept(socket);
        acceptor.async_accept(socket);

        boost::system::error_code ec;

        cout << "client connect" << endl;
        //string receivedData;
        while (true) {
            uint32_t dataLength = 0;
            boost::asio::read(socket, boost::asio::buffer(&dataLength, sizeof(dataLength)));
            vector<char>receivedData(dataLength);

            boost::asio::read(socket, boost::asio::buffer(receivedData));
            //boost::asio::read(socket, boost:: asio::dynamic_buffer(receivedData),ec);//dynamic_buffer와 일반 buffer의 차이
            if (ec) {
                std::cerr << "Error reading dat: " << ec.message() << endl;
            }
            else {


                std::istringstream archiveStream(string(receivedData.begin(), receivedData.end()));
                boost::archive::binary_iarchive archive(archiveStream);//eof에러 발생

                MyStruct receivedStruct;
                archive >> receivedStruct;

                {
                    cout << "intValue" << receivedStruct.intValue << endl;
                    cout << "stringValue" << receivedStruct.stringValue << endl;
                    for (int i = 0; i < receivedStruct.doubleVector.size(); i++) {
                        cout << "index" << i << ": " << receivedStruct.doubleVector[i] << endl;
                    }
                }

                boost::property_tree::ptree pt;
                pt.put("MyStruct.intValue", receivedStruct.intValue);
                pt.put("MyStruct.stringValue", receivedStruct.stringValue);

                int index = 0;
                for (const auto& value : receivedStruct.doubleVector) {
                    string key = "MyStruct.doubleVecotr.value" + to_string(index);// 벡터 번호
                    pt.put(key, value);
                    index++;
                }

                boost::property_tree::xml_parser::write_xml("received_data.xml", pt);
            }
            
            
        }
        socket.close();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << endl;
    }


}
*/

/////////////////////////

/*
class Server {//1:n구현하다가 못 한 부분
public:
    Server(io_context& ioContext, short port) : acceptor_(ioContext, ip::tcp::endpoint(ip::tcp::v4(), port)) {
        start_accept();
   
    }
private:
    ip::tcp::acceptor acceptor_;
    vector<shared_ptr<ip::tcp::socket>> clients_;//2차원 배열
    vector<char> receive_buffer_;

    void start_accept() {//context = 어떤 객체를 핸들링 하기 위한 접근 수단이라는 의미
        shared_ptr<ip::tcp::socket> client_socket = make_shared<ip::tcp::socket>(acceptor_.get_executor());//shared_ptr형식의 포인터를 만드는데 안에 인자로는 acceptor의 실행기의 context를 인자로 사용함
        acceptor_.async_accept(*client_socket, [this, client_socket](const boost::system::error_code& ec) {
            if (!ec) {
                cout << "Client connected:" << client_socket->remote_endpoint() << endl;
                clients_.push_back(client_socket);
                start_receive(client_socket);
            }
            else {
                cerr << "Accept error: " << ec.message() << endl;
            }
            start_accept();//무한 루프로 이 함수 계속 호출
        });

    }

    void start_receive(shared_ptr<ip::tcp::socket> client_socket) {
        client_socket->async_receive(buffer(receive_buffer_), [this, client_socket](const boost::system::error_code ec, size_t byte_received){
            if (!ec) {
                //데이터를 받을곳
                
                //cout << receive_buffer_[0] << endl;  //출력하는 부분인데 에러 발생
                //for (auto i : receive_buffer_) {
                //    cout <<"data : " << i << endl;
                //}
                //cout << "Received: " << string(receive_buffer_.begin(), receive_buffer_.begin() + byte_received) << endl;
                //cout << "Received " << byte_received << " bytes of data." << endl;

                process_received_data(client_socket, receive_buffer_, byte_received);
                start_receive(client_socket);
            }
            else {
                //데이터를 못 받는다 = 클라이언트 연결 끊어짐
                cerr << "Client disconnected: " << client_socket->remote_endpoint() << endl;
                remove_client(client_socket);
            }
        }); 
    }

    void process_received_data(shared_ptr<ip::tcp::socket> client_socket, const vector<char>& data, size_t length) {
        //Sleep(500);//너무 빨리 떠서 로그 확인용
        string received_data(data.begin(), data.begin() + length);

        cout<<"received from"<<client_socket->remote_endpoint()<<": "<<received_data<<endl;
        for (const auto& other_client : clients_) {
            if (other_client != client_socket) {//나를 제외한 다른 모든 클라이언트에게 메시지를 전송하기 위한 if문
                write(*other_client, buffer(data, length));
            }
        }
        //cout << "Received: " << string(data.begin(), data.begin() + length) << endl;
    }

    void remove_client(shared_ptr<ip::tcp::socket> client_socket) {
        clients_.erase(remove(clients_.begin(), clients_.end(), client_socket),clients_.end());
    }
};

int main(void) {
    try {
        io_context ioContext;
        Server server(ioContext, 12345);

        ioContext.run();//비동기 루프 실행
    }
    catch (exception& e) {
        cerr << "Exception :" << e.what() << endl;
    }
    return 0;
}*/
