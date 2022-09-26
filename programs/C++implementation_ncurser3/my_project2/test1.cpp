#include "third-party/include/serial/serial.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <regex>
#include <sstream>


//Variables
std::regex reg_forward ("(uf[^.]?+)");
std::regex reg_backward ("(ub[^.]?+)");
static bool s_Finished = false;





//Functions
int getSensorDistance(std::string str,std::regex reg){
    
    // Used when your searching a string
    std::smatch matches;
    int message_int;
    std::string message;
    while(std::regex_search(str, matches, reg)){
        
        // Get the first match
        //std::cout << matches.str(0) << "\n";
        //std::cout << matches.str(0).substr(2,4); //<< std::endl;
        message = matches.str(0).substr(2,5);

        //std::cout << message;
        //std::cout << (int) matches.str(0).substr(2,5) << std::endl;
        
        std::stringstream data(message);

        data >> message_int;

        //std::cout << message_int;
        // Eliminate the previous match and create
        // a new string to search
        str = matches.suffix().str();
        
        //std::cout << "\n";
    }
    return message_int;
}

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

int main()
{
    //serial::Serial my_serial("/dev/rfcomm0", 9600, serial::Timeout::simpleTimeout(3000));
    serial::Serial my_serial("/dev/ttyACM0", 9600, serial::Timeout::simpleTimeout(3000));
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


    int front_distance;
    int back_distance;
    int servo_pos;

    bool running = true;
    std::string my_string;
    std::string part_string;

    while(running){

        my_serial.flushOutput();
        my_string = my_serial.read(7);
        //std::cout << result;

        //part_string= std::regex_replace(my_string, std::regex("uf[^\r]?"), "");
        front_distance = getSensorDistance(my_string,reg_forward);
        //back_distance = getSensorDistance(my_string,reg_backward);
        //std::cout << part_string.substr(0,6) << std::endl;
        //std::cout << part_string << std::endl;

        if (front_distance > 1 && front_distance < 10){
            /*
            my_serial.flushOutput();
            my_serial.write("m1150150\r\n");         
            std::cout << "hello there";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            my_serial.flushOutput();
            my_serial.write("m0000000\r\n");
            */
            //servo_pos = turnServo(0,my_serial,reg_forward);
            //std:: cout<< "kortaste avstånd vinkel:" << servo_pos << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }   

        
        std::cout << "Forward distance is: " << front_distance << std::endl;
        //front_distance = (front_distance == 0) ? : std::cout << "Forward distance is: " << front_distance << std::endl;

        //std::cout << "Backward distance is: " << back_distance << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }




    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //my_serial.flushInput();
    //std::string response = my_serial.read(6);
    //std::cout << "Arduion: " << response << std::endl;


}