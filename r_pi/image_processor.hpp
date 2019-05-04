#include <fstream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "config.hpp"
#include "mongo.hpp"

class ImageProcessor {
    public:
        ImageProcessor() {
            this->fgbg = cv::createBackgroundSubtractorMOG2();
        }

        /*
        Process the frame and return the result
        */
        cv::Mat3b process(cv::Mat const &frame) {
            Config config = Config::get_instance();
            // Show original footage
            // cv::imshow("Original", frame);
            
            // Apply frame to background subtractor
            this->fgbg->apply(frame, this->fgMask);

            cv::Mat3b result;
            // Convert single-channel image to BGR
            cv::cvtColor(this->fgMask, result, cv::COLOR_GRAY2BGR);

            // Apply binary threshold
            cv::threshold(result, result, 250, 255, 0);

            // Remove noise
            cv::erode(result, result, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);
            cv::dilate(result, result, cv::Mat(), cv::Point(-1, -1), 3, 1, 1);

            // Iterate over each detection location
            std::vector<Machine> machines = config.get_machines();
            std::vector<Machine> occupied_machines;
            for(std::vector<Machine>::iterator it = machines.begin(); it != machines.end(); ++it) {
                
                std::string name = it->name;
                std::vector<cv::Point> points = it->detection_locations;


                // Generate mask
                cv::Mat mask(result.size().height, result.size().width, CV_8UC3, cv::Scalar(0, 0, 0));

                int num_points = points.size();
                int num_polygons = 1;
                int line_type = 8;
                cv::Point corners[1][num_points];
                for(int i = 0; i < num_points; i++) {
                    corners[0][i] = (points)[i];
                }
                const cv::Point* corner_list[1] = { corners[0] };
                cv::fillPoly(mask, corner_list, &num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);

                cv::Mat cropped;
                cv::bitwise_and(result, mask, cropped);

                // Get bounding box
                cv::Rect bounding_box = cv::boundingRect(points);
                cropped = cropped(bounding_box);
                
                // Invert image
                cv::Mat cropped_grayscale;
                cv::cvtColor(cropped, cropped_grayscale, cv::COLOR_BGR2GRAY);
                int white = cv::countNonZero(cropped_grayscale);

                // Calculate percent of image that is white pixels
                int total = cv::contourArea(points);
                std::vector<cv::Mat> channels;
                cv::split(cropped, channels);
                float avg = float(white) / float(total) * 100;
                std::vector<std::vector<cv::Point>> contours;
                contours.push_back(points);
                
                if(avg > 5) {
                    std::cout << "Machine `" << name << "` at `" << config.get_company() << "` is in use" << std::endl;
                    occupied_machines.push_back(*it);
                    cv::drawContours(result, contours, 0, cv::Scalar(0,255,0), 1, 8);
                } else {
                    cv::drawContours(result, contours, 0, cv::Scalar(255,255,255), 1, 8);
                }
            }

            Mongo::add_machine_data(occupied_machines);

            return result;
        }

    private:
        // Background subtractor used in the algorithm
        cv::Ptr<cv::BackgroundSubtractor> fgbg;

        // Mask containing information about the image's background
        cv::Mat fgMask;
};