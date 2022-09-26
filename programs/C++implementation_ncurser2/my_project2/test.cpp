#include "third-party/include/serial/serial.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <regex>
#include <sstream>


//Variables
//std::regex reg_forward ("(uf[^.]?+)");
//std::regex reg_backward ("(ub[^.]?+)");
static bool s_Finished = false;


//Functions

void getSensorDistance(serial::Serial& channel){

    std::regex reg ("(uf[^.]?+)");

    // Used when your searching a string
    std::smatch matches;
    int message_int;
    std::string message;

    bool running = true;
    std::string str;
    while(running == true){
        channel.flushOutput();
        str = channel.read(7);
        //std::cout << str << std::endl;
        while(std::regex_search(str, matches, reg)){

            message = matches.str(0).substr(2,5);
            
            std::stringstream data(message);

            data >> message_int;
            str = matches.suffix().str();
        }
        std::cout << message_int << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
/*
int turnServo(int direction, serial::Serial& channel, std::regex& reg_forward){
    std::string my_string;
    int result = -1;
    int front_distance;
    if(direction == 0){
        std::stringstream message;
        for(int i = 100; i < 200; i = i + 10){
            channel.flushOutput();
            my_string = channel.read(7);
            front_distance = getSensorDistance(my_string,reg_forward);
            message.str("");
            channel.flushOutput();
            message << "s" << i <<"\r\n";
            std::cout << message.str() << std::endl; 
            channel.write(message.str());
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (front_distance > 50 && front_distance < 200){
                result = i;
                break;
            }
        }
    }
    else if (direction == 1){
        std::stringstream message;
        for(int i = 100; i > 0; i = i - 10){
            channel.flushOutput();
            my_string = channel.read(7);
            front_distance = getSensorDistance(my_string,reg_forward);
            message.str("");
            channel.flushOutput();
            message << "s" << i <<"\r\n";
            std::cout << message.str() << std::endl; 
            channel.write(message.str());
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (front_distance > 50 && front_distance < 200){
                result = i;
                break;
            }
        }
    }
    return result;
}
*/

int main()
{
    //serial::Serial my_serial("/dev/rfcomm0", 9600, serial::Timeout::simpleTimeout(3000));
    serial::Serial my_serial("/dev/ttyACM0", 9600, serial::Timeout::simpleTimeout(3000));
    if (my_serial.isOpen()){
        std::cout << "Port opened succesfully" << std::endl;
    }
    else{
        std::cout << "Port failed to open" << std::endl;
        exit(1);
    }

    std::thread t1(getSensorDistance,std::ref(my_serial));

    std::cout << "thread 2 is active too" << std::endl;

    t1.join();

}
