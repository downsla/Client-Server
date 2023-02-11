#include <boost/asio.hpp>
#include <array>
#include <iostream>

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

int main(int argc, char* argv[]) 
{
    try 
    {
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::socket socket(ioContext);
        boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1337);
        boost::system::error_code error;

        for(;;) //loop until connect
        {
            socket.connect(endpoint, error);
            if(!error)
            {
                break;
            }
        }
        std::cout << "start" << std::endl;

        //system("read -p 'enter'");
        confirmWrite(socket, "ft /folder/file.txt"); //command with file dest
        //system("read -p 'enter'");
        confirmWrite(socket, "14"); //file size
        //system("read -p 'enter'");
        boost::asio::write(socket, boost::asio::buffer("file contents"), error); //file
        //system("read -p 'enter'");

        std::cout << "end" << std::endl;
    } 
    catch(std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}