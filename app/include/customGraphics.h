/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : This file contains the headers of all the graphical and animation components
 * be it classes, functions, etc.
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <map>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>


#ifndef CUSTOMGRAPHICS_H
#define CUSTOMGRAPHICS_H

// typedef int (* frameProcessor) (cv::Mat &mat1, cv::Mat &mat2);


class StitchedBall
{
    private:    
        int numStitches{};
        int numVerticesPerStitch{};
        int radius{};

    public:
        StitchedBall(int numStitches,
                    int numVerticesPerStitch,
                    int radius);

        void makeBall(cv::Mat &vertices);
};

float degreesToRadians(float degrees);


void getRotationMat3dX(float rotationAngle,
                      float direction,
                      cv::Mat &rotationMat);

void getRotationMat3dY(float rotationAngle,
                      float direction,
                      cv::Mat &rotationMat);

void getRotationMat3dZ(float rotationAngle,
                      float direction,
                      cv::Mat &rotationMat);

void drawFigure(cv::Mat &vizIm,
                cv::Mat figVertices);
                
void rotationBounce(float currTime,
                    float timeScale,
                    float resistance,
                    float &speed,
                    float &direction);

void Bounce2d100mm(float &displacementX,
                   float &displacementY,
                   float speedX,
                   float currTime);

void spinFigure(cv::Mat &vertices,
                cv::Point3f figMean,
                float angleX,
                float angleY,
                float angleZ,
                float directionX,
                float directionY,
                float directionZ
                );

#endif // CUSTOMGRAPHICS_H

