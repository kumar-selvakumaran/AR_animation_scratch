/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : This is the script used for visualing and exploring the Harris Corners detection algorithm.
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/types_c.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>

#include <utils.h>
#include <calibutils.h>

using std::string_literals::operator""s;

int main()
{
    // cv::VideoCapture inputVideo("https://10.110.199.119:8080/video");
    cv::VideoCapture inputVideo("../data/testvideo.mp4");
    bool readStatus = inputVideo.isOpened();
    if (readStatus == false)
    {
        std::cout << "\n couldnt read the video feed \n";
    }

    cv::Mat frame{}, out{};

    for (float currTime = 0.0f;; ++currTime)
    {
        inputVideo >> frame;
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        cv::resize(frame, frame, cv::Size(1000, 600), 1, 1, 1);

        displayImage(frame, "inputVideo video", false, false, false, true);

        int blockSize = (int) cv::waitKey(1);

        // blockSize = blockSize ? blockSize >=2 : 3;
        // blockSize = blockSize ? blockSize <=15 : 8;

        std::printf("\n blockSize : %d", blockSize);
        cv::cornerHarris(frame,
                         out,
                         2,
                         3,
                         0.03);
        cv::Mat frameNormed, frameNormedScaled;

        normalize( out, frameNormed, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());

        cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
        convertScaleAbs( frameNormed, frameNormedScaled);
        for( int i = 0; i < frameNormed.rows ; i++ )
        {
            for( int j = 0; j < frameNormed.cols; j++ )
            {
                if( frameNormed.at<float>(i,j) > 190)
                {
                    circle( frame,
                    cv::Point(j,i),
                    5,
                    cv::Scalar(0, 0, (int) frameNormedScaled.at<uchar>(i,j)),
                    2,
                    25,
                    0);
                }
            }
        }
        
        printmat(frameNormedScaled, 3);
        
        displayImage(frame, "processed video", false, false, false, true);
    }

    return 0;
}