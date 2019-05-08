#ifndef MACHINE_H
#define MACHINE_H

#include <opencv2/imgcodecs.hpp>

struct Machine {
    // The name of the machine
    std::string name;

    // The tolerance the image processor will use to determine if the machine is occupied
    // The tolerance is a value in (0, 100]
    float tolerance;

    // A list of points defining the bounds of the machine
    std::vector<cv::Point> contour_points;
};

#endif