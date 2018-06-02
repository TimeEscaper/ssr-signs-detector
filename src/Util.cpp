//
// Created by sibirsky on 30.05.18.
//

#include "../include/Util.h"

void Util::showImage(const cv::Mat &image, const char* label) {
    cv::namedWindow(label);
    cv::imshow(label, image);
    cv::waitKey(10000);
    cv::destroyWindow(label);
}