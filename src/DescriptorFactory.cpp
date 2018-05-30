//
// Created by sibirsky on 29.05.18.
//

#include "../include/DescriptorFactory.h"

void DescriptorFactory::computeHOG(const cv::Mat& image, std::vector<float>& descriptors) {
    cv::HOGDescriptor hogDescriptor(cv::Size(16, 16), //windowSize
            cv::Size(4,4), //blockSize
            cv::Size(4,4), //blockStride,
            cv::Size(2,2), //cellSize,
            9); //nbins


    hogDescriptor.compute(image, descriptors);
}