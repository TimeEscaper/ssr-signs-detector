//
// Created by sibirsky on 30.05.18.
//

#include "../include/Util.h"

void Util::showImage(const cv::Mat &image) {
    cv::namedWindow("Image");
    cv::imshow("Image", image);
    cv::waitKey(10000);
    cv::destroyWindow("Image");
}