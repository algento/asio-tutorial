/**
 * @file async_tcp_server.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <asio.hpp>
#include <asio/error_code.hpp>
#include <asio/io_context.hpp>

class ConnHandler : public std::enable_shared_from_this<ConnHandler> {
 public:
    using Ptr = std::shared_ptr<ConnHandler>;
    explicit ConnHandler(asio::io_context& io_context) : socket_(io_context){};
    static Ptr Create(asio::io_context& io_context) {
        return std::make_shared<ConnHandler>(io_context);
    }

    asio::ip::tcp::socket& socket() { return socket_; }
    void Start() {
        socket_.async_read_some(
            asio::buffer(data_, max_length),
            [capture0 = shared_from_this()](auto&& PH1, auto&& PH2) {
                capture0->HandleRead(std::forward<decltype(PH1)>(PH1),
                                     std::forward<decltype(PH2)>(PH2));
            });

        socket_.async_write_some(
            asio::buffer(data_, max_length),
            [capture0 = shared_from_this()](auto&& PH1, auto&& PH2) {
                capture0->HandleWrite(std::forward<decltype(PH1)>(PH1),
                                      std::forward<decltype(PH2)>(PH2));
            });
    }

    void HandleRead(const asio::error_code& error, size_t byte_transferred) {
        if (!error) {
            std::cout << data_ << std::endl;
            std::cout << byte_transferred << std::endl;
        } else {
            std::cerr << "error: " << error.message() << std::endl;
            socket_.close();
        }
    }

    void HandleWrite(const asio::error_code& error, size_t byte_transferred) {
        if (!error) {
            std::cout << "Server sent Hello message" << std::endl;
        } else {
            std::cerr << "error: " << error.message() << std::endl;
            socket_.close();
        }
    }

 private:
    asio::ip::tcp::socket socket_;
    std::string message_ = "Hello from Server";
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server {
 public:
    Server(asio::io_context& io_context)
        : acceptor_(io_context,
                    asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1234)) {
        StartAccept();
    }
    void HandleAccept(ConnHandler::Ptr connection,
                      const asio::error_code& error) {
        if (!error) {
            connection->Start();
        }
        StartAccept();
    }

 private:
    void StartAccept() {
        ConnHandler::Ptr connection = ConnHandler::Create(
            (asio::io_context&)acceptor_.get_executor().context());
        // acceptor_.async_accept(connection->socket(),
        //                        std::bind(&Server::HandleAccept, this,
        //                                  connection, std::placeholders::_1));
        acceptor_.async_accept(
            connection->socket(), [this, connection](auto&& PH1) {
                HandleAccept(connection, std::forward<decltype(PH1)>(PH1));
            });
    }
    asio::ip::tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    try {
        asio::io_context io_context;
        Server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}