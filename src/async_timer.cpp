/**
 * @file async_timer.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief asio C++ library/Tutorial/Basic Skills/Timer.2 - Using a timer asynchronously
 *        (https://think-async.com/Asio/asio-1.20.0/doc/asio/tutorial/tuttimer2.html)
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>

#include <asio.hpp>

//* asio async 기능을 사용한다는 것은 비동기 작업 완료 후 호출되는 콜백 함수를 갖는다는 의미이다.
void print(const asio::error_code& /*e*/) {
    std::cout << "Hello, asynchronous world!" << std::endl;
}

int main() {
    asio::io_context io;
    asio::steady_timer t{io, asio::chrono::seconds(5)};

    //* 비동기 대기가 끝나면 print라는 함수를 콜백으로 호출한다.
    t.async_wait(&print);

    //* asio는 run을 호출한 스레드에서만 콜백 핸들러가 호출되도록 보장한다.
    //* 따라서 run()함수를 호출하지 않으면, 콜백함수는 호출되지 않는다.
    //* 이 함수는 할 작업이 있는 동안, 계속 실행된다.
    //* 여기서는 5초동안의 비동기 대기가 해당 작업이다. 따라서 타이머가 완료되어 콜백함수가 완료되기 전까지 리턴하지 않는다.
    //* 따라서 run() 함수를 호출하기 전에 io_context에 작업을 할당하는 것이 중요하다.
    io.run();

    return 0;
}