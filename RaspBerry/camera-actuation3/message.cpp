#include "message.hpp"

Message::Message():{
	serial::Serial my_serial("/dev/rfcomm0", 9600, serial::Timeout::simpleTimeout(3000));

    if (my_serial.isOpen())
    {
        std::cout << "Port opened succesfully" << std::endl;
    }
    else
    {
        std::cout << "Port failed to open" << std::endl;
        exit(1);
    }
    my_serial.flushOutput();
}


void send_message(std::string m_message){
    std::string command = m_message;
    command = command + "\r\n";
    size_t bytesWritten = my_serial.write(command);
    std::cout << "Bytes sent: " << bytesWritten << std::endl;
}