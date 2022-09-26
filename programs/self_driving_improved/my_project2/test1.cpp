#include "third-party/include/serial/serial.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <regex>
#include <sstream>
#include <mutex>
#include <ctime>
#include <functional>


#define forward_speed "m1100100\r\n"
#define backward_speed "m0150150\r\n"
#define left_forward_speed "m1000100\r\n"
#define left_backward_speed "m0000120\r\n"
#define right_forward_speed "m1100000\r\n"
#define right_backward_speed "m0120000\r\n"
#define stay_speed "m0000000\r\n"
#define servo_neutral "s75\r\n"
#define minimum_distance 20


//Variables
//std::regex reg_forward ("(uf[^.]?+)");
//std::regex reg_backward ("(ub[^.]?+)");
bool doneNothing_Finished = false;
bool forward_Finished = false;
bool Scan_Finished = false;
bool ActuateWithDirection_Finished = false;


std::mutex coutMutex;
std::mutex coutMutex2;
std::mutex forwardDistanceMutex;
std::mutex backwardDistanceMutex;

//Functions

void DoNothing(serial::Serial& my_serial,bool& doneNothing_Finished,bool& Scan_Finished){
    for(int i =1; i < 20; ++i){
        //my_serial.flushOutput();
        my_serial.write(servo_neutral);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        //my_serial.flushOutput();
        my_serial.write(stay_speed);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    doneNothing_Finished = true;
    Scan_Finished = false;
    std::cout << "do Nothing Active" << std::endl;
}


void GoForward(serial::Serial& my_serial,bool& Scan_Finished){
    //my_serial.flushOutput();
    my_serial.write(forward_speed);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    /*
    my_serial.flushOutput();
    my_serial.write("m1175000\r\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(125));
    */
    //Scan_Finished = false;
    std::cout << "Forward Active" << std::endl;
}


void ActuateWithDirection(serial::Serial& my_serial, int degree,bool& ActuateWithDirection_Finished,bool& doneNothing_Finished){
    if (degree < 50  && degree > 1){
        //my_serial.flushOutput();
        my_serial.write(left_backward_speed);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        std::cout << "Turn left Active" << std::endl;
    }
    else{
        //my_serial.flushOutput();
        my_serial.write(right_backward_speed);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        std::cout << "Turn right Active" << std::endl;
    }
    ActuateWithDirection_Finished = true;
    doneNothing_Finished = false;
}

void getSensorDistanceFront(serial::Serial& channel, int& forward_distance){

    std::regex reg ("(uf\\d\\d?\\d?\\.)");

    // Used when your searching a string
    std::smatch matches;
    int message_int;
    std::string message;

    bool running = true;
    std::string str;
    while(running == true){
        str = channel.read(50);
        while(std::regex_search(str, matches, reg)){

            message = matches.str(0).substr(2,5);
            
            std::stringstream data(message);

            data >> message_int;
            str = matches.suffix().str();
        }

        forwardDistanceMutex.lock();
        forward_distance = message_int;
        std::cout << "forward_distance is:"<<forward_distance << std::endl;
        forwardDistanceMutex.unlock();
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void getSensorDistanceBack(serial::Serial& channel, int& backward_distance){

    std::regex reg ("(ub\\d\\d?\\d?\\.)");
    
    // Used when your searching a string
    std::smatch matches;
    int message_int;
    std::string message;

    bool running = true;
    std::string str;
    while(running == true){
        str = channel.read(50);
        while(std::regex_search(str, matches, reg)){

            message = matches.str(0).substr(2,5);
            
            std::stringstream data(message);

            data >> message_int;
            str = matches.suffix().str();
        }

        backwardDistanceMutex.lock();
        backward_distance = message_int;
        std::cout << "backward_distance is:" <<backward_distance << std::endl;
        backwardDistanceMutex.unlock();
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}



int Scan(serial::Serial& my_serial, int& forward_distance,int& backward_distance , bool& Scan_Finished,bool& doneNothing_Finished,bool& ActuateWithDirection_Finished){
    std::stringstream message;
    int i;
    int stuck = 0;
    while (forward_distance < minimum_distance && forward_distance > 1){
        ++stuck;
        i = std::rand() % 150;
        //std::cout << "forward distance is " << forward_distance << std::endl;
        if(i > 75){
            i = i - 10;
        }
        else{
            i = i + 10;
        }
        message.str("");
        //my_serial.flushOutput();
        message << "s" << i <<"\r\n";
        
        coutMutex.lock();
        std::cout << "forward distance in scanning is:"<<forward_distance << std::endl;
        coutMutex.unlock();
        
        my_serial.write(message.str());
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        my_serial.write(stay_speed);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        std::this_thread::sleep_for(std::chrono::milliseconds(400));

        coutMutex2.lock();
        std::cout << "stuck is: " << stuck << std::endl;
        coutMutex2.unlock();

        if(stuck > 10){
            if (backward_distance > minimum_distance){
                my_serial.write(backward_speed);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

    doneNothing_Finished = false;
    Scan_Finished = true;
    ActuateWithDirection_Finished = false;
    std::cout << "Scanning Active" << std::endl;
    return i;
}



int main()
{
    serial::Serial my_serial("/dev/rfcomm0", 9600, serial::Timeout::simpleTimeout(100));
    //serial::Serial my_serial("/dev/ttyACM0", 9600, serial::Timeout::simpleTimeout(3000));
    int forward_distance = 0;
    int backward_distance = 0;
    int degree;
    if (my_serial.isOpen()){
        std::cout << "Port opened succesfully" << std::endl;
    }
    else{
        std::cout << "Port failed to open" << std::endl;
        exit(1);
    }

    std::thread t1(getSensorDistanceFront,std::ref(my_serial), std::ref(forward_distance));
    std::thread t2(getSensorDistanceBack,std::ref(my_serial),std::ref(backward_distance));
    

    while(true){
        if (doneNothing_Finished == false){
            DoNothing(std::ref(my_serial),std::ref(doneNothing_Finished),std::ref(Scan_Finished));
            //std::cout << "do nothing active \n";
        }
        else{
            while(forward_distance > minimum_distance || forward_distance == 0){
                GoForward(std::ref(my_serial),std::ref(Scan_Finished));
                //std::cout << "forward active \n";
            }
            //DoNothing(std::ref(my_serial),std::ref(doneNothing_Finished),std::ref(Scan_Finished));
            if(Scan_Finished == false){
                std::cout << "scanning active \n";
                degree = Scan(std::ref(my_serial), std::ref(forward_distance),std::ref(backward_distance),std::ref(Scan_Finished),std::ref(doneNothing_Finished),std::ref(ActuateWithDirection_Finished));
                std::cout <<"degree:" << degree << std::endl;
                if (ActuateWithDirection_Finished==false){
                    ActuateWithDirection(std::ref(my_serial), std::ref(degree),std::ref(ActuateWithDirection_Finished),std::ref(doneNothing_Finished));
                    std::cout << "Actuation Active" << std::endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    t1.join();
    t2.join();

}
// behövs mutex för sensor cout 