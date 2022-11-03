#include <iostream>
#include <boost/asio.hpp>

int main() {
    try {
        boost::asio::io_context io_context;

        boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1339));
        
        while(true) {
            std::cout << "Accepting" << std::endl;

            boost::asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Connected" << std::endl;

            std::string message = "Hi";
            boost::system::error_code error;

            boost::asio::write(socket, boost::asio::buffer(message), error);
        }
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}