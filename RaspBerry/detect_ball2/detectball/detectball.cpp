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

/** Function Headers */
//void detectAndDisplay( string& img_filename );
//void detectAndDisplay();

/** Global variables */
string cascade_name = "ball_cascade.xml";
CascadeClassifier cascade;

string window_name = "SPQR TEAM - ball detection with LBP";

// Create a window

int main( int argc, const char** argv )
{
	string dirname;
	cout << "SPQR TEAM - ball detection with LBP" << endl;
	cout << endl;
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
	while (true) {
		cap >> frame;

		Mat resized_frame(Size(320, 240), CV_8UC3);
		resize(frame, resized_frame, resized_frame.size());
		frame = resized_frame.clone();

	    std::vector<Rect> balls;
	    Mat frame_gray;

	    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
	    cascade.detectMultiScale(frame_gray, balls, 1.1, 5, 8, Size(16, 16));

	   Mat gui_frame = frame.clone();
	   for( unsigned int i = 0; i < balls.size(); i++ )
	   {
	      Point center( balls[i].x + cvRound(balls[i].width*0.5), cvRound(balls[i].y + balls[i].height*0.5) );
	      circle(gui_frame, center, cvRound(balls[i].width*0.5), Scalar( 255, 0, 255 ), 2, 8, 0 );
	      cout << "position of ball :" << balls[i].x << "," << balls[i].y << endl;
	      cout << "radius of the ball:" << cvRound(balls[i].width*0.5) << endl;
		  //Mat ballROI = frame_gray( balls[i] );
	   } 

	   //-- Show what you got
	   imshow( kWinName, gui_frame );
	   waitKey(5);
	}
	//cap.release();
	return 0;
}