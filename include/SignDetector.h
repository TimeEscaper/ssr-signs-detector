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

    cv::Mat preProcess(const cv::Mat& image);
    void findPolys(const cv::Mat& image, std::vector<cv::Rect>& polys);
    cv::Rect polyEnvelopeRect(const std::vector<cv::Point> &poly);
    void findCircles(const cv::Mat& image, std::vector<cv::Rect> envelopeRects);
    bool applyClassifier(const cv::Mat& image);

};


#endif //FIRSTPROJECT_SIGNDETECTOR_H
