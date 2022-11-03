#include <boost/asio.hpp>
#include <array>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context ioContext;

        boost::asio::ip::tcp::resolver resolver {ioContext};
        
        auto endpoints = resolver.resolve("127.0.0.1", "1339");

        boost::asio::ip::tcp::socket socket{ioContext};
        boost::asio::connect(socket, endpoints);

        while(true) {
            std::array<char, 128> buf;
            
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if(error == boost::asio::error::eof) {
                break;
            } else if(error) {
                throw boost::system::system_error(error);
            }

            std::cout.write(buf.data(), len);
        }
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}