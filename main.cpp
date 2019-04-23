#include <iostream>
#include <sstream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
 
using namespace cv;
using namespace std;

int read() {
    cv::VideoCapture cap("test_images/sample.mov");
    if (!cap.isOpened()) {
        cout << "!!! Failed to open file: " << "sample.mov" << endl;
        return -1;
    }

    Mat frame, fgMask;
    Ptr<BackgroundSubtractor> fgbg = createBackgroundSubtractorMOG2();

    for(;;) {

        if (!cap.read(frame))             
            break;

        fgbg->apply(frame, fgMask);
        rectangle(fgMask, cv::Point(315,25), cv::Point(400,175),cv::Scalar(255,255,255), 1);

        Mat cropped = fgMask(Rect(316,26, 75, 149));

        vector<Mat> channels;
        split(cropped, channels);
        float avg = mean(channels[0])[0];

        if(avg > 30) {
            cout << "Object Detected (% 255 = " << avg << ")" << endl;
        }

        cv::imshow("window", cropped);

        char key = waitKey(10);
        if (key == 27) // ESC
            break;
    }

    return 0;
}

int main() {
    read();
    return 0;
}