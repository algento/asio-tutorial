/**
 * @file sync_tcp_daytime_server.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief Daytime.3 - A asynchronous TCP daytime server
 *        (https://think-async.com/Asio/asio-1.20.0/doc/asio/tutorial/tutdaytime3.html) 
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <asio.hpp>

using asio::ip::tcp;

//* 클라이언트로 보낼 문자열을 생성하는 함수 (현재 시간을 스트링으로 반환한다.)
std::string make_daytime_string() {
    time_t now = time(0);
    return ctime(&now);
}

//* 해당 클래스 인스턴스를 참조하는 작업이 있는 경우, 살아 있도록 shared_ptr과 enable_shared_from_this를 사용한다.
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
    using Ptr = std::shared_ptr<TcpConnection>;

    static Ptr create(asio::io_context& io) {
        // return std::make_shared<TcpConnection>(io);
        return Ptr(new TcpConnection(io));
    }

    tcp::socket& socket() { return socket_; }

    //* 클라이언트에 데이터를 전달하는 함수
    //* 전체 데이터 블록이 보내질 수 있도록 async_write_some 대신 async_write을 사용하였다.
    //* 핸들러로는 handle_write 함수를 지정하였다. 클라이언트 연결에 대한 추가작업은 handle_write의 책임이다.
    void start() {
        message_ = make_daytime_string();
        asio::async_write(
            socket_, asio::buffer(message_),
            std::bind(&TcpConnection::handle_write, shared_from_this(),
                      std::placeholders::_1, std::placeholders::_2));
    }

 private:
    TcpConnection(asio::io_context& io) : socket_(io) {}

    void handle_write(const asio::error_code& /* error */,
                      size_t /* bytes_transferred */) {
        std::cout << "handle_write" << std::endl;
    }

    tcp::socket socket_;
    std::string message_;
};

//* 클라이언트 연결을 처리할 서버 객체가 필요하다.
class TcpServer {
 public:
    explicit TcpServer(asio::io_context& io)
        : io_context_(io), acceptor_(io, tcp::endpoint(tcp::v4(), 13)) {
        start_accept();
    }

 private:
    //* start_accept 함수는 소켓을 생성하고 비동기 승인 작업을 시작하여 새로운 연결을 기다린다.
    void start_accept() {
        //* 새 연결을 만든다. (소켓 생성)
        TcpConnection::Ptr new_connection = TcpConnection::create(io_context_);

        //* 비동기 승인 작업을 시작 (새로운 연결 대기)
        //* 승인될 경우에 수행할 handle_accept을 등록한다.
        acceptor_.async_accept(
            new_connection->socket(),
            std::bind(&TcpServer::handle_accept, this, new_connection,
                      std::placeholders::_1));
    }
    void handle_accept(TcpConnection::Ptr new_connection,
                       const asio::error_code& error) {
        std::cout << "handle_accept" << std::endl;
        //* 오류가 없다면, 연결된 클라이언트에 대한 요청을 처리한다.
        if (!error) {
            new_connection->start();
        }
        //* 클라이언트 요청을 처리한 후, 다음 승인 작업을 시작한다.
        start_accept();
    }
    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

int main() {
    try {
        asio::io_context io_context;
        TcpServer server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
