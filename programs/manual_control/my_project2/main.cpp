#include "third-party/include/serial/serial.h"
#include <ncurses.h>
#include <iostream>
#include <thread>


int main(){
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
	std::string command;


	my_serial.write("p10000\r\n");

	initscr();
	while(running){

		refresh();

		int input = getch();

		printw("%d",input);

		switch(input){

			case 119: // key is w
		        command = "m1150150\r\n";
		        for (int i = 0; i < 10; ++i){
		        	my_serial.write(command);
		        	std::this_thread::sleep_for(std::chrono::milliseconds(10));
		        }
				break;
			case 115: // key is s 
		        command = "m0175175\r\n";
		        for (int i = 0; i < 10; ++i){
		        	my_serial.write(command);
		        	std::this_thread::sleep_for(std::chrono::milliseconds(10));
		        }
				break;
			case 97: //key is a
		        command = "m1000150\r\n";
		        for (int i = 0; i < 10; ++i){
		        	my_serial.write(command);
		        	std::this_thread::sleep_for(std::chrono::milliseconds(10));
		        }
				break;
			case 100: //key is d
		        command = "m1150000\r\n";
		        for (int i = 0; i < 10; ++i){
		        	my_serial.write(command);
		        	std::this_thread::sleep_for(std::chrono::milliseconds(10));
		        }
				break;
			case 112: // key is p
				command = "m0000000\r\n";
				for (int i = 0; i < 10; ++i){
					my_serial.write(command);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
		        my_serial.write(command);
		        break;
			case 32:
				command = "m0000000\r\n";
				for (int i = 0; i < 10; ++i){
					my_serial.write(command);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				running = false;
				break;
			case 49: // 1
				command = "s0\r\n";
				for (int i = 0; i < 10; ++i){
					my_serial.write(command);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				break;
			case 50: //2
				command = "s37\r\n";
				for (int i = 0; i < 10; ++i){
					my_serial.write(command);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				break;
			case 51: //3
				command = "s75\r\n";
				for (int i = 0; i < 10; ++i){my_serial.write(command);}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
		        my_serial.write(command);
				break;
			case 52: //4
				command = "s112\r\n";
				for (int i = 0; i < 10; ++i){
					my_serial.write(command);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
		        my_serial.write(command);
				break;
			case 53: //5 
				command = "s150\r\n";
				for (int i = 0; i < 10; ++i){
					my_serial.write(command);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				break;
			case 98: // buzzer on 98 is ascii value of keyboard 
				for (int i = 0; i < 10; ++i){
					my_serial.write("b1\r\n");
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				break;
			case 110: //n
				for (int i = 0; i < 10; ++i){
					my_serial.write("b0\r\n");
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				break;

		}
	}

	endwin();

	return 0;
}


/*
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
        
        std::string result = my_serial.read(5);
        std::cout << result;
        
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
    }
}
*/