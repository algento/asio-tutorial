#include <chrono>
/**
 * @file bind_arguments_to_handler.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief asio C++ library/Tutorial/Basic Skills/Timer.3 - Binding arguments to a handler
 *        (https://think-async.com/Asio/asio-1.20.0/doc/asio/tutorial/tuttimer3.html)
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <functional>
#include <iostream>

#include <asio.hpp>

//* 핸들러 함수에 error_code 외에 추가 파라미터를 전달한다.
//* (타이머 객체에 대한 포인터, 타이머가 6이될 때, 프로그램을 중지시키는데 사용할 카운터 변수에 대한 포인터)
void print(const asio::error_code& /*e*/, asio::steady_timer* t, int* count) {
    if (*count < 5) {
        //* 카운트가 5보다 작다면, 카운트를 출력하고 증가시킨다.
        std::cout << *count << std::endl;
        ++(*count);
        //* 기존 타이며의 만료 시간을 증가시킨다.
        t->expires_at(t->expiry() + asio::chrono::seconds(1));
        std::cout << "expire time: "
                  << std::chrono::duration_cast<std::chrono::seconds>(
                         t->expiry().time_since_epoch())
                         .count()
                  << " seconds" << std::endl;
        ;
        t->async_wait(std::bind(print, std::placeholders::_1, t, count));
    }
    //* 카운트가 5가 되면 더이상 비동기대기를 하지 않게 된다. 이렇게 되면, io_context::run()이 반환되면 중지된다.
}

int main() {
    asio::io_context io;
    int count = 0;
    //* 초기 타이머를 등록한다. 초기 만료 시간을 1초로 등록하지만, 콜백 핸들러에서 만료 시간을 증가시킨다.
    asio::steady_timer t(io, asio::chrono::seconds(1));
    t.async_wait(std::bind(print, std::placeholders::_1, &t, &count));
    io.run();
    std::cout << "Final count is " << count << std::endl;
}