#include <boost/asio.hpp>
#include <array>
#include <iostream>

std::string unknownRead(boost::asio::ip::tcp::socket& socket) 
{
    boost::system::error_code error;

    std::stringstream sstream;
    for(;;) //loop read_some
    {
        if(!socket.available()) //check if all is read
        {
            break;
        }

        std::array<char, 128> buf;
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if(error == boost::asio::error::eof) 
        {
            break;
        } 
        else if(error) 
        {
            throw boost::system::system_error(error);
        }

        sstream.write(buf.data(), len);
    }

    boost::asio::write(socket, boost::asio::buffer("ok"), error); //response
    return sstream.str();
}

bool cmdRead(boost::asio::ip::tcp::socket& socket) 
{
    boost::system::error_code error;
    std::array<char, 3> buf;
    size_t len = boost::asio::read(socket, boost::asio::buffer(buf), error);

    if(!len) //check if disconnected
    {
        return false;
    }

    if(strncmp(buf.data(), "ft", 2) == 0) //check command
    {
        std::cout << unknownRead(socket) << std::endl; //read file dest then wait
        socket.wait(socket.wait_read);

        int fileSize = std::stoi(unknownRead(socket)); //read file size
        std::cout << fileSize << std::endl;

        std::vector<char> bufV(fileSize);
        len = boost::asio::read(socket, boost::asio::buffer(bufV), error); //read file
        std::cout.write(bufV.data(), len);
        std::cout << std::endl;
    }

    return true;
}

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

        while(cmdRead(socket)) //wait for command unless disconnected
        {
            //system("read -p 'enter'");
        }
        
        std::cout << "end" << std::endl;
    } 
    catch(std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}