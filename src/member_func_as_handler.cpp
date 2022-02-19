/**
 * @file member_func_as_handler.cpp
 * @author Sejong Heo (tromberX@gmail.com)
 * @brief asio C++ library/Tutorial/Basic Skills/Timer.4 - Using a member function as a handler
 *        (https://think-async.com/Asio/asio-1.20.0/doc/asio/tutorial/tuttimer4.html)
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <functional>
#include <iostream>

#include <asio.hpp>

//* 별도의 함수가 아니라 클래스의 멤버함수를 콜백 핸들러로 사용한다.
//* 클래스를 사용할 경우, 별도의 인자를 등록하지 않고 해당 인자를 멤버 변수로 하여 콜백 핸들러의 인자를 최소화할 수 있다.
//* 대신, 클래스의 this 포인터를 입력으로 넣어준다.
class Printer {
 public:
    explicit Printer(asio::io_context& io)
        : timer_(io, asio::chrono::seconds(1)), count_(0) {
        // timer_.async_wait(std::bind(&Printer::print, this));
        timer_.async_wait(
            std::bind(&Printer::print, this, std::placeholders::_1));
    }

    ~Printer() { std::cout << "Final count is " << count_ << std::endl; }

    //* 콜백핸들러로 사용할 멤버 함수는 일반 함수와 달리 굳이 asio::error_code를 인자로 받을 필요가 없다.
    void print(const asio::error_code& /* e */) {
        if (count_ < 5) {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expiry() + asio::chrono::seconds(1));
            timer_.async_wait(
                std::bind(&Printer::print, this, std::placeholders::_1));
        }
    }

 private:
    asio::steady_timer timer_;
    int count_;
};

int main() {
    asio::io_context io;
    Printer p(io);
    io.run();
    return 0;
}