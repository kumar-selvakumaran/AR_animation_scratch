/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : This is the script used reprojecting and visualizing objects present in realworld 3d coordinates
 * onto an image.
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
#include <customGraphics.h>

using std::string_literals::operator""s;

int main()
{
    // cv::VideoCapture inputVideo("http://10.0.0.111:8080/video");
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
    
    float minYRotationSpeed = 0.1f;
    
    cv::Mat rotationMatX{}, rotationMatY{}, rotationMatZ{};


    cv::Mat figureVertices{};

    int numStitches = 4;
    int numVerticesPerStitch = 6;
    int radius = 10;

    StitchedBall ball3d(numStitches,
                        numVerticesPerStitch,
                        radius);

    ball3d.makeBall(figureVertices);

    
    cv::Scalar meanX(cv::mean(figureVertices.rowRange(0, figureVertices.rows - 1).col(0)));
    cv::Scalar meanY(cv::mean(figureVertices.rowRange(0, figureVertices.rows - 1).col(1)));
    cv::Scalar meanZ(cv::mean(figureVertices.rowRange(0, figureVertices.rows - 1).col(2)));

    cv::Point3f figMean(
        meanX[0],
        meanY[0],
        meanZ[0]);



    for (float currTime = 0.0f;; ++currTime)
    {
        
        cv::Mat tempVizImg(cv::Size(600,600), CV_8UC1, cv::Scalar(0));
        inputVideo >> frame;
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        cv::resize(frame, frame, cv::Size(1000, 600), 1, 1, 1);
        bool detectStatus = cv::findChessboardCorners(frame, targetShape, cornersImage, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);
        // std::cout << "\n detected chessboadrd corners\n";
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

            const float figureDefaultX = 40.0f;
            const float figureDefaultY = -60.0f;  
            const float figureDefaultZ = -0.0f;  

            cv::Mat projectedPoints{};

            std::vector<cv::Point3f> tempPoints{};


            // 4 BASIS POINTS ----------- TASK 5
            // tempPoints.push_back(cv::Point3f(0, 0, 0));
            // tempPoints.push_back(cv::Point3f(0, -100, 0));
            // tempPoints.push_back(cv::Point3f(160, 0, 0));
            // tempPoints.push_back(cv::Point3f(160, -100, 0));
    

            // 3 AXES   ----------- TASK 5
            // tempPoints.push_back(cv::Point3f(0, 0, 0));
            // tempPoints.push_back(cv::Point3f(40, 0, 0));
            // tempPoints.push_back(cv::Point3f(0, -40, 0));
            // tempPoints.push_back(cv::Point3f(0, 0, 40));
            
            //xxxxxxxxxxxxxxxxxxxxxx    FINAL EXTENSION     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
            cv::Mat tempFigureVertices{};
            figureVertices.copyTo(tempFigureVertices);

            
            // float minZRotationSpeed = 0.1f;
            float speed{}, direction{};
            rotationBounce(currTime, 0.1f, 0.009f, speed, direction);
            
            float angleY = speed * minYRotationSpeed;

            spinFigure(tempFigureVertices,
                   figMean,
                    // 1.0f * currTime,
                    1.0f*currTime,
                    1.0f*currTime,
                    angleY,
                    // 0.0f,
                    // 1.0f * currTime,
                    // 0.0f,
                   direction,
                   direction,
                   -direction);
                
            float displacementX{}, displacementY{}, displacementZ{};
            
            float translationSpeedX = 1.0f;
            Bounce2d100mm(displacementX,
                        displacementY,
                        translationSpeedX,
                        currTime);
                
            tempFigureVertices.col(0) += figureDefaultX + displacementX;
            tempFigureVertices.col(1) += figureDefaultY + displacementY;
            tempFigureVertices.col(2) += figureDefaultZ + displacementZ;
            
            if((int)currTime %2 == 0){
                std::printf("\nrotation speed : %f , direction : %f | translation X : %f Y : %f", speed, direction, displacementX, displacementY);
            }

            // printmat(tempFigureVertices, 5);
            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


            // cv::Mat worldCoordVertices2d{};
            // cv::Mat(tempFigureVertices.colRange(0,2)).copyTo(worldCoordVertices2d);

            // std::cout << "\n world coords 2d\n";
            // printmat(worldCoordVertices2d, 3);
            // std::cout << "\n  viz image \n";
            // printmat(tempVizImg, 3);
            // drawFigure(tempVizImg, worldCoordVertices2d);
            // std::cout << "\n DREW FIGURE \n";
            // displayImage(tempVizImg, "world coord animation" , false, false, false, true);

            std::vector<cv::Scalar> colors({cv::Scalar(0, 0, 255),
                                            cv::Scalar(0, 255, 0),
                                            cv::Scalar(255, 0, 0)});


            cv::projectPoints(
                // tempFigureVertices,
                tempPoints,
                rotationMat,
                translationMat,
                cameraMat,
                distortionErrors,
                projectedPoints);

            cv::Mat projectedCorners{};
            cv::projectPoints(
                worldCoordsMat,
                rotationMat,
                translationMat,
                cameraMat,
                distortionErrors,
                projectedCorners);

            // cv::drawChessboardCorners(frame, targetShape, cv::Mat(cornersImage), detectStatus);
            cv::drawChessboardCorners(frame, targetShape, projectedCorners, detectStatus);

            // cv::Size gridSize{};
            // gridSize = getGridDimsPix(cornersImage);

            // cv::circle(frame, cornersImage.at(21), 10, cv::Scalar(0, 0, 250), cv::LINE_8);
            // cv::circle(frame, cornersImage.at(22), 10, cv::Scalar(0, 0, 250), cv::LINE_8);
            // cv::circle(frame, cornersImage.at(30), 10, cv::Scalar(0, 0, 250), cv::LINE_8);

            // cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);

            
            //######### for STITCHED BALL ONLY ############3
            std::cout << "\nframe";
            printmat(frame, 3);
            std::cout << "\n projectedPoints";
            printmat(projectedPoints, 3);
            drawFigure(frame,
            projectedPoints);
            //###############################################

            // cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
            // for (int i = 0; i < projectedPoints.rows; i++)
            // {
            //     // blue-red sine colour fade
            //     cv::circle(frame,
            //                 projectedPoints.at<cv::Point2f>(i),
            //                 5,
            //                 cv::Scalar((std::cos(currTime * 0.03f) * 14) * (std::cos(currTime * 0.03f) * 14),
            //                             0,
            //                             (std::sin(currTime * 0.03f) * 14) * (std::sin(currTime * 0.03f) * 14)),
            //                 cv::LINE_4);

            //     // draw axes TASK 5
            //     // cv::arrowedLine(frame,
            //     //                 projectedPoints.at<cv::Point2f>(0),
            //     //                 projectedPoints.at<cv::Point2f>(i),
            //     //                 colors.at(i%3),
            //     //                 4,
            //     //                 cv::LINE_8
            //     //                 );
            // }


        // std::cout << "\n came to display image\n";
        cv::resize(frame, frame, cv::Size(1400, 800), 1, 1, 1);
        displayImage(frame, "processed video", false, false, false, true);
        // printmat(frame, 2);
        // std::cout << "\n crossed display image\n";
        // cv::Mat tempWorldCoords(worldCoords);
        }
    }

    return 0;
}