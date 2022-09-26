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
bool doneNothing_Finished = false;
bool forward_Finished = false;
bool Scan_Finished = false;
bool ActuateWithDirection_Finished = false;


std::mutex coutMutex;
std::mutex coutMutex2;
std::mutex forwardDistanceMutex;

//Functions

void DoNothing(serial::Serial& my_serial,bool& doneNothing_Finished,bool& Scan_Finished){
    for(int i =1; i < 20; ++i){
        my_serial.flushOutput();
        my_serial.write("s75\r\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        my_serial.flushOutput();
        my_serial.write("m0000000\r\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    doneNothing_Finished = true;
    Scan_Finished = false;
    std::cout << "do Nothing Active" << std::endl;
}


void GoForward(serial::Serial& my_serial,bool& Scan_Finished){
    my_serial.flushOutput();
    my_serial.write("m1000175\r\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(125));
    my_serial.flushOutput();
    my_serial.write("m1175000\r\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(125));
    //Scan_Finished = false;
    std::cout << "Forward Active" << std::endl;
}


void ActuateWithDirection(serial::Serial& my_serial, int degree,bool& ActuateWithDirection_Finished,bool& doneNothing_Finished){
    if (degree < 50  && degree > 1){
        my_serial.flushOutput();
        my_serial.write("m0000175\r\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        std::cout << "Turn left Active" << std::endl;
    }
    else{
        my_serial.flushOutput();
        my_serial.write("m0175000\r\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        std::cout << "Turn right Active" << std::endl;
    }
    ActuateWithDirection_Finished = true;
    doneNothing_Finished = false;
}

void getSensorDistance(serial::Serial& channel, int& forward_distance){

    //std::regex reg ("(uf[^.]?+)");
    std::regex reg ("(uf\\d\\d?\\d?\\.)");

    // Used when your searching a string
    std::smatch matches;
    int message_int;
    std::string message;

    bool running = true;
    std::string str;
    while(running == true){
        //channel.flushOutput();
        str = channel.read(30);
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
        forwardDistanceMutex.lock();
        forward_distance = message_int;
        std::cout << forward_distance << std::endl;
        forwardDistanceMutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


int Scan(serial::Serial& my_serial, int& forward_distance,bool& Scan_Finished,bool& doneNothing_Finished,bool& ActuateWithDirection_Finished){
    std::stringstream message;
    int i;
    while (forward_distance < 35 && forward_distance > 1){
        i = std::rand() % 150;
        //std::cout << "forward distance is " << forward_distance << std::endl;
        if(i > 75){
            i = i - 10;
        }
        else{
            i = i + 10;
        }
        message.str("");
        my_serial.flushOutput();
        message << "s" << i <<"\r\n";
        /*
        coutMutex.lock();
        std::cout << message.str() << std::endl;
        coutMutex.unlock();
        */
        my_serial.write(message.str());
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        my_serial.write("m0000000\r\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        std::this_thread::sleep_for(std::chrono::milliseconds(400));
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
    int degree;
    if (my_serial.isOpen()){
        std::cout << "Port opened succesfully" << std::endl;
    }
    else{
        std::cout << "Port failed to open" << std::endl;
        exit(1);
    }

    std::thread t1(getSensorDistance,std::ref(my_serial), std::ref(forward_distance));
    
    /*
    while(true){
        if (doneNothing_Finished == false){
            DoNothing(std::ref(my_serial),std::ref(doneNothing_Finished));
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else{
            if(forward_distance > 50 || forward_distance == 0){
                GoForward(std::ref(my_serial));
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            else{
                if(Scan_Finished == false){

                    Scan(std::ref(my_serial), std::ref(forward_distance),std::ref(Scan_Finished),std::ref(doneNothing_Finished));
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
                else{
                    if (doneNothing_Finished == false){
                        DoNothing(std::ref(my_serial),std::ref(doneNothing_Finished));
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    }
                    else{
                        if(ActuateWithDirection_Finished==false){
                            ActuateWithDirection(std::ref(my_serial), std::ref(forward_distance),std::ref(ActuateWithDirection_Finished),std::ref(doneNothing_Finished));
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        }
                        else{
                            if(doneNothing_Finished == false){
                                DoNothing(std::ref(my_serial),std::ref(doneNothing_Finished));
                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            }
                            else{
                                GoForward(std::ref(my_serial));
                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            }
                        }
                    }
                }
            }

        }
    }
    */
    while(true){
        if (doneNothing_Finished == false){
            DoNothing(std::ref(my_serial),std::ref(doneNothing_Finished),std::ref(Scan_Finished));
            //std::cout << "do nothing active \n";
        }
        else{
            while(forward_distance > 35 || forward_distance == 0 || forward_distance < 10){
                GoForward(std::ref(my_serial),std::ref(Scan_Finished));
                //std::cout << "forward active \n";
            }
            if(Scan_Finished == false){
                std::cout << "scanning active \n";
                degree = Scan(std::ref(my_serial), std::ref(forward_distance),std::ref(Scan_Finished),std::ref(doneNothing_Finished),std::ref(ActuateWithDirection_Finished));
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

}
// behövs mutex för sensor cout 