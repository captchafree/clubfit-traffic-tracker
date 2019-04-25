#include <iostream>
#include <sstream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "image_processor.hpp"


int read() {
    cv::VideoCapture cap("test_images/sample.mov");
    if (!cap.isOpened()) {
        std::cout << "!!! Failed to open file: " << "sample.mov" << std::endl;
        return -1;
    }

    cv::Mat frame;
    ImageProcessor processor;
    for(;;) {
        if (!cap.read(frame))             
            break;

        cv::Mat3b result = processor.process(frame);
        
        //Display result
        cv::imshow("Object Detection", result);

        //Exit program on `ESC`
        char key = cv::waitKey(30);
        if (key == 27) // ESC
           break;
    }

    return 0;
}

int main() {
    read();
    return 0;
}

//COUNTOUR CODE
/*
Mat gray;
cvtColor( frame, gray, COLOR_BGR2GRAY );
blur( gray, gray, Size(3,3) );

Mat canny_output;
int thresh = 100;
Canny(gray, canny_output, thresh, thresh*2 );
vector<vector<Point> > contours;
findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
vector<vector<Point> > contours_poly( contours.size() );
vector<Rect> boundRect( contours.size() );
vector<Point2f>centers( contours.size() );
vector<float>radius( contours.size() );
for(size_t i = 0; i < contours.size(); i++) {
    approxPolyDP( contours[i], contours_poly[i], 3, true );
    boundRect[i] = boundingRect( contours_poly[i] );
    minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
}

RNG rng(12345);
Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
for(size_t i = 0; i< contours.size(); i++) {
    Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
    drawContours( drawing, contours_poly, (int)i, color );
    //rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2 );
    //circle( drawing, centers[i], (int)radius[i], color, 2 );
}
imshow("Contours", drawing);
*/