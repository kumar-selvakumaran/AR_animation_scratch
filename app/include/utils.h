/**
 * Names : Kumar Selvakumaran,  Adnan Amir,
 * date : 2/27/2024
 * Purpose : Contains the headers of all the code 
 * code for general functions for debugging and visualization other miscellaneous purposes .
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

#ifndef UTILS_H
#define UTILS_H

typedef int (* frameProcessor) (cv::Mat &mat1, cv::Mat &mat2);


/**
 * The function "myPrintVec" prints the elements of a vector to the standard output.
 * 
 * @param vec A reference to a std::vector object containing elements of type T.
 */
template<typename T>
void myPrintVec(std::vector<T> &vec) {
    std::cout << "\t[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\t" << std::endl;
}

/**
 * The template function "sortIndices" sorts the indices of a vector based on the corresponding
 * values in the vector in ascending order by default or descending order if specified.
 * 
 * @tparam T The template parameter `T` represents the data type of the elements in the vector.
 * 
 * @param array A reference to the vector whose indices are to be sorted.
 * 
 * @param descending A boolean value indicating the sorting order. 
 * If `false` (default), sorting is done in ascending order. 
 * If `true`, sorting is done in descending order.
 * 
 * @return A vector of size_t representing the sorted indices of the input vector.
 */
template <typename T>
std::vector<size_t> sortIndices(std::vector<T> &array, bool descending = false) {
    std::vector<size_t> indices(array.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::stable_sort(indices.begin(),
                    indices.end(),
                    [&array](size_t val1, size_t val2){return array[val1] < array[val2];}
                    );
    if(descending){
        reverse(indices.begin(), indices.end());
    }
    return indices;
}


/**
 * The class `VideoOps` provides functionality for processing videos, including reading video files,
 * extracting frames, and performing operations on frames.
 */
class VideoOps
{   
    public:
        std::string vidPath;
        cv::VideoCapture *inputVideo;
        bool readStat;
        double vidFrameCount;
        double vidFrameRate;
        double vidDuration;
        double vidDims[2];

    public:
        // Constructor to initialize the different member variables.
        VideoOps(std::string vidPath);
        int processVideo(bool playVideo, bool saveFrames, std::string outputDir, int saveWait, bool saveVideo, frameProcessor processor);
        void printDetails();
};

std::string myMatType(cv::Mat &src);

void printmat(cv::Mat &src, int vizdim);

int earliestDecPos(double num);

cv::Mat makeHist(cv::Mat &src, int numBins);

void vizHist(cv::Mat hist, int numBins);

void displayImage(cv::Mat &img, std::string windowName, bool normalize, bool onlyPositive, bool printDetails, bool videoMode);

int makeTestSeqMat(cv::Mat &testMat, bool onlyRows, bool onlyCols);

int splitString(std::string src, char delimiter, std::vector<std::string> &splits);

int getFileName(std::string filePath, std::string &fileName);

void drawPreds(cv::Mat srcog, cv::RotatedRect rotatedRect, std::string label, std::string windowName, std::string saveName);

bool file_exists(std::string name);

#endif // UTILS_H

