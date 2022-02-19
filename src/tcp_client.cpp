/**
 * @file tcp_client.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief from https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
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

int main() {
    asio::io_context io_context;
    // 소켓 생성
    asio::ip::tcp::socket socket(io_context);
    // 소켓 연결
    socket.connect(asio::ip::tcp::endpoint(
        asio::ip::address::from_string("127.0.0.1"), 1234));
    // 클라이언트에서 메세지 요청
    const std::string msg = "Hello from Client\n";
    asio::error_code error;
    asio::write(socket, asio::buffer(msg), error);

    if (!error) {
        std::cout << "Client sent hello message!" << std::endl;
    } else {
        std::cout << "Sent failed: " << error.message() << std::endl;
    }

    // 서버에서 reponse 받기
    asio::streambuf receive_buffer;
    asio::read(socket, receive_buffer, asio::transfer_all(), error);
    if (error && error != asio::error::eof) {
        std::cout << "Receive failed " << error.message() << std::endl;
    } else {
        const char* data =
            asio::buffer_cast<const char*>(receive_buffer.data());
        std::cout << data << std::endl;
    }

    return 0;
}