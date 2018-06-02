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

    std::vector<cv::Rect> polys;
    findPolys(contours, polys);

    cv::Mat canvas = image;
    for (int i = 0; i < polys.size(); i++) {
        cv::rectangle(canvas, polys[i], cv::Scalar(255, 0, 0));
    }
    Util::showImage(canvas);
}

void SignDetector::findPolys(const std::vector<std::vector<cv::Point>> &contours,
                             std::vector<cv::Rect>& polys) {
    std::vector<cv::Rect> rectangles;
    std::vector<cv::Rect> triangles;
    int maxRectArea = 0;
    int maxTrianArea = 0;

    for (int i = 0; i < contours.size(); i++) {
        std::vector<cv::Point> poly;
        cv::approxPolyDP(contours[i], poly, 3, true);
        if (poly.size() == 4) {
            cv::Rect rectangle = envelopeRect(poly);
            int area = rectangle.area();
            if (area > maxRectArea)
                maxRectArea = area;
            rectangles.push_back(rectangle);
        } else if (poly.size() == 3) {
            cv::Rect triangle = envelopeRect(poly);
            int area = triangle.area();
            if (area > maxTrianArea)
                maxTrianArea = area;
            triangles.push_back(triangle);
        }
    }

    for (int i = 0; i < rectangles.size(); i++) {
        cv::Rect rect = rectangles[i];
        if (rect.area() > 0.2*maxRectArea)
            polys.push_back(rect);
    }
    for (int i = 0; i < triangles.size(); i++) {
        cv::Rect trian = triangles[i];
        if (trian.area() > 0.2*maxTrianArea)
            polys.push_back(trian);
    }
}

cv::Rect SignDetector::envelopeRect(const std::vector<cv::Point> &poly) {
    cv::Point cur = poly[0];
    int xmin = cur.x;
    int ymin = cur.y;
    int xmax = cur.x;
    int ymax = cur.y;
    for (int i = 1; i < poly.size(); i++) {
        cv::Point cur = poly[i];
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
    return cv::Rect(xmin, ymin, xmax-xmin, ymax-ymin);
}