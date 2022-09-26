#include "third-party/include/serial/serial.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <regex>
#include <sstream>
#include <mutex>
#include <ctime>
#include <functional>

//Variables
//std::regex reg_forward ("(uf[^.]?+)");
//std::regex reg_backward ("(ub[^.]?+)");
static bool s_Finished = false;
std::mutex coutMutex;
std::mutex coutMutex2;

//Functions


void actuate(serial::Serial& my_serial, int degree ){
    my_serial.flushOutput();
    my_serial.write("s90\r\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    if (degree < 65  && degree > 1){
        my_serial.flushOutput();
        my_serial.write("m0180000\r\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
    else if(degree >=65 && degree <= 130){
        my_serial.flushOutput();
        my_serial.write("m0180180\r\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
    else{
        my_serial.flushOutput();
        my_serial.write("m0000180\r\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
}

void getSensorDistance(serial::Serial& channel, int& forward_distance){

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
        /*
        coutMutex.lock();
        std::cout << message_int << std::endl;
        coutMutex.unlock();
        */
        forward_distance = message_int;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void scan(serial::Serial& my_serial, int& forward_distance){
    std::stringstream message;
    while(true){
        /*
        coutMutex.lock();
        std::cout << "thread 2 " << forward_distance << std::endl;
        coutMutex.unlock();
        */
        int i;
        while (forward_distance < 50 && forward_distance > 0){
            my_serial.flushOutput();
            my_serial.write("m0000000\r\n");
            i = std::rand() % 200;
            if(i > 100){
                i = i - 10;
            }
            else{
                i = i + 10;
            }
            message.str("");
            my_serial.flushOutput();
            message << "s" << i <<"\r\n";
            coutMutex.lock();
            std::cout << message.str() << std::endl;
            coutMutex.unlock();
            my_serial.write(message.str());
            std::this_thread::sleep_for(std::chrono::milliseconds(250)); 
        }
        actuate(my_serial,i);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        coutMutex.lock();
        std::cout << i << std::endl;
        coutMutex.unlock();  
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
    serial::Serial my_serial("/dev/rfcomm0", 9600, serial::Timeout::simpleTimeout(100));
    //serial::Serial my_serial("/dev/ttyACM0", 9600, serial::Timeout::simpleTimeout(3000));
    int forward_distance = 0;
    if (my_serial.isOpen()){
        std::cout << "Port opened succesfully" << std::endl;
    }
    else{
        std::cout << "Port failed to open" << std::endl;
        exit(1);
    }

    std::thread t1(getSensorDistance,std::ref(my_serial), std::ref(forward_distance));
    /*
    std::stringstream message;
    while(true){
        coutMutex.lock();
        std::cout << "thread 2 " << forward_distance << std::endl;
        coutMutex.unlock();
        if (forward_distance < 20 && forward_distance > 0){
            for(int i = 100; i > 0; i = i - 10){
                message.str("");
                my_serial.flushOutput();
                message << "s" << i <<"\r\n";
                coutMutex.lock();
                std::cout << message.str() << std::endl;
                coutMutex.unlock();
                my_serial.write(message.str());
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }
        else if(forward_distance > 50){
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    */
    std::thread t2(scan,std::ref(my_serial), std::ref(forward_distance));

    t1.join();
    t2.join();

}
// behövs mutex för sensor cout 