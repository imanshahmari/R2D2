#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include "third-party/include/serial/serial.h"
#include <fstream>
#include <sstream>
#include <iostream>

class Message{
    public:
        Message();
        void send_message(std::string);
};

#endif
