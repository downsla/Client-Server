#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include "network.h"

//! Server
/*!
Opens port for accepting.
*/
int main() 
{
    try 
    {
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1337);
        boost::asio::ip::tcp::acceptor acceptor(ioContext, endpoint);
        boost::asio::ip::tcp::socket socket(ioContext);
        acceptor.accept(socket); //wait for connection
        std::cout << "start" << std::endl;

        boost::system::error_code error;
        std::array<char, 3> buf;
        //system("read -p 'enter'");
        cmdRead(socket); //wait for command
        //system("read -p 'enter'");
        //cmdRead(socket);

        std::cout << "end" << std::endl;
    } 
    catch(std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}