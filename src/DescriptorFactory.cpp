//
// Created by sibirsky on 29.05.18.
//

#include "../include/DescriptorFactory.h"

void DescriptorFactory::computeHOG(const cv::Mat& image, std::vector<float>& descriptors) {
    cv::HOGDescriptor hogDescriptor(cv::Size(32, 32), //windowSize
            cv::Size(16,16), //blockSize
            cv::Size(8,8), //blockStride,
            cv::Size(8,8), //cellSize,
            9); //nbins


    hogDescriptor.compute(image, descriptors);
}