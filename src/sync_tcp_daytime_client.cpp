/**
 * @file sync_tcp_daytime_client.cpp
 * @author your name (you@domain.com)
 * @brief Daytime.1 - A synchronous TCP daytime client
 *        (https://think-async.com/Asio/asio-1.20.0/doc/asio/tutorial/tutdaytime1.html) 
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <array>
#include <exception>
#include <iostream>

#include <asio.hpp>

using asio::ip::tcp;

//* daytime 서비스에 접근하기 위해서 접속 서버를 지정할 수 있어야 한다.
int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }
        asio::io_context io;

        //* 인자로 지정된 서버 이름을 tcp 엔드 포인트로 변경해야 한다. 이를 위해 resolver를 사용하낟.
        tcp::resolver resolver(io);

        //* resolve 함수는 호스트 이름과 서비스 이름을 인자로 입력받아 endpoint의 리스토로 변환한다.
        //* 반환된 endpoints는 begin(), end() 함수를 통해 range로 접근할 수 있다.
        tcp::resolver::results_type endpoints =
            resolver.resolve(argv[1], "daytime");

        //* 소켓을 생성하고 endpoint의 리스트에 연결한다. 연결되기 전까지 대기한다?
        tcp::socket socket(io);
        asio::connect(socket, endpoints);
        // 해당 함수는 endpoint 리스트에 있는 ipv4, ipv6 엔드포인트에 대해서 동작하는 것을 찾아 연결한다.

        //* 연결이 되면 데이터를 수신한다.
        for (;;) {
            // std::vector 혹은 char []를 사용할 수도 있다.
            std::array<char, 128> buf;
            asio::error_code error;

            //* asio::buffer 함수는 자동으로 배열의 사이즈를 계산하여 버퍼 오버런을 막는다.
            size_t len = socket.read_some(asio::buffer(buf), error);

            //* 서버가 연결을 종료하면, asio::read_some 함수는 asio::error::eof를 반환한다.
            if (error == asio::error::eof) {
                break;  // connection closed cleanly by peer
            }
            if (error) {
                throw asio::system_error(error);  //some other error
            }
            //* formatted data는 <<으로, unformatted data는 write로 출력할 수 있다.
            std::cout.write(buf.data(), len);
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}