#include "third-party/include/serial/serial.h"
#include <iostream>
#include <thread>
int main()
{
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

    bool running = true;

    while(running){
        
        std::string result = my_serial.read(7);
        std::cout << result;
        /*
        //Writing to Serial 
        std::string command;
        std::cout << "Give a command :" << std::endl;
        std::cin >> command;
        command = command + "\r\n";
        size_t bytesWritten = my_serial.write(command);
        std::cout << "Bytes sent: " << bytesWritten << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(command == "exit"){
            running = false;
        }
        */

    }




    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //my_serial.flushInput();
    //std::string response = my_serial.read(6);
    //std::cout << "Arduion: " << response << std::endl;


}