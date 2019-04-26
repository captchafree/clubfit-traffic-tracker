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
                std::vector<cv::Point> corners;
                for(int i = 0; i < row.size(); i+=2) {
                    corners.push_back(cv::Point(stoi(row[i]), stoi(row[i+1])));
                }
                this->detection_locations.push_back(corners);
            }
        }

        /*
        Process the frame and return the result to show
        */
        cv::Mat3b process(cv::Mat const &frame) {
            //Show original footage
            //cv::imshow("Original", frame);
            
            //Apply frame to background subtractor
            this->fgbg->apply(frame, this->fgMask);

            cv::Mat3b result;
            //Convert single-channel image to BGR
            cv::cvtColor(this->fgMask, result, cv::COLOR_GRAY2BGR);
            cv::threshold(result, result, 254, 255, 0);

            int index = 1;
            //Iterate over each detection location
            for(std::vector<std::vector<cv::Point>>::iterator it = detection_locations.begin(); it != detection_locations.end(); ++it) {

                //Generate mask
                cv::Mat mask(result.size().height, result.size().width, CV_8UC3, cv::Scalar(0, 0, 0));

                int num_points = (*it).size();
                int num_polygons = 1;
                int line_type = 8;
                cv::Point corners[1][num_points];
                for(int i = 0; i < num_points; i++) {
                    corners[0][i] = (*it)[i];
                }
                const cv::Point* corner_list[1] = { corners[0] };
                cv::fillPoly(mask, corner_list, &num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);

                cv::Mat cropped;
                cv::bitwise_and(result, mask, cropped);

                //Get bounding box
                cv::Rect bounding_box = cv::boundingRect(*it);
                cropped = cropped(bounding_box);
                
                //Invert image
                cv::Mat cropped_grayscale;
                cv::cvtColor(cropped, cropped_grayscale, cv::COLOR_BGR2GRAY);
                int white = cv::countNonZero(cropped_grayscale);

                //Calculate percent of image that is white pixels
                int total = cv::contourArea(*it);
                std::vector<cv::Mat> channels;
                cv::split(cropped, channels);
                float avg = float(white) / float(total) * 100;
                std::vector<std::vector<cv::Point>> contours;
                contours.push_back(*it);
                
                if(avg > 35) {
                    //std::cout << "Object Detected in box " << index << " (% white = " << avg << ")" << std::endl;
                    cv::drawContours(result, contours, 0, cv::Scalar(0,255,0), 1, 8);
                } else {
                    cv::drawContours(result, contours, 0, cv::Scalar(255,255,255), 1, 8);
                }

                index++;
            }
            
            return result;
        }

    private:
        cv::Ptr<cv::BackgroundSubtractor> fgbg;
        std::vector<std::vector<cv::Point>> detection_locations;
        cv::Mat fgMask;
};