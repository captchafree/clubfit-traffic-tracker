#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "CSVRow.hpp"

class ImageProcessor {
    public:
        ImageProcessor() {
            this->fgbg = cv::createBackgroundSubtractorMOG2();
            
            std::ifstream file("config.csv");
            CSVRow row;
            while(file >> row) {
                this->detection_locations.push_back(cv::Rect(stoi(row[0]), stoi(row[1]), stoi(row[2]), stoi(row[3])));
            }
        }

        cv::Mat3b process(cv::Mat const &frame) {
            //Show original footage
            cv::imshow("Original", frame);

            //Apply frame to background subtractor
            this->fgbg->apply(frame, this->fgMask);

            cv::Mat3b result;
            //Convert single-channel image to BGR
            cv::cvtColor(this->fgMask, result, cv::COLOR_GRAY2BGR);
            int index = 1;
            //Iterate over each detection location
            for(std::vector<cv::Rect>::iterator it = detection_locations.begin(); it != detection_locations.end(); ++it) {
                //Crop mask based on detection location bounds
                cv::Mat cropped = this->fgMask(*it);

                //Invert image
                cv::bitwise_not(cropped, cropped);

                //Calculate percent of image that is white pixels
                std::vector<cv::Mat> channels;
                cv::split(cropped, channels);
                int total = cropped.rows * cropped.cols;
                float avg = (float(total - cv::countNonZero(cropped)) / total) * 100;

                //If average is over threshold object is detected in region
                if(avg > 35) {
                    std::cout << "Object Detected in box " << index << " (% white = " << avg << ")" << std::endl;
                    cv::rectangle(result, *it, cv::Scalar(0,255,0), 2);
                } else {
                    cv::rectangle(result, *it, cv::Scalar(255,255,255), 2);
                }

                index++;
            }
            
            return result;
        }

    private:
        cv::Ptr<cv::BackgroundSubtractor> fgbg;
        std::vector<cv::Rect> detection_locations;
        cv::Mat fgMask;
};