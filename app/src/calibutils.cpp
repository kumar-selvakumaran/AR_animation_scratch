/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : Contains the relevant functions for camera calibration.
 */

#include <opencv2/core/core.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include <utils.h>

using std::string_literals::operator""s;

/**
 * The function "getGridDimsPix" calculates the grid dimensions in pixels based on the distance between 
 * specified reference points within a vector of corner points in an image. The grid dimensions are determined 
 * by calculating the Euclidean distance between the reference point and its horizontal and vertical neighbours.
 * This function is particularly useful in scenarios where the grid layout of an image is analyzed, such as 
 * in image processing applications where spatial relationships are important.
 * 
 * @param cornersImage A vector of cv::Point2f that contains the corner points of an image. It is assumed that
 * the points necessary for calculating the grid dimensions are properly indexed within this vector.
 * 
 * @return cv::Size An object of cv::Size containing the width and height of the grid in pixels. The width and height
 * are calculated as the Euclidean distance between the reference point and its immediate horizontal and vertical
 * neighbours, respectively.
 */
cv::Size getGridDimsPix(std::vector<cv::Point2f> cornersImage){
    cv::Mat referencePoint(cornersImage.at(21));
    cv::Mat horizontalNeighbour(cornersImage.at(22));
    cv::Mat vecticalNeighbour(cornersImage.at(30));
    // cv::Mat checkWidth = (origin - horizontalNeighbour);
    int gridWidth = cv::norm(referencePoint-horizontalNeighbour);
    int gridHeight = cv::norm(referencePoint-vecticalNeighbour);
    cv::Size gridDims(gridWidth, gridHeight);
    return gridDims;
}

/**
 * The function "getWorldCoords" initializes the vector "worldCoords" with 3D world coordinates
 * for a grid of points. The grid size is determined by the number of horizontal (9) and vertical (6)
 * positions, resulting in a total of 54 points. Each point is represented as a 3D vector with x, y, and z
 * coordinates. The x and y coordinates can be scaled to millimeters based on the "inMM" flag and the
 * "gridToMM" scaling factor. By default, coordinates are provided in grid units.
 * 
 * @param worldCoords A reference to a vector of 3D vectors (cv::Vec3f) that will be filled with the
 * world coordinates of the grid points.
 * @param inMM A boolean flag indicating whether the coordinates should be in millimeters (true) or
 * grid units (false). The default value is false.
 * @param gridToMM An integer that represents the scaling factor to convert grid units to millimeters.
 * The default value is 20.
 */
void getWorldCoords(std::vector<cv::Vec3f> &worldCoords, bool inMM = false, int gridToMM = 20){
    worldCoords = std::vector<cv::Vec3f> (54);
    int numHorizontal{9}, numVertical{6};
    for(float i = 0.0f ; i < numVertical ; i++){
        for(float j = 0.0f ; j < numHorizontal ; j++){
            if(inMM == false){
                worldCoords.at(i*numHorizontal + j) = cv::Vec3f(j, -i, 0.0f);
            } else {
                worldCoords.at(i*numHorizontal + j) = cv::Vec3f(j * gridToMM, -i * gridToMM, 0.0f);
            }
        }
    }
}


/**
 * The function "writeCameraParameters" writes calibration data including camera matrix, distortion errors,
 * rotation and translation matrices to a CSV file. It first writes the reProjectionError, then iterates through
 * the cameraMatrix to write its values, followed by the distortionErrors, and finally writes the rotation and
 * translation matrices parameters for each calibration.
 * 
 * @param reProjectionError A double value representing the re-projection error.
 * @param cameraMatrix A cv::Mat object representing the camera matrix.
 * @param rotationMatrix A cv::Mat object representing the rotation matrix.
 * @param translationMatrix A cv::Mat object representing the translation matrix.
 * @param distortionErrors A std::vector<double> containing distortion errors.
 * 
 * @return An integer value. Returns 0 to indicate successful execution.
 */
int writeCameraParameters(double reProjectionError,
                          cv::Mat cameraMatrix,
                          cv::Mat rotationMatrix,
                          cv::Mat translationMatrix,
                          std::vector<double> distortionErrors)
{
    std::string file{"../data/calibrationData.csv"s};
    
    // bool csvExists = file_exists(file);

    std::string seperatorToken{"<sep>"};
    std::fstream db{};
    // char dumpChar{};
    db.open(file.c_str(), std::fstream::trunc | std::fstream::out);
    int numCalibrations = rotationMatrix.rows;
    // std::printf("\n writing %d calibration parameters\n", numCalibrations);

    db << reProjectionError << ',' << std::endl; 
    
    for (int i = 0; i < cameraMatrix.rows; i++)
        for (int j = 0; j < cameraMatrix.cols; j++)
        {
            db << cameraMatrix.at<double>(i, j) << ','; 
        }
    
    db << std::endl;


    for(double distortionError : distortionErrors){
        db << distortionError << ',';
    }

    db << std::endl;

    for (int i = 0; i < numCalibrations; i++)
    {
        std::vector<cv::Point3d> parameters{rotationMatrix.at<cv::Point3d>(i),
                                            translationMatrix.at<cv::Point3d>(i)};
        for (cv::Point3d parameter : parameters)
        {
            db << parameter.x << ',' << parameter.y << ',' << parameter.z << ',' << seperatorToken << ",";
        }
        db << std::endl;
    }

    db.close();
    return 0;
}

/**
 * The function "readCameraParameters" reads camera calibration parameters from a CSV file and assigns them to the respective
 * parameters passed by reference. It extracts the re-projection error, camera matrix, rotation matrix, translation matrix,
 * and distortion errors. These parameters are crucial for camera calibration and 3D reconstruction tasks.
 *
 * @param reProjectionError Reference to a double where the re-projection error will be stored.
 * @param cameraMatrix Reference to a cv::Mat object where the camera matrix will be stored. The camera matrix includes
 * intrinsic parameters of the camera.
 * @param rotationMatrix Reference to a cv::Mat object where the rotation matrix will be stored. The rotation matrix
 * represents the rotation of the camera with respect to the world coordinates.
 * @param translationMatrix Reference to a cv::Mat object where the translation matrix will be stored. The translation matrix
 * represents the translation of the camera with respect to the world coordinates.
 * @param distortionErrors Reference to a vector of doubles where the distortion errors (coefficients) will be stored. Distortion
 * errors are used to correct radial and tangential distortions in the camera lens.
 *
 * @return Returns 0 upon successful completion of reading and storing all parameters.
 */

int readCameraParameters(double &reProjectionError,
                          cv::Mat &cameraMatrix,
                          cv::Mat &rotationMatrix,
                          cv::Mat &translationMatrix,
                          std::vector<double> &distortionErrors)
{
    std::string file{"../data/calibrationData.csv"s};

    std::fstream db{};
    db.open(file.c_str(), std::fstream::in);
    std::string line{};
    std::getline(db, line);
    reProjectionError = std::stod(line);

    std::vector<std::string> splitstr{};
    std::vector<double> tempMat{};

    std::getline(db, line);
    splitString(line, ',', splitstr);
    for(std::string parameter : splitstr){
        tempMat.push_back(std::stod(parameter));
    }

    cv::Mat(tempMat).copyTo(cameraMatrix);
    cameraMatrix = cameraMatrix.reshape(1, std::vector<int>{3, 3});
    distortionErrors.clear();
    std::getline(db, line);
    splitString(line, ',', splitstr);
    for(std::string parameter : splitstr){
        distortionErrors.push_back(std::stod(parameter));
    }

    std::vector<cv::Point3d> tempRotationMat{}, tempTranslationMat{};

    while(getline(db, line)){
        splitString(line,',', splitstr);
        cv::Point3d tempPoint{
            std::stod(splitstr[0]),
            std::stod(splitstr[1]),
            std::stod(splitstr[2])
        };

        tempRotationMat.push_back(tempPoint);

        tempPoint.x = std::stod(splitstr[4]);
        tempPoint.y = std::stod(splitstr[5]);
        tempPoint.z = std::stod(splitstr[6]);

        tempTranslationMat.push_back(tempPoint);

    }

    cv::Mat(tempRotationMat).copyTo(rotationMatrix);
    cv::Mat(tempTranslationMat).copyTo(translationMatrix);

    db.close(); 
    return 0;
}
