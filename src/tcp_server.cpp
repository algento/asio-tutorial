/**
 * @file tcp_server.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>

#include <asio.hpp>
#include <asio/error_code.hpp>
#include <asio/io_context.hpp>
#include <asio/streambuf.hpp>
#include <asio/system_error.hpp>

std::string read(asio::ip::tcp::socket& socket) {
    asio::streambuf buffer;
    asio::read_until(socket, buffer, "\n");
    return asio::buffer_cast<const char*>(buffer.data());
}

void send(asio::ip::tcp::socket& socket, const std::string& message) {
    const std::string msg = message + "\n";
    asio::write(socket, asio::buffer(message));
}

int main() {
    asio::io_context io_context;
    // 새 연결 listen
    asio::ip::tcp::acceptor acceptor(
        io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1234));
    // 소켓 생성
    asio::ip::tcp::socket socket(io_context);
    // 소켓 연결
    acceptor.accept(socket);
    // 읽기 수행
    std::string message = read(socket);
    std::cout << message << std::endl;
    // 쓰기 수행
    send(socket, "Hello from Server");
    std::cout << "Server sent message! " << std::endl;
    return 0;
}