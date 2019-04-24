#include <iostream>
#include <sstream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
 
using namespace cv;
using namespace std;

int read() {
    VideoCapture cap("test_images/sample.mov");
    if (!cap.isOpened()) {
        cout << "!!! Failed to open file: " << "sample.mov" << endl;
        return -1;
    }

    Mat frame, fgMask;
    Ptr<BackgroundSubtractor> fgbg = createBackgroundSubtractorMOG2();

    vector<Rect> detection_locations;
    detection_locations.push_back(Rect(235,26, 75,149));
    detection_locations.push_back(Rect(316,26, 75,149));
    detection_locations.push_back(Rect(316,200, 75,149));
    detection_locations.push_back(Rect(397,26, 75,149));

    for(;;) {
        if (!cap.read(frame))             
            break;

        fgbg->apply(frame, fgMask);

        Mat3b result;
        cvtColor(fgMask, result, COLOR_GRAY2BGR);
        int index = 1;
        for(vector<Rect>::iterator it = detection_locations.begin(); it != detection_locations.end(); ++it) {
            Mat cropped = fgMask(*it);

            bitwise_not(cropped, cropped);
            vector<Mat> channels;
            split(cropped, channels);

            int total = cropped.rows * cropped.cols;
            float avg = (float(total - countNonZero(cropped)) / total) * 100;

            if(avg > 35) {
                cout << "Object Detected in box " << index << " (% white = " << avg << ")" << endl;
                rectangle(result, *it, cv::Scalar(0,255,0), 2);
            } else {
                rectangle(result, *it, cv::Scalar(255,255,255), 2);
            }

            index++;
        }
        
        cv::imshow("window", result);

        char key = waitKey(30);
        if (key == 27) // ESC
           break;
    }

    return 0;
}

int main() {
    read();
    return 0;
}
