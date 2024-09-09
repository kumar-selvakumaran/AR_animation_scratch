/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : This is the script for running the final extension, that is the rendering of a sphere
 * bouncing on a checkboard marker.
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
    cv::VideoCapture inputVideo("../data/testvideo2.mp4");
    bool readStatus = inputVideo.isOpened();
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

    int numStitches = 10;
    int numVerticesPerStitch = 12;
    int radius = 20;

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
        if (detectStatus == true)
        {
            cv::cornerSubPix(frame,
                             cornersImage,
                             cv::Size(11, 11),
                             cv::Size(-1, -1),
                             cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER,
                                              30,
                                              0.1));
            // std::cout << "\n detected corners";
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

            cv::Mat tempFigureVertices{};
            figureVertices.copyTo(tempFigureVertices);

            
            // float minZRotationSpeed = 0.1f;
            float speed{}, direction{};
            rotationBounce(currTime, 0.1f, 0.01f, speed, direction);
            
            float angleY = speed * minYRotationSpeed;

            spinFigure(tempFigureVertices,
                   figMean,
                    0.0f,
                    angleY,
                    angleY,
                   -direction,
                   -direction,
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

            // printmat(tempFigureVertices, 5);
            
            if((int)currTime %2 == 0){
                std::printf("\nrotation speed : %f , direction : %f | translation X : %f Y : %f", speed, direction, displacementX, displacementY);
            }
            

            cv::projectPoints(
                tempFigureVertices,
                rotationMat,
                translationMat,
                cameraMat,
                distortionErrors,
                projectedPoints);


            cv::drawChessboardCorners(frame, targetShape, cv::Mat(cornersImage), detectStatus);

            drawFigure(frame,
            projectedPoints);

        }

        displayImage(frame, "processed video", false, false, false, true);

    }

    return 0;
}