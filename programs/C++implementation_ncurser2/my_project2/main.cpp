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

	initscr();
	while(running){

		refresh();

		int input = getch();

		printw("%d",input);

		switch(input){
			case 119: // key is w
		        command = "m0200200\r\n";
		        my_serial.write(command);
				break;
			case 115: // key is s 
		        command = "m1200200\r\n";
		        my_serial.write(command);
				break;
			case 97: //key is a
		        command = "m0000200\r\n";
		        my_serial.write(command);
				break;
			case 100: //key is d
		        command = "m0200000\r\n";
		        my_serial.write(command);
				break;
			case 112:
				command = "m0000000\r\n";
		        my_serial.write(command);
		        break;
			case 32:
				command = "m0000000\r\n";
		        my_serial.write(command);
				running = false;
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