/*
*  detectball
*
*  Detect a black and white ball in images
*  Copyright 2017 Domenico Daniele Bloisi
*
*  This file is part of the tutorial
*  "How to Use OpenCV for Ball Detection" by Domenico D. Bloisi
*  http://web.unibas.it/bloisi/tutorial/balldetection.html
*  and it is distributed under the terms of the
*  GNU Lesser General Public License (Lesser GPL)
*
*  detectball is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  detectball is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with createpos.  If not, see <http://www.gnu.org/licenses/>.
*
*  This file contains a C++ OpenCV based implementation for
*  detecting a black and white ball in images as explained
*  in "How to Use OpenCV for Ball Detection" by Domenico D. Bloisi
*  http://profs.scienze.univr.it/~bloisi/tutorial/balldetection.html
*
*  Please, cite the above web page if you use createpos.
*
*  detectball has been written by Domenico D. Bloisi
*
*  Please, report suggestions/comments/bugs to
*  domenico.bloisi@gmail.com
*
*/

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include "third-party/include/serial/serial.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>


#if defined(_MSC_VER)
#include <tchar.h>
#include <strsafe.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#elif defined(__GNUC__) || defined(__GNUG__)
#include <dirent.h>
#endif

using namespace std;
using namespace cv;


/** Global variables */
string cascade_name = "ball_cascade.xml";
CascadeClassifier cascade;


enum action {left, right, stay, straight, back};


//Functions 
action checkAction(int,int,int,int,int,int);

action checkAction(int posistionX, int radius, int threshX1,int threshX2, int threshR1,int threshR2){
	if (posistionX < threshX1){
		return action::right;
	}
	else if (posistionX > threshX1  && posistionX < threshX2 ){
		if (radius > threshR2){
			return action::back;
		}
		else if(radius < threshR2 && radius > threshR1){
			return action::stay;
		}
		else if(radius < threshR1){
			return action::straight;
		}

	}
	else if (posistionX > threshX2){
		return action::left;
	}
}

int main( int argc, const char** argv )
{
	string dirname;

	serial::Serial my_serial("/dev/rfcomm0", 9600, serial::Timeout::simpleTimeout(3000));
	static const string kWinName = "Deep learning object detection in OpenCV";
	namedWindow(kWinName, WINDOW_NORMAL);	

	if (argc == 2) {
		dirname.assign(argv[1]);
	}
	else if (argc == 4) {
		dirname.assign(argv[1]);
                string a2(argv[2]); 
                if(a2.compare("-c") == 0) {
                    cascade_name.assign(argv[3]);
                }
	}
	else {
		cout << "Usage is:" << endl;
		cout << argv[0] << "<dir name> [-c <cascade filename>]" << endl;
		cout << endl;
		return EXIT_FAILURE;
	}
	
	//-- 1. Load the cascade
	cout << "Loading the cascade " << cascade_name << "...";
	cout.flush();
	if (!cascade.load(cascade_name)) {
		cout << endl;
		cout << "--(!)Error loading CASCADE: " << cascade_name << endl;
                cout << "Please provide the cascade filename" << endl;
                cout << "example:" << endl;
                cout << "         detectball ../test -c ../ball_cascade.xml" << endl;
		return EXIT_FAILURE;
	}
	else {
		cout << "[OK]" << endl;
	}
	Mat frame;
	VideoCapture cap;
	cap.open(0);

	int nr_message = 0;
	//int nr_message_right = 0;


	while (true) 
	{
		cap >> frame;

		Mat resized_frame(Size(320, 240), CV_8UC3);
		resize(frame, resized_frame, resized_frame.size());
		frame = resized_frame.clone();

	    std::vector<Rect> balls;
	    Mat frame_gray;

	    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
	    cascade.detectMultiScale(frame_gray, balls, 1.1, 5, 8, Size(16, 16));

	   Mat gui_frame = frame.clone();
	   action actionNow;
	   for( unsigned int i = 0; i < balls.size(); i++ )
	   {
	      Point center( balls[i].x + cvRound(balls[i].width*0.5), cvRound(balls[i].y + balls[i].height*0.5) );
	      circle(gui_frame, center, cvRound(balls[i].width*0.5), Scalar( 255, 0, 255 ), 2, 8, 0 );
	      cout << "position of ball :" << balls[i].x << "," << balls[i].y << endl;
	      cout << "radius of the ball:" << cvRound(balls[i].width*0.5) << endl;

	      actionNow = checkAction(balls[i].x,cvRound(balls[i].width*0.5),75,225,25,50);
	      if (nr_message % 5 == 0)
	      	{
		      switch(actionNow)
		      	{
		      	case action::right:
		      		my_serial.flushOutput();
		            my_serial.write("m0000200\r\n");
		            std::cout << "Bytes sent: right motor" << std::endl;
		            break;
		      	case action::left:
		      		my_serial.flushOutput();
		            my_serial.write("m0200000\r\n");
		            std::cout << "Bytes sent: right motor" << std::endl;
		            break;

		      	case action::stay:
		      		my_serial.flushOutput();
		            my_serial.write("m0000000\r\n");
		            std::cout << "Bytes sent: right motor" << std::endl;
		            break;


		      	case action::straight:
		      		my_serial.flushOutput();
		            my_serial.write("m0200200\r\n");
		            std::cout << "Bytes sent: right motor" << std::endl;
		            break;

		      	case action::back:
		      		my_serial.flushOutput();
		            my_serial.write("m1200200\r\n");
		            std::cout << "Bytes sent: right motor" << std::endl;
		            break;
		      	}
	      	}
	    }

		nr_message= nr_message +1 ;
		  //std::cout << "Nr messages " << nr_message << std::endl;
		//std::cout << "Nr messages left " << nr_message_left << std::endl;

		//-- Show what you got
		imshow( kWinName, gui_frame );
		waitKey(5);
	}
	return 0;
}