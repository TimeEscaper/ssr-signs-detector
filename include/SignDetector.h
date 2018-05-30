//
// Created by sibirsky on 30.05.18.
//

#ifndef FIRSTPROJECT_SIGNDETECTOR_H
#define FIRSTPROJECT_SIGNDETECTOR_H


#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

class SignDetector {

public:
    SignDetector(const std::string& trainedSVMPath);
    void detect(const cv::Mat& image, std::vector<cv::Mat> detectedSigns);

private:
    cv::SVM svm;

    void findRectangles(const cv::Mat& image, std::vector<cv::Mat> rectangles);
    cv::Point2f lineIntersection(const cv::Vec4i& a, const cv::Vec4i& b);

};


#endif //FIRSTPROJECT_SIGNDETECTOR_H
