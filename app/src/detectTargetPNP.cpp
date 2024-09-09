

/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : This is the script used reprojecting and visualizing image corners using pre-computed
 * camera calibration parameters.
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
    // cv::VideoCapture inputVideo("http://10.110.199.119:8080/video");
    cv::VideoCapture inputVideo("../data/testvideo3.mp4");
    bool readStatus = inputVideo.isOpened();
    if (readStatus == false)
    {
        std::cout << "\n couldnt read the video feed \n";
    }

    cv::Mat frame;
    cv::Size targetShape(9, 6);
    std::vector<cv::Point2f> cornersImage;

    std::vector<cv::Vec3f> worldCoords{};
    getWorldCoords(worldCoords, true, 20);

    // TASK 2
    std::vector<std::vector<cv::Point2f>> imageCornersList{};
    std::vector<std::vector<cv::Vec3f>> worldCoordsList{};
    int minCalibPoints{5};
    char userInput{};

    cv::Mat cameraMat{};
    double reProjError{};
    std::vector<double> distortionErrors{};
    cv::Mat rotationMat{};
    cv::Mat translationMat{};
    cv::Mat junk{};

    readCameraParameters(reProjError,
                         cameraMat,
                         junk,
                         junk,
                         distortionErrors);

    std::cout << "\n printing cameraMat " ;
    printmat(cameraMat, INT_MAX);


    for (;;)
    {
        inputVideo >> frame;
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        cv::resize(frame, frame, cv::Size(1000, 600), 1, 1, 1);
        bool detectStatus = cv::findChessboardCorners(frame, targetShape, cornersImage, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

        if (detectStatus == true)
        {
            cv::cornerSubPix(frame,
                             cornersImage,
                             cv::Size(11, 11),
                             cv::Size(-1, -1),
                             cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER,
                                              30,
                                              0.1));

            cv::Mat cornersImageMat(cornersImage);
            cv::Mat worldCoordsMat(worldCoords);

            cv::solvePnP(
                worldCoordsMat,
                cornersImageMat,
                cameraMat,
                distortionErrors,
                rotationMat,
                translationMat);

            std::cout << "\nprinting DETECTED ROTATION matrix ";
            printmat(rotationMat, INT_MAX);

            std::cout << "\nprinting DETECTED TRANSLATION matrix ";
            printmat(translationMat, INT_MAX);

            cv::drawChessboardCorners(frame, targetShape, cv::Mat(cornersImage), detectStatus);

            cv::Size gridSize{};
            gridSize = getGridDimsPix(cornersImage);

            for(int i = 0 ; i < cornersImage.size(); i++){
                cv::circle(frame, cornersImage.at(i), 3, cv::Scalar(0, 0, 250), 4);
            }

            cv::Mat vizCorners(cornersImage);
            
        }

        displayImage(frame, "processed video", false, false, false, true);

        cv::Mat tempWorldCoords(worldCoords);
    }

    return 0;
}