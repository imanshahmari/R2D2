# Welcome to R2D2
This the software implemented in c++(std11) for controling a robot with specific hardware mentioned below. R2D2 uses object detection model YOLO for hand gesture recognition and LBP and other filters to navigate tracking a soccer ball when it encounters it.

## Hardwares used:
The hardware used for this project is the following 

*   Rasperrby Pi 4 with raspbcamera installed
*   Arduino uno control board
*   L298N motor driver
*   Bluetooth HC05
*   2st Ultrasonic sensor SparkFun
*   1st Servo motor SG90
*   2st DC motors 
*   1st LiPo battery 11.1V

## Technologies used:

Numerous projects inside Google. The following are some published papers that
use TF-GAN:

*   OpenCV library for detection and inference of machine learning models
*   UART Serial communication
*   YOLOV3 object detection model
*   Cascade Classifier
*   SensorFusion basic approach
*   Arduino Software


## REQUIREMENTS
* OpenCV 4.5.1
* CMake
* Arduino Software

## How to run:
Connect to the bluetooth module HC05 on arduino:
```
cd connect_bluetooth
bash connect_bluetooth.sh
```
You can build and run the programs now. 

## Maintainers

*   Iman Shahmari, imanshahmari@gmail.com, [github](https://github.com/imanshahmari)

## Authors

*   Iman Shahmari, imanshahmari@gmail.com, [github](https://github.com/imanshahmari)
