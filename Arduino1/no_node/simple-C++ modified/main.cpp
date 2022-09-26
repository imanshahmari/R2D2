#include <CppLinuxSerial/SerialPort.hpp>
#include <string>
#include <array>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>



using namespace mn::CppLinuxSerial;
using namespace std::chrono_literals;


void CallBack(std::string servoValue){

	SerialPort serialPort("/dev/ttyACM0", BaudRate::B_9600, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
	// SerialPort serialPort("/dev/ttyACM0", 13000);
	serialPort.SetTimeout(-1); // Block when reading until any data is received
	std::string sender = servoValue;
	
	serialPort.Open();
	auto start = std::chrono::high_resolution_clock::now();
    //std::string readData;
	serialPort.Write(sender);
	serialPort.Close();
}



int main() {
	// Create serial port object and open serial port
	//SerialPort serialPort("/dev/ttyACM0", BaudRate::B_9600, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
	// SerialPort serialPort("/dev/ttyACM0", 13000);
	//serialPort.SetTimeout(-1); // Block when reading until any data is received
	//serialPort.Open();

	//Write some ASCII datae
	std::string servoValue;
	std::cout << "Type a number: ";
	std::cin >> servoValue;
	// Read some data back
    while(1) {
        CallBack(servoValue);
        std::this_thread::sleep_for(2000ms);
    }

	// Close the serial port
	//serialPort.Close();
}
