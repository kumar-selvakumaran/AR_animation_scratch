/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : This is the script used reprojecting and visualizing z-axis perspectives of objects 
 * on a black background.
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
#include <math.h>

#include <utils.h>
#include <customGraphics.h>
// #include <calibutils.h>

using std::string_literals::operator""s;

int main()
{

    cv::Mat testim = cv::Mat::zeros(800, 1400, CV_32FC1);

    StitchedBall ball3d(16,
                        20,
                        240);
                

    cv::Mat drawPoints{};

    ball3d.makeBall(drawPoints);

    testim.convertTo(testim, CV_8U);

    char c = ' ';

    cv::Mat rotationMatX{}, rotationMatY{}, rotationMatZ{};

    for (int i = 0; c != 'x'; i++)
    {
        // for(int ballNumber = 0 ; ballNumber < )
            spinFigure(drawPoints,
                    cv::Point3f(0, 0, 0),
                    0.001f * 50,
                    0.001f * 50,
                    0.001f * 50,
                    1.0f,
                    1.0f,
                    1.0f);


            cv::Mat vizIm{};
            testim.copyTo(vizIm);

            cv::Mat tempPoints{};
            cv::Mat(drawPoints.colRange(0, 2)).copyTo(tempPoints);

            const float figureDefaultX = 700.0f;
            const float figureDefaultY = 400.0f;

            tempPoints.col(0) += figureDefaultX;
            tempPoints.col(1) += figureDefaultY;

            drawFigure(vizIm, tempPoints);

        displayImage(vizIm, "testimg", false, false, false, true);
    }

    return 0;
}