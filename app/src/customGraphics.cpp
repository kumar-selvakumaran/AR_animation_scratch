
/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : This file contains all the graphical and animation components, be it classes, functions, etc.
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

/**
 * The function "degreesToRadians" converts an angle from degrees to radians.
 * 
 * @param degrees The parameter `degrees` is a floating-point number that represents the angle in degrees.
 * 
 * @return The function returns a floating-point number representing the angle in radians.
 */
float degreesToRadians(float degrees)
{
    return (degrees * M_PI / 180);
}

/**
 * Constructor for the `StitchedBall` class. Initializes a new instance of a stitched ball with a specific 
 * number of stitches, vertices per stitch, and radius. A stitch here is the atomic 2d regular polygon that will be used to 
 * generate by appending rotated versions.
 * 
 * @param numStitches The number of stitches the ball will have.
 * @param numVerticesPerStitch The number of vertices each stitch will contain.
 * @param radius The radius of the ball.
 */
StitchedBall::StitchedBall(int numStitches,
                           int numVerticesPerStitch,
                           int radius)
{
    this->numStitches = numStitches;
    this->numVerticesPerStitch = numVerticesPerStitch;
    this->radius = radius;
}

/**
 * The method "makeBall" populates a given cv::Mat object with vertices that represent a stitched ball, 
 * based on the previously defined parameters such as the number of stitches, vertices per stitch, and radius. 
 * It calculates the vertices for a single stitch and replicates it with appropriate rotations to simulate 
 * the full stitched ball. Additionally, it logs details about the process and the shapes created during the 
 * operation.
 * 
 * @param vertices A reference to a cv::Mat object that will be populated with the vertices of the stitched ball.
 *                 The method modifies this object to include the calculated vertices.
 */
void StitchedBall::makeBall(cv::Mat &vertices)
{
    int numVerticesPerStitch = this->numVerticesPerStitch;
    int numStitches = this->numStitches;
    int radius = this->radius;

    float angularInterval{(float)360 / numVerticesPerStitch};
    cv::Mat sampleStitch{};
    cv::Mat(cv::Size2i(3, numVerticesPerStitch + 1), CV_32FC1, cv::Scalar(0.0f)).copyTo(sampleStitch);
    std::cout << "\n##########\t MAKING BALL\t##########\n\n numPoints : " << numVerticesPerStitch << "\n drawPoinst shape : " << sampleStitch.size() << "\n";
    float tempX{}, tempY{};
    for (float i = 0; i < 361; i += angularInterval)
    {
        tempX = std::sin(degreesToRadians(i)) * radius;
        tempY = std::cos(degreesToRadians(i)) * radius;
        sampleStitch.at<cv::Point3f>((int)i / (int)angularInterval) = cv::Point3f(
            tempX,
            tempY,
            1.0f);
    }

    std::cout << "\n sample stitch : ";
    printmat(sampleStitch, INT_MAX);

    sampleStitch.copyTo(vertices);

    float stitchGap = 180.0f / numStitches;
    cv::Mat rotationMat{};

    for (float stitchAngle = 0.0f; stitchAngle < 180.0f; stitchAngle += stitchGap)
    {
        getRotationMat3dY(stitchAngle, 1.0f, rotationMat);
        cv::vconcat(vertices, sampleStitch * rotationMat, vertices);
    }
}

/**
 * The function `spinFigure` rotates a 3D figure around its center by given angles along the X, Y, and Z axes. 
 * The rotation is applied in the order of X, Y, and then Z axes rotations.
 *
 * @param vertices A reference to a cv::Mat object representing the vertices of the figure to be rotated.
 * @param figMean The 3D center point of the figure as a cv::Point3f.
 * @param angleX The rotation angle around the X-axis in degrees.
 * @param angleY The rotation angle around the Y-axis in degrees.
 * @param angleZ The rotation angle around the Z-axis in degrees.
 * @param directionX The direction of rotation around the X-axis.
 * @param directionY The direction of rotation around the Y-axis.
 * @param directionZ The direction of rotation around the Z-axis.
 */
void spinFigure(cv::Mat &vertices,
                cv::Point3f figMean,
                float angleX,
                float angleY,
                float angleZ,
                float directionX,
                float directionY,
                float directionZ)
{
    cv::Mat rotationMatX{}, rotationMatY{}, rotationMatZ{};

    getRotationMat3dY(angleX, directionX, rotationMatX);
    getRotationMat3dY(angleY, directionY, rotationMatY);
    getRotationMat3dZ(angleZ, directionZ, rotationMatZ);

    vertices = vertices * rotationMatX * rotationMatY * rotationMatZ;

}

/**
 * The function `getRotationMat3dX` calculates the rotation matrix for a rotation around the X-axis.
 *
 * @param rotationAngle The angle of rotation in degrees.
 * @param direction The direction of the rotation. A positive value rotates in one direction, 
 * a negative value in the opposite.
 * @param rotationMat A reference to a cv::Mat object where the rotation matrix will be stored.
 */
void getRotationMat3dX(float rotationAngle,
                       float direction,
                       cv::Mat &rotationMat)
{
    cv::Mat(cv::Size(3, 3), CV_32F, cv::Scalar(0.0f)).copyTo(rotationMat);
    rotationMat.at<float>(0, 0) = 1.0f;
    rotationMat.at<float>(0, 1) = 0.0f;
    rotationMat.at<float>(0, 2) = 0.0f;

    rotationMat.at<float>(1, 0) = 0.0f;
    rotationMat.at<float>(1, 1) = std::cos(degreesToRadians(rotationAngle));
    rotationMat.at<float>(1, 2) = -std::sin(degreesToRadians(rotationAngle)) * direction;

    rotationMat.at<float>(2, 0) = 0.0f;
    rotationMat.at<float>(2, 1) = std::sin(degreesToRadians(rotationAngle)) * direction;
    rotationMat.at<float>(2, 2) = std::cos(degreesToRadians(rotationAngle));
}

/**
 * The function `getRotationMat3dY` calculates the rotation matrix for a rotation around the Y-axis.
 *
 * @param rotationAngle The angle of rotation in degrees.
 * @param direction The direction of the rotation. A positive value rotates in one direction,
 * a negative value in the opposite.
 * @param rotationMat A reference to a cv::Mat object where the rotation matrix will be stored.
 */
void getRotationMat3dY(float rotationAngle,
                       float direction,
                       cv::Mat &rotationMat)
{
    cv::Mat(cv::Size(3, 3), CV_32F, cv::Scalar(0.0f)).copyTo(rotationMat);
    rotationMat.at<float>(0, 0) = std::cos(degreesToRadians(rotationAngle));
    rotationMat.at<float>(0, 1) = 0.0f;
    rotationMat.at<float>(0, 2) = -std::sin(degreesToRadians(rotationAngle)) * direction;

    rotationMat.at<float>(1, 0) = 0.0f;
    rotationMat.at<float>(1, 1) = 1.0f;
    rotationMat.at<float>(1, 2) = 0.0f;

    rotationMat.at<float>(2, 0) = std::sin(degreesToRadians(rotationAngle)) * direction;
    rotationMat.at<float>(2, 1) = 0.0f;
    rotationMat.at<float>(2, 2) = std::cos(degreesToRadians(rotationAngle));
}

/**
 * The function `getRotationMat3dZ` calculates the rotation matrix for a rotation around the Z-axis.
 *
 * @param rotationAngle The angle of rotation in degrees.
 * @param direction The direction of the rotation. A positive value rotates in one direction,
 * a negative value in the opposite.
 * @param rotationMat A reference to a cv::Mat object where the rotation matrix will be stored.
 */
void getRotationMat3dZ(float rotationAngle,
                       float direction,
                       cv::Mat &rotationMat)
{
    cv::Mat(cv::Size(3, 3), CV_32F, cv::Scalar(0.0f)).copyTo(rotationMat);
    rotationMat.at<float>(0, 0) = std::cos(degreesToRadians(rotationAngle));
    rotationMat.at<float>(0, 1) = -std::sin(degreesToRadians(rotationAngle)) * direction;
    rotationMat.at<float>(0, 2) = 0.0f;

    rotationMat.at<float>(1, 0) = std::sin(degreesToRadians(rotationAngle)) * direction;
    rotationMat.at<float>(1, 1) = std::cos(degreesToRadians(rotationAngle));
    rotationMat.at<float>(1, 2) = 0.0f;

    rotationMat.at<float>(2, 0) = 0.0f;
    rotationMat.at<float>(2, 1) = 0.0f;
    rotationMat.at<float>(2, 2) = 1.0f;
}


/**
 * The function `rotationBounce` calculates the speed and direction of a bouncing rotation based on the 
 * current time, applying a resistance factor to control rate of bounce growth over time. The bounce grows as 
 * a quadratic function of time.
 *
 * @param currTime The current time or frame in the animation sequence.
 * @param timeScale A scaling factor to control the speed of the bounce.
 * @param resistance A resistance factor to simulate the decay of the bounce over time.
 * @param speed A reference to a float variable where the calculated speed will be stored.
 * @param direction A reference to a float variable where the calculated direction (-1 or 1) will be stored.
 */
void rotationBounce(float currTime,
                            float timeScale,
                            float resistance,
                            float &speed,
                            float &direction)
{
    currTime *= timeScale;
    float bounceNumber = std::floor(currTime / 10);
    // std::cout << "bounce number " << bounceNumber << "\n";
    float x = currTime - (bounceNumber * 10);
    direction = -1.0f + (2.0f * (float)(1 - (int)bounceNumber % 2));
    // float speed = - (0.30f*((x-5.0f)*(x-5.0f))) + 8; touch up parabola
    // float speed = - (0.6f*((x-5.0f)*(x-5.0f))); //touch down parabola
    speed = x * x / resistance;

    // std::printf("direction : %f, speed : %f\n", direction, speed);
}

/**
 * The function `Bounce2d100mm` simulates a 2D bounce using a parabolic function within a 100mm range, calculating displacement in 
 * both X and Y directions based on the current time and initial speed.
 *
 * @param displacementX A reference to a float variable where the X displacement will be stored.
 * @param displacementY A reference to a float variable where the Y displacement, simulating gravity, will be stored.
 * @param speedX The initial speed in the X direction.
 * @param currTime The current time or frame in the bounce animation.
 */
void Bounce2d100mm(float &displacementX,
                   float &displacementY,
                   float speedX,
                   float currTime)
{
    float bounceNumber = std::floor((currTime * speedX) / 100.f);
    int toOrFro = ((int)bounceNumber) % 2;
    float direction = -1.0f + (2.0f * (float)(1 - toOrFro));
    displacementX = ((currTime * speedX) / 100.0f) - bounceNumber;
    displacementX = ((float)toOrFro * 100.f) + (direction * displacementX * 100);
    displacementY = -(((displacementX - 50.0f) * (displacementX - 50.0f)) / 41.9f) + 60.0f;
}


/**
 * The function `drawFigure` draws the figure represented by the vertices on a visualization image line by line with
 * each vertice drawn as a circle as well.
 *
 * @param vizIm A reference to a cv::Mat object representing the visualization image where the figure will be drawn.
 * @param figVertices A cv::Mat object containing the vertices of the figure to be drawn.
 */
void drawFigure(cv::Mat &vizIm,
                cv::Mat figVertices)
{
    // printmat(figVertices, INT_MAX);
    cv::cvtColor(vizIm, vizIm, cv::COLOR_GRAY2BGR);
    for (int i = 1; i < figVertices.rows; i++)
    {
        cv::line(
            vizIm,
            figVertices.at<cv::Point2f>(i),
            figVertices.at<cv::Point2f>(i - 1),
            cv::Scalar(250, 0, 250),
            2,
            3);

        cv::circle(
            vizIm,
            figVertices.at<cv::Point2f>(i),
            1,
            cv::Scalar(0, 0, 250),
            2,
            2);
    }
    return;
}