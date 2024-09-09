/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : Contains the headers of relevant functions for camera calibration.
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

#ifndef CALIBUTILS_H
#define CALIBUTILS_H

cv::Size getGridDimsPix(std::vector<cv::Point2f>);
void getWorldCoords(std::vector<cv::Vec3f> &worldCoords, bool inMM = false, int gridToMM = 20);

int writeCameraParameters(double reProjectionError,
                          cv::Mat cameraMatrix,
                          cv::Mat rotationMatrix,
                          cv::Mat translationMatrix,
                          std::vector<double> distortionErrors);

int readCameraParameters(double &reProjectionError,
                          cv::Mat &cameraMatrix,
                          cv::Mat &rotationMatrix,
                          cv::Mat &translationMatrix,
                          std::vector<double> &distortionErrors);

#endif // CALIBUTILS_H

