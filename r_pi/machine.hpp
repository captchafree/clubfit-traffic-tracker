#ifndef MACHINE_H
#define MACHINE_H

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <opencv2/imgcodecs.hpp>

struct Machine {
    std::string name;
    std::vector<cv::Point> detection_locations;
};

#endif