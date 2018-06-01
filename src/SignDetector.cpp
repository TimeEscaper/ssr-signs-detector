//
// Created by sibirsky on 30.05.18.
//

#include <algorithm>

#include "../include/SignDetector.h"
#include "../include/Util.h"

SignDetector::SignDetector(const std::string &trainedSVMPath) {
    svm.load(trainedSVMPath.c_str());
}

void SignDetector::detect(const cv::Mat &image, std::vector<cv::Mat> detectedSigns) {
    cv::Mat workingImage;
    cv::cvtColor(image, workingImage, CV_BGR2GRAY);
    cv::GaussianBlur(workingImage, workingImage, cv::Size(3,3), 0);
    cv::adaptiveThreshold(workingImage, workingImage, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, 0);
    //cv::Canny(workingImage, workingImage, 0, 255);
    Util::showImage(workingImage);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(workingImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
    //cv::drawContours(workingImage, contours, -1, cv::Scalar(255, 0, 0), 3, 8, hierarchy);
    Util::showImage(workingImage);

    std::vector<cv::Rect> rectangles;
    std::vector<std::vector<cv::Point>> rects;
    int maxArea = 0;
    for (int i = 0; i < contours.size(); i++) {
        std::vector<cv::Point> rect;
        cv::approxPolyDP(contours[i], rect, 3, true);
        if (rect.size() == 4) {
            rects.push_back(rect);
            cv::Point cur = rect[0];
            int xmin = cur.x;
            int ymin = cur.y;
            int xmax = cur.x;
            int ymax = cur.y;
            for (int i = 1; i < rect.size(); i++) {
                cv::Point cur = rect[i];
                if (cur.x < xmin)
                    xmin = cur.x;
                else if (cur.x > xmax) {
                    xmax = cur.x;
                }
                if (cur.y < ymin)
                    ymin = cur.y;
                else if (cur.y > ymax) {
                    ymax = cur.y;
                }
            }
            cv::Rect rectangle(xmin, ymin, xmax-xmin, ymax-ymin);
            rectangles.push_back(rectangle);
            int area = (xmax-xmin)*(ymax-ymin);
            if (area > maxArea)
                maxArea = area;
        }
    }

    cv::drawContours(workingImage, rects, -1, cv::Scalar(255, 0, 0), 3, 8);
    Util::showImage(workingImage);

    cv::Mat canvas = image;
    for (int i = 0; i < rectangles.size(); i++) {
        if (rectangles[i].area() >= 0.2*maxArea)
            cv::rectangle(canvas, rectangles[i], cv::Scalar(255, 0, 0));
    }
    Util::showImage(canvas);

}

void SignDetector::findRectangles(const cv::Mat& image, std::vector<cv::Rect>& rectangles) {
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(image, lines, 1, CV_PI/180, 15);
}

cv::Point2f SignDetector::lineIntersection(const cv::Vec4i &a, const cv::Vec4i &b) {
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
    int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
    if (float d = ((float)(x1-x2) * (y3-y4)) - ((y1-y2) * (x3-x4)))
    {
        cv::Point2f pt;
        pt.x = ((x1*y2 - y1*x2) * (x3-x4) - (x1-x2) * (x3*y4 - y3*x4)) / d;
        pt.y = ((x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4)) / d;
        if(pt.x<cv::min(x1,x2)-10||pt.x>cv::max(x1,x2)+10||pt.y<cv::min(y1,y2)-10||pt.y>cv::max(y1,y2)+10){
            return cv::Point2f(-1,-1);
        }
        if(pt.x<cv::min(x3,x4)-10||pt.x>cv::max(x3,x4)+10||pt.y<cv::min(y3,y4)-10||pt.y>cv::max(y3,y4)+10){
            return cv::Point2f(-1,-1);
        }
        return pt;
    }
    else
        return cv::Point2f(-1, -1);
}

void SignDetector::sortCorners(std::vector<cv::Point2f> &corners, cv::Point2f center) {

}

void SignDetector::sortRectCorners(const std::vector<cv::Point>& corners, std::vector<cv::Point>& sorted) {

}

bool SignDetector::comparator(cv::Point2f a, cv::Point2f b) {
    return a.x<b.x;
}

bool SignDetector::compare(cv::Point& a, cv::Point& b) {
    return (a.x < b.x) && (a.y > b.y);
}
