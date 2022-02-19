/**
 * @file sync_handler_in_multi_thread.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief asio C++ library/Tutorial/Basic Skills/Timer.5 - Synchronising handlers in multithreaded programs
 *        (https://think-async.com/Asio/asio-1.20.0/doc/asio/tutorial/tuttimer5.html)
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <functional>
#include <iostream>
#include <thread>

#include <asio.hpp>
#define ENABLE_STRAND 1

//* 멀티스레드에서 콜백 핸들러를 동기화시기키 위해 strand 클래스를 사용하는 법을 배운다.
//* 이전 예제에서는 하나의 스레드 (메인 스레드)에서만 io_context::run()을 호출했다.
//* io_context::run()을 호출한 스레드에서만 해당 io_context를 이용한 콜밸핸들러를 호출할 수 있다.
//* 따라서, 하나의 io_context에서 여러 개의 콜백 핸들러를 호출할 경우, 동시에 호출하는 것을 불가능하다.
//* 단일 스레드 접근 방식은 특히 서버에 다음과 같은 제한 사항을 가진다.
//*   1. 핸들러 완료에 오랜 시간이 걸리면 응답성이 떨어진다.
//*   2. 멀티 프로세서 시스템에서 확장할 수 없다.
//* 이를 극복하기 위해 thread_pool에서 io_context::run()을 돌릴 수 있다.
//* 하지만 이 경우, 핸들러가 동시에 실행될 수 있으므로,
//* 핸들러가 공유된 리소스 혹은 스레드에 안전하지 않은 리소스에 접근할 때 동기화하는 방법이 필요하다.

class Printer {
 public:
    explicit Printer(asio::io_context& io)
        : strand_(asio::make_strand(io)),
          timer1_(io, asio::chrono::seconds(1)),
          timer2_(io, asio::chrono::seconds(1)),
          count_(0) {
#if (ENABLE_STRAND)
        timer1_.async_wait(
            asio::bind_executor(strand_, std::bind(&Printer::print1, this)));
        timer2_.async_wait(
            asio::bind_executor(strand_, std::bind(&Printer::print2, this)));
#else
        timer1_.async_wait(std::bind(&Printer::print1, this));
        timer2_.async_wait(std::bind(&Printer::print2, this));
#endif
    }

    ~Printer() { std::cout << "Final count is " << count_ << std::endl; }

    //* 콜백핸들러로 사용할 멤버 함수 print1, print2는 count_라는 리소스를 공유한다.
    void print1() {
        if (count_ < 10) {
            std::thread::id this_id = std::this_thread::get_id();
            std::cout << "Thread ID: " << this_id << std::endl;
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + asio::chrono::seconds(1));
#if (ENABLE_STRAND)
            //* 콜백 핸들러를 strand에 바인드한다. bind_executor는 strand를 통해서 핸들러를 전달하는 새로운 핸들러를 생성하여 반환한다.
            timer1_.async_wait(asio::bind_executor(
                strand_, std::bind(&Printer::print1, this)));
#else
            timer1_.async_wait(std::bind(&Printer::print1, this));
#endif
        }
    }

    void print2() {
        if (count_ < 10) {
            std::thread::id this_id = std::this_thread::get_id();
            std::cout << "Thread ID: " << this_id << std::endl;
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + asio::chrono::seconds(1));
#if (ENABLE_STRAND)
            timer2_.async_wait(asio::bind_executor(
                strand_, std::bind(&Printer::print2, this)));
#else
            timer2_.async_wait(std::bind(&Printer::print2, this));
#endif
        }
    }

 private:
    asio::steady_timer timer1_;
    asio::steady_timer timer2_;
    //* strand 클래스는 이 클래스를 통해서 전달된 (dispatched) 여려 핸들러에 대해서  실행 중인
    //* 핸들러가 완료되기 전에 다음 핸들러가 실행되지 않도록 보장하는 executor_adapter이다.
    //* 이것은 io_context::run을 수행하는 스레드의 수와 상관없이 동작하는 것이 보장된다.
    //* 물론 strand를 통해서 전달되지 않거나 다른 strand를 통해서 전달되지 않은 핸들러와는 동시에 실행할 수 있다.
    asio::strand<asio::io_context::executor_type> strand_;
    int count_;
};

int main() {
    asio::io_context io;
    Printer p(io);

    //* https://stackoverflow.com/questions/9048119/why-cant-stdbind-and-boostbind-be-used-interchangeably-in-this-boost-asio-t
    //* asio::io_context::run()은 오버로드 함수로 인자가 없는 것과 하나가 있는 것이 있다.
    //* std::bind의 deduction 과정에서 인자가 있는 것과 연결되어 오류가 발생한다.
    // asio::thread t(std::bind(&asio::io_context::run, &io)); //! Compile error
    asio::thread t(std::bind(
        static_cast<asio::io_context::count_type (asio::io_context::*)()>(
            &asio::io_context::run),
        &io));
    //* 위에서 스레드를 별도로 생성하여 이 프로그램에서는 main 스레드와 t 스레드, 2개의 스레드가 동작한다.

    // asio::thread t([&io] { io.run(); });
    io.run();
    t.join();
    return 0;
}