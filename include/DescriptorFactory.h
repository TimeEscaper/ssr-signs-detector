//
// Created by sibirsky on 29.05.18.
//

#ifndef FIRSTPROJECT_DESCRIPTORFACTORY_H
#define FIRSTPROJECT_DESCRIPTORFACTORY_H


#include <vector>
#include <opencv2/opencv.hpp>

class DescriptorFactory {

public:
    static void computeHOG(const cv::Mat& image, std::vector<float>& descriptors);

};


#endif //FIRSTPROJECT_DESCRIPTORFACTORY_H
