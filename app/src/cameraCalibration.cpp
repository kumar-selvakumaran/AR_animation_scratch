/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : This is the script used for calibrating the camera and writing the calibrationData.csv
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
    
    // for live video capture
    // cv::VideoCapture inputVideo("http://10.110.115.209:8080/video");
    
    //for saved video capture
    cv::VideoCapture inputVideo("../data/testvideo.mp4");
    
    bool readStatus = inputVideo.isOpened();
    // int ex = static_cast<int>(inputVideo.get(cv::CAP_PROP_FOURCC));     // Get Codec Type- Int form
    if (readStatus == false)
    {
        std::cout << "\n couldnt read the video feed \n";
    }

    cv::Mat frame;
    // 6 corners vertical, 9 corners horizontal
    cv::Size targetShape(9, 6);
    std::vector<cv::Point2f> cornersImage;

    std::vector<cv::Vec3f> worldCoords{};
    getWorldCoords(worldCoords, true, 20);

    // TASK 2
    std::vector<std::vector<cv::Point2f>> imageCornersList{};
    std::vector<std::vector<cv::Vec3f>> worldCoordsList{};
    int minCalibPoints{5};
    char userInput{};

    for (;;)
    {
        inputVideo >> frame;
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        cv::resize(frame, frame, cv::Size(1000, 600), 1, 1, 1);
        displayImage(frame, "processed video", false, false, false, true);
        bool detectStatus = cv::findChessboardCorners(frame, targetShape, cornersImage, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

        // std::cout << "\n pattern found : " << detectStatus;

        if (detectStatus == true)
        {
            cv::cornerSubPix(frame, cornersImage, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
        }

        // cv::drawChessboardCorners(frame, targetShape, cv::Mat(cornersImage), detectStatus);

        cv::Point2f drawPoint{};
        int numPoints{}, startPoint{3};
        numPoints = cornersImage.size();
        bool initPoint = true;
        std::vector<cv::Point2f> orientedCorners(54);

        cv::Mat cornersTemp(cornersImage);
        // std::map<cv::Point2f, cv::Vec3f>

        // for(size_t i = startPoint ; i < 49 ; i++){
        //         drawPoint = cornersImage.at(i);
        //         cv::circle(frame, drawPoint, 10, cv::Scalar(0, 0, 250), cv::LINE_8);
        // }

        userInput = cv::waitKey(1);
        if (userInput == 's')
        {
            std::printf("\nUSER GAVE INPUT AS %c\n", userInput);
        }
        // else {
        //     std::printf("\nUSER GAVE INPUT AS %c", userInput);
        // }

        if (detectStatus == true)
        {
            cv::Size gridSize{};
            gridSize = getGridDimsPix(cornersImage);
            // std::cout << "\n grid dims : \t" << gridSize;

            cv::circle(frame, cornersImage.at(21), 10, cv::Scalar(0, 0, 250), cv::LINE_8);
            cv::circle(frame, cornersImage.at(22), 10, cv::Scalar(0, 0, 250), cv::LINE_8);
            cv::circle(frame, cornersImage.at(30), 10, cv::Scalar(0, 0, 250), cv::LINE_8);

            cv::Mat vizCorners(cornersImage);
            // std::printf("user input : %c", userInput);
            if (userInput == 's')
            {
                worldCoordsList.push_back(worldCoords);
                imageCornersList.push_back(cornersImage);
                std::cout << "\nsaved calibration image and coords | total : " << worldCoordsList.size() << "\n";
                if (worldCoordsList.size() < minCalibPoints)
                {
                    std::printf("\nsaved calibration image and coords (%d / %d)", worldCoordsList.size(), minCalibPoints);
                    std::printf("\nenter %d more at least to calibrate camera", minCalibPoints - worldCoordsList.size());
                    cv::Mat temp(worldCoords);
                    printmat(temp, 3);
                }
                else
                {
                    cv::Mat cameraMat = cv::Mat::eye(3, 3, CV_64FC1);
                    cameraMat.at<double>(0, 2) = frame.cols / 2;
                    cameraMat.at<double>(1, 2) = frame.rows / 2;
                    double reProjError{};
                    std::vector<double> distortionErrors{};
                    cv::Mat rotationMat{};
                    cv::Mat translationMat{};
                    cv::Mat intrinsics{};
                    cv::Mat extrinsics{};
                    cv::Mat perViewErrors{};
                    reProjError = cv::calibrateCamera(worldCoordsList,
                                                      imageCornersList,
                                                      frame.size(),
                                                      cameraMat,
                                                      distortionErrors,
                                                      rotationMat,
                                                      translationMat,
                                                      cv::CALIB_FIX_ASPECT_RATIO);
                    std::printf("\nrotation mat");
                    printmat(rotationMat, INT_MAX);
                    std::printf("\ntranslation mat");
                    printmat(translationMat, INT_MAX);
                    std::printf("\ndistortion Errors");
                    myPrintVec(distortionErrors);
                    // printmat(distortionErrors, INT_MAX);
                    std::printf("\ncamera Matrix");
                    printmat(cameraMat, INT_MAX);
                    std::cout << "\n Reprojection Error " << reProjError; 

                    writeCameraParameters(reProjError,
                          cameraMat,
                          rotationMat,
                          translationMat,
                          distortionErrors);

                    // readCameraParameters(reProjError,
                    //       cameraMat,
                    //       rotationMat,
                    //       translationMat,
                    //       distortionErrors);


                }

                while (userInput == 's')
                {
                    std::cout << "\n\n press non 's' key to continue\n\n";
                    userInput = cv::waitKey(0);
                }
            }
        }

        // std::cout << "\t number of image corner Sets :\t" << imageCornersList.size() << "\t number of world Coords set :\t" << worldCoordsList.size();
        displayImage(frame, "processed video", false, false, false, true);

        cv::Mat tempWorldCoords(worldCoords);
        // printmat(tempWorldCoords, INT_MAX);

        // cv::waitKey(0);
    }
}