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

    void findPolys(const std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Rect>& polys);
    cv::Rect envelopeRect(const std::vector<cv::Point>& poly);

};


#endif //FIRSTPROJECT_SIGNDETECTOR_H
