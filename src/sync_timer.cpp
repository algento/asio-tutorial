/**
 * @file sync_timer.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief asio C++ library/Tutorial/Basic Skills/Timer.1 - Using a timer synchronously 
 *       (https://think-async.com/Asio/asio-1.20.0/doc/asio/tutorial/tuttimer1.html)
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>

#include <asio.hpp>

int main() {
    //* asio 사용 시, 모든 프로그램은 io_contex 혹은 thread_pool와 같은 I/O 실행 컨택스트를 하나 이상 가지고 있어야 한다.
    asio::io_context io;
    //* steady_timer 클래스를 사용한다.
    asio::steady_timer t{io, asio::chrono::seconds(5)};
    //* steady_timer에 의한 블록 대기를 실행
    t.wait();
    //* 5초 대기 후 출력
    std::cout << "Hello synchronous orld!!" << std::endl;
    return 0;
}