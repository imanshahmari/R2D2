#include "opencv2/video/tracking.hpp"
#include <opencv2/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>



//Initilize parameters 
std::string configurationPath = "/weights/yolov3-tiny.cfg";
std::string weightsPath = "/weights/yolov3-tiny_final.weights";
std::string objects = "kiwi_car";
std::vector<std::string> classes;

float confThreshold = 0.1f; // Confidence threshold
float nmsThreshold = 0.4f;  // Non-maximum suppression threshold
int inpWidth = 416;        // Width of network's input image
int inpHeight = 416;       // Height of network's input image



//Functions 

std::tuple <uint16_t, uint16_t, uint16_t, uint16_t> postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs)
{
    // ----------------------------------
    // Remove the bounding boxes with low confidence using non-maxima suppression
    // ----------------------------------
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    float max_confidence = 0.0f;
    uint16_t centre_x_fun = 0;
    uint16_t centre_y_fun = 0;
    uint16_t width_fun = 0;
    uint16_t height_fun = 0;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        if (confidences[idx] > max_confidence) {
            max_confidence = confidences[idx];
            centre_x_fun = box.x + box.width/2;
            centre_y_fun = box.y + box.height/2;
            width_fun = box.width;
            height_fun = box.height;
        }
    }
    //static_cast<uint16_t>(centre_y)
    return {centre_x_fun, centre_y_fun, width_fun, height_fun};
}





