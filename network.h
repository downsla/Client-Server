#include <boost/asio.hpp>
#include <array>
#include <iostream>

std::string unknownRead(boost::asio::ip::tcp::socket& socket) 
{
    boost::system::error_code error;

    std::stringstream sstream;
    for(;;) //loop read_some
    {
        size_t available = socket.available();
        if(!available) //check if all is read
        {
            break;
        }

        char* buf = new char[available];
        size_t len = socket.read_some(boost::asio::buffer(buf, available), error);
        if(error == boost::asio::error::eof) 
        {
            break;
        } 
        else if(error) 
        {
            throw boost::system::system_error(error);
        }

        sstream.write(buf, len);
    }

    boost::asio::write(socket, boost::asio::buffer("ok"), error); //response
    return sstream.str();
}

void cmdRead(boost::asio::ip::tcp::socket& socket) 
{
    boost::system::error_code error;
    std::array<char, 3> buf;
    std::array<char, 14> buf2;

    size_t len = boost::asio::read(socket, boost::asio::buffer(buf), error);

    if(strncmp(buf.data(), "ft", 2) == 0) //check command
    {
        std::cout << unknownRead(socket) << std::endl; //read file dest then wait
        socket.wait(socket.wait_read);

        std::cout << unknownRead(socket) << std::endl; //read file size

        len = boost::asio::read(socket, boost::asio::buffer(buf2), error); //read file
        std::cout.write(buf2.data(), len);
        std::cout << std::endl;
    }
}

bool confirmWrite(boost::asio::ip::tcp::socket& socket, std::string cmd)
{
    boost::system::error_code error;   
    boost::asio::write(socket, boost::asio::buffer(cmd), error);

    std::array<char, 3> buf;
    size_t len = boost::asio::read(socket, boost::asio::buffer(buf), error); //wait for reply
    std::cout.write(buf.data(), len);
    std::cout << std::endl;

    return strcmp(buf.data(), "ok") == 0;
}