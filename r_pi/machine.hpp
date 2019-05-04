#ifndef MACHINE_H
#define MACHINE_H

#include <opencv2/imgcodecs.hpp>

struct Machine {
    std::string name;
    std::vector<cv::Point> detection_locations;
};

#endif