/**
 * Names : Kumar Selvakumaran
 * date : 3/18/2024
 * Purpose : Contains the code for general functions for debugging and
 * visualization other miscellaneous purposes .
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

/**
 * The typedef `frameProcessor` represents a function pointer type used for
 * processing frames. It expects two `cv::Mat` objects as input parameters and
 * returns an integer.
 */
typedef int (*frameProcessor)(cv::Mat &mat1, cv::Mat &mat2);

/**
 * The constructor `VideoOps` initializes a VideoOps object with the provided
 * video path. If the video path is "0", it initializes the VideoCapture with
 * the default camera index (0). Otherwise, it initializes the VideoCapture with
 * the provided video path using FFMPEG backend.
 *
 * @param vidPath The path to the video file or camera index (0 for default
 * camera).
 */
VideoOps::VideoOps(std::string vidPath)
{
    if (vidPath == "0")
        this->inputVideo = new cv::VideoCapture(0);
    else
        this->inputVideo = new cv::VideoCapture(vidPath, cv::CAP_FFMPEG);

    this->vidPath = vidPath;

    this->readStat = inputVideo->isOpened();

    this->vidFrameCount = inputVideo->get(cv::CAP_PROP_FRAME_COUNT);

    this->vidFrameRate = inputVideo->get(cv::CAP_PROP_FPS);

    this->vidDuration = static_cast<double>(vidFrameCount) / vidFrameRate;

    this->vidDims[0] = inputVideo->get(cv::CAP_PROP_FRAME_WIDTH);
    this->vidDims[1] = inputVideo->get(cv::CAP_PROP_FRAME_HEIGHT);
}

/**
 * The function `printDetails` prints various details of a video.
 */
void VideoOps::printDetails()
{
    std::cout << "\n vid path : " << vidPath;
    std::cout << "\n read status : " << readStat;
    std::cout << "\n frames per second : " << vidFrameCount;
    std::cout << "\n frame rate : " << vidFrameRate;
    std::cout << "\n video duration : " << vidDuration;
    std::cout << "\n video dims : (" << vidDims[0] << ", " << vidDims[1] << ")\n";
}

/**
 * The method `processVideo` processes a video by apply operations defind by a
 * frameprocesor object, optionally playing it, saving frames and/or saving the
 * processed video.
 *
 * @param playVideo A boolean indicating whether to display the processed video.
 *
 * @param saveFrames A boolean indicating whether to save individual frames of
 * the video.
 *
 * @param outputDir A string representing the directory where the frames will be
 * saved.
 *
 * @param saveWait An integer specifying the frequency of frame saving.
 *
 * @param saveVideo A boolean indicating whether to save the processed video.
 *
 * @param processor A function pointer that takes in a cv::Mat frame and
 * processes it.
 *
 * @return An integer indicating the success of the video processing operation.
 *         Returns 0 upon successful completion.
 */

int VideoOps::processVideo(bool playVideo, bool saveFrames,
                           std::string outputDir, int saveWait, bool saveVideo,
                           frameProcessor processor)
{
    cv::Mat frame;
    std::string outpath_suffix{".jpg"};
    int frameno{1};

    // int count{1};
    int saveWaiter = saveWait;

    std::string::size_type extension =
        vidPath.find_last_of('.'); // Find extension point
    std::cout << "\n extension : " << extension;

    std::string fileName;
    getFileName(vidPath, fileName);

    // char choice= 'R';
    const std::string outputVideoPath =
        "../data/videoDataOut/" + fileName + "_pred" +
        ".mp4"; // Form the new name with container
    int ex = static_cast<int>(
        inputVideo->get(cv::CAP_PROP_FOURCC)); // Get Codec Type- Int form
    // Transform from int to char via Bitwise operators
    // char EXT[] = {(char)(ex & 0XFF), (char)((ex & 0XFF00) >> 8),
    //               (char)((ex & 0XFF0000) >> 16), (char)((ex & 0XFF000000) >> 24),
    //               0};
    cv::Size outPutVideoSize = cv::Size(
        (int)inputVideo->get(cv::CAP_PROP_FRAME_WIDTH), // Acquire input size
        (int)inputVideo->get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter outputVideo;

    outputVideo.open(outputVideoPath, ex, inputVideo->get(cv::CAP_PROP_FPS),
                     outPutVideoSize, true);

    if (!outputVideo.isOpened())
    {
        std::cout << "Could not open the output video for write: "
                  << outputVideoPath << "\n";
        return -1;
    }

    cv::namedWindow("processed", 1);
    for (;;)
    {
        *inputVideo >> frame;

        // std::cout << "\nframe number : "<< frameno << " dims : " << frame.dims <<
        // " frame size : " << frame.size(); printmat(frame, 3);

        if (frame.empty())
        {
            printf("frame is empty\n");
            break;
        }

        processor(frame, frame);

        if ((saveFrames == true) && (saveWaiter < 0))
        {
            std::string savePath;
            savePath =
                outputDir + fileName + "_" + std::to_string(frameno) + outpath_suffix;
            cv::imwrite(savePath, frame);
            saveWaiter = saveWait;
            frameno++;
        }

        if (saveVideo == true)
        {
            outputVideo << frame;
        }

        if (playVideo == true)
        {
            cv::waitKey(5);
            cv::imshow("processed", frame);
        }

        saveWaiter--;
    };

    return 0;
}
/**
 * The function "myMatType" returns a string representation of the type of a
 * given OpenCV Mat object.
 *
 * @param src The input cv::Mat object whose type needs to be determined.
 *
 * @return A string representing the type of the input Mat object.
 */
std::string myMatType(cv::Mat &src)
{
    int type = src.type();
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth)
    {
    case CV_8U:
        r = "8U";
        break;
    case CV_8S:
        r = "8S";
        break;
    case CV_16U:
        r = "16U";
        break;
    case CV_16S:
        r = "16S";
        break;
    case CV_32S:
        r = "32S";
        break;
    case CV_32F:
        r = "32F";
        break;
    case CV_64F:
        r = "64F";
        break;
    default:
        r = "User";
        break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

/**
 * The function "printmat" prints a specified chunk of a matrix along with
 * additional information.
 *
 * @param src The input matrix whose chunk needs to be printed.
 *
 * @param vizdim An integer representing the dimension of the chunk to be
 * visualized.
 */
void printmat(cv::Mat &src, int vizdim)
{
    int xMax = std::min(90, src.rows);
    int xMin = xMax - std::min(vizdim, src.rows);
    int yMax = std::min(90, src.cols);
    int yMin = yMax - std::min(vizdim, src.cols);
    cv::Mat vizslice(src(cv::Range(xMin, xMax), cv::Range(yMin, yMax)));
    std::cout << "\nmatrix chunk : \n"
              << format(vizslice, cv::Formatter::FMT_NUMPY);
    double minVal;
    double maxVal;
    cv::minMaxLoc(src, &minVal, &maxVal);
    std::cout << "\nchannels : " << src.channels()
              << " type : " << myMatType(src);
    std::cout << " Min val : " << minVal << " Max val : " << maxVal;
    std::cout << " rows : " << src.rows << " cols : " << src.cols << "\n";
}

/**
 * The function "myNormMat" normalizes the values in the input matrix to the
 * range [0, 1].
 *
 * @param src The input matrix whose values are to be normalized.
 *
 * @param dst The output matrix where the normalized values will be stored.
 *
 * @return A std::pair<double, double> containing the minimum and maximum values
 * of the input matrix before normalization.
 */
std::pair<double, double> myNormMat(cv::Mat &src, cv::Mat &dst)
{
    double minVal;
    double maxVal;
    cv::minMaxLoc(src, &minVal, &maxVal);
    dst = src.clone();
    if (maxVal != minVal)
    {
        dst = (dst - minVal) / (maxVal - minVal);
    }
    std::pair<double, double> pars(minVal, maxVal);
    return pars;
}

/**
 * The function "myNormVec" normalizes the values in the source vector to a
 * specified range.
 *
 * @param src The source vector containing the values to be normalized.
 *
 * @param dst A reference to a vector where the normalized values will be
 * stored.
 *
 * @return A std::pair<double, double> containing the minimum and maximum values
 * of the source vector before normalization.
 */
std::pair<double, double> myNormVec(std::vector<double> &src,
                                    std::vector<double> &dst)
{
    double minVal;
    double maxVal;

    cv::Mat matSrc(src);
    cv::minMaxLoc(matSrc, &minVal, &maxVal);

    if (maxVal != minVal)
    {
        matSrc = (matSrc - minVal) / (maxVal - minVal);
    }

    dst = matSrc;
    std::pair<double, double> pars(minVal, maxVal);
    return pars;
}

/**
 * Scales the matrix using the min and max values supplied.
 *
 * @param src The input matrix to be normalized.
 *
 * @param dst The output matrix where the normalized result will be stored.
 *
 * @param pars A pair of doubles representing the normalization range (minimum
 * and maximum values).
 *
 */
void myNormMatInv(cv::Mat &src, cv::Mat &dst, std::pair<double, double> pars)
{
    double minVal = pars.first;
    double maxVal = pars.second;
    dst = src.clone();
    dst = (dst * (maxVal - minVal)) + minVal;
}

/**
 * The function "earliestDecPos" finds the position of the earliest decimal
 * place in a given number.
 *
 * @param num The input double value for which the earliest decimal place
 * position needs to be found.
 *
 * @return An integer representing the position of the earliest decimal place in
 * the provided number.
 */

int earliestDecPos(double num)
{
    if (std::floor(num) > 0)
    {
        std::cout << "\n TRYNG TO FIND THE EARLIEST DECIMAL PLACE FOR NUM > 1 (NOT "
                     "INTENDED WHILE DEFINITION)\n";
        return 0;
    }
    double decPart = num - std::floor(num);
    int reqpos = 0;
    double diff = 0;
    while (diff == 0)
    {
        double decPart10 = decPart * 10;
        diff = (int)decPart10 - (int)decPart;
        decPart = decPart10;
        reqpos++;
    }

    return reqpos;
}

/**
 * The function `displayImage` displays the input image in a window with the
 * specified window name.
 *
 * @param img Reference to the input image of type `cv::Mat`.
 *
 * @param windowName A string representing the name of the window in which the
 * image will be displayed.
 *
 * @param normalize A boolean flag indicating whether to normalize the input
 * image before displaying.
 *
 * @param onlyPositive A boolean flag indicating whether to normalize only the
 * positive values of the image.
 *
 * @param printDetails A boolean flag indicating whether to print details about
 * the image.
 *
 * @param videoMode A boolean flag indicating whether to operate in video mode
 * (continuous display with minimal delay).
 */
void displayImage(cv::Mat &img, std::string windowName, bool normalize,
                  bool onlyPositive, bool printDetails, bool videoMode)
{
    cv::Mat vizim = img.clone();
    cv::Mat matSrc(img);

    if (normalize == true)
    {
        double minVal, maxVal;
        cv::minMaxLoc(img, &minVal, &maxVal);

        if (onlyPositive == true)
        {
            int height = img.size().height;
            int width = img.size().width;
            cv::Mat iMat(height, width, CV_64FC3, cv::Scalar(1, 1, 1));
            img = img + (iMat * minVal);
        }

        if (maxVal != minVal)
        {
            matSrc = (img - minVal) / (maxVal - minVal);
            matSrc = matSrc * 255;
            vizim = matSrc;
        }
    }

    vizim.convertTo(vizim, CV_8U);
    if (printDetails == true)
    {
        printmat(vizim, 3);
    }

    cv::namedWindow(windowName.c_str());
    cv::imshow(windowName.c_str(), vizim);
    if (videoMode == true)
    {
        cv::waitKey(1);
    }
    else
    {
        cv::waitKey(0);
    }
}

int makeTestSeqMat(cv::Mat &testMat, bool onlyRows, bool onlyCols)
{
    for (int i = 0; i < testMat.rows; i++)
    {
        for (int j = 0; j < testMat.cols; j++)
        {
            double featureVal = testMat.at<double>(i, j);

            if ((onlyRows == true) && (onlyCols == false))
            {
                featureVal = i + 1;
            }
            else if ((onlyRows == false) && (onlyCols == true))
            {
                featureVal = j + 1;
            }
            else
            {
                featureVal = (i * testMat.rows) + j + 1;
            }
            testMat.at<double>(i, j) = featureVal;
        }
    }

    return 0;
}

/**
 * The function `splitString` splits a string into substrings using the
 * specified delimiter and stores the substrings in a vector.
 *
 * @param src The source string to be split.
 *
 * @param delimiter The character used to separate the substrings.
 *
 * @param splits Reference to a vector of strings where the resulting substrings
 * will be stored.
 *
 * @return An integer indicating the success of the split operation.
 *         Returns 0 upon successful completion.
 */
int splitString(std::string src, char delimiter,
                std::vector<std::string> &splits)
{
    std::string tmp = "";
    splits.clear();
    std::istringstream iss(src);
    while (std::getline(iss, tmp, delimiter))
    {
        splits.push_back(tmp);
    }
    return 0;
}

/**
 * The function `getFileName` extracts the file name from a given file path.
 *
 * @param filePath The input file path from which the file name needs to be
 * extracted.
 *
 * @param fileName Reference to a string where the extracted file name will be
 * stored.
 *
 * @return An integer indicating the success of the operation.
 *         Returns 0 upon successful completion.
 */
int getFileName(std::string filePath, std::string &fileName)
{
    std::vector<std::string> splits;
    splitString(filePath, '/', splits);
    // myPrintVec(splits);
    int namePos = splits.size() - 1;
    std::string fileNameWithEx = splits[namePos];
    splits.clear();
    splitString(fileNameWithEx, '.', splits);
    // myPrintVec(splits);
    fileName = splits[0];
    return 0;
}

/**
 * The function `drawPreds` draws predictions on an input image represented by
 * `srcog`.
 *
 * @param srcog The input image on which predictions are drawn, of type
 * `cv::Mat`.
 *
 * @param rotatedRect The rotated rectangle specifying the location and
 * orientation of the prediction.
 *
 * @param label The label or text associated with the prediction.
 *
 * @param windowName The name of the window to display the image with
 * predictions.
 *
 * @param saveName The filename to save the image with predictions.
 */
void drawPreds(cv::Mat srcog, cv::RotatedRect rotatedRect, std::string label,
               std::string windowName, std::string saveName)
{

    cv::Mat dst;
    srcog.copyTo(dst);
    cv::Point2f vertices[4];

    rotatedRect.points(vertices);
    for (int i = 0; i < 4; i++)
    {
        line(dst, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 255), 2);
    }
    putText(dst, label.c_str(), vertices[1], cv::FONT_HERSHEY_SIMPLEX, 0.8,
            cv::Scalar(50, 100, 50), 2);
    displayImage(dst, windowName, false, false, false, true);

    cv::imwrite(saveName, dst);
}

void clear(std::string file)
{
    std::ofstream f(file, std::ofstream::out | std::ofstream::trunc);
    if (f.good())
    {
        f.open(file, std::ofstream::out | std::ofstream::trunc);
    }
    f.close();
}

bool file_exists(std::string name)
{
    std::ifstream f(name.c_str());
    return f.good();
}
