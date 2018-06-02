//
// Created by sibirsky on 30.05.18.
//

#ifndef FIRSTPROJECT_UTIL_H
#define FIRSTPROJECT_UTIL_H

#include <opencv2/opencv.hpp>

class Util {

public:
    static void showImage(const cv::Mat& image, const char* label);
};


#endif //FIRSTPROJECT_UTIL_H
