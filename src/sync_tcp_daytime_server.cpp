/**
 * @file sync_tcp_daytime_server.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief Daytime.2 - A synchronous TCP daytime server
 *        (https://think-async.com/Asio/asio-1.20.0/doc/asio/tutorial/tutdaytime2.html) 
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ctime>
#include <iostream>
#include <string>

#include <asio.hpp>

using asio::ip::tcp;

//* 클라이언트로 보낼 문자열을 생성하는 함수 (현재 시간을 스트링으로 반환한다.)
std::string make_daytime_string() {
    time_t now = time(0);
    return ctime(&now);
}

//* 동기식 TCP 서버를 어떻게 만드는지 보여준다.
int main() {
    try {
        asio::io_context io;

        //* 새 연결을 listen하기 위한 acceptor이다. 포트 13번, ipv4로 초기화되었다.
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));
        for (;;) {
            //* 순차 처리를 반복하는 서버로써, 한 번에 하나의 연결만 처리한다.
            //* 클라이언트 연결에 대한 소켓을 생성하고 연결이 될 때까지 대기한다.
            tcp::socket socket(io);
            acceptor.accept(socket);

            //* 클라이언트가 연결되면 현재 시간에 대한 스트링을 생성해서 클라이언트에게 전송한다.
            std::string message = make_daytime_string();
            asio::error_code ignored_error;
            asio::write(socket, asio::buffer(message), ignored_error);
            std::cout << "Sent message to client" << std::endl;
        }
    } catch (std::exception& e) {
        //* 예외처리
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
