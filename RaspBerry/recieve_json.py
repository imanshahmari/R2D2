#!/usr/bin/env python3
import sys
import serial
import json

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    ser.reset_input_buffer()
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().rstrip()
            my_json = line.decode('utf8').replace("'",'"')
            print(my_json)
            data = json.loads(my_json)
            for i in data["data"]:
                print(i)
