//
// Created by sibirsky on 30.05.18.
//

#include <algorithm>

#include "../include/SignDetector.h"
#include "../include/Util.h"
#include "../include/DescriptorFactory.h"
#include "../include/ImageConstants.h"
#include "../include/Trainer.h"


SignDetector::SignDetector(const std::string &trainedSVMPath) {
    svm.load(trainedSVMPath.c_str());
}

void SignDetector::detect(const cv::Mat &image, std::vector<cv::Mat> detectedSigns) {
    cv::Mat workingImage = preProcess(image);
    Util::showImage(workingImage, "Pre-process");

    std::vector<cv::Rect> polys;
    findPolys(workingImage, polys);

    cv::Mat canvas = image;
    for (int i = 0; i < polys.size(); i++) {
        cv::Rect poly = polys[i];
        cv::Mat sign = cv::Mat(image, poly);
        bool isSign = applyClassifier(sign);
        if (isSign) {
            cv::rectangle(canvas, poly, cv::Scalar(255, 0, 0));
            detectedSigns.push_back(sign);
        }
    }
    Util::showImage(canvas, "Result");
}

cv::Mat SignDetector::preProcess(const cv::Mat &image) {
    cv::Mat processed;
    cv::cvtColor(image, processed, CV_BGR2GRAY);
    cv::GaussianBlur(processed, processed, cv::Size(3,3), 0);
    cv::adaptiveThreshold(processed, processed, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, 0);
    //cv::Canny(workingImage, workingImage, 0, 255);
    return processed;
}

void SignDetector::findPolys(const cv::Mat& image, std::vector<cv::Rect>& polys) {
    cv::Mat workingImage = image;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(workingImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
    //cv::drawContours(workingImage, contours, -1, cv::Scalar(255, 0, 0), 3, 8, hierarchy);
    Util::showImage(workingImage, "Contours");

    std::vector<cv::Rect> rectangles;
    std::vector<cv::Rect> triangles;
    int maxRectArea = 0;
    int maxTrianArea = 0;

    for (int i = 0; i < contours.size(); i++) {
        std::vector<cv::Point> poly;
        cv::approxPolyDP(contours[i], poly, 3, true);
        if (poly.size() == 4) {
            cv::Rect rectangle = polyEnvelopeRect(poly);
            int area = rectangle.area();
            if (area > maxRectArea)
                maxRectArea = area;
            rectangles.push_back(rectangle);
        } else if (poly.size() == 3) {
            cv::Rect triangle = polyEnvelopeRect(poly);
            int area = triangle.area();
            if (area > maxTrianArea)
                maxTrianArea = area;
            triangles.push_back(triangle);
        }
    }

    for (int i = 0; i < rectangles.size(); i++) {
        cv::Rect rect = rectangles[i];
        if (rect.area() > 0.2*maxRectArea) {
            double relation;
            if (rect.width > rect.height)
                relation = (double)rect.height / (double)rect.width;
            else
                relation = (double)rect.width / (double)rect.height;
            if (relation > 0.2)
                polys.push_back(rect);
        }
    }

    for (int i = 0; i < triangles.size(); i++) {
        cv::Rect trian = triangles[i];
        if (trian.area() > 0.2*maxTrianArea) {
            double relation;
            if (trian.width > trian.height) {
                relation = (double)trian.height / (double)trian.width;
            } else {
                relation = (double)trian.width / (double)trian.height;
            }
            if (relation > 0.4)
                polys.push_back(trian);
        }
    }
}

cv::Rect SignDetector::polyEnvelopeRect(const std::vector<cv::Point> &poly) {
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

void SignDetector::findCircles(const cv::Mat &image, std::vector<cv::Rect> envelopeRects) {
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(image, circles, CV_HOUGH_GRADIENT, 1, 1);
    cv::Mat canvas = image;
    for (int i = 0; i < circles.size(); i++) {
        cv::Point center = cv::Point(circles[i][0], circles[i][1]);
        int radius = circles[i][2];
        cv::circle(canvas, center, radius, cv::Scalar(255,0,0), 3, 8, 0);
    }
    Util::showImage(canvas, "Circles");
}

bool SignDetector::applyClassifier(const cv::Mat &image) {
    std::vector<float> hogDescriptor;
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(IMG_ROI_WIDTH, IMG_ROI_HEIGHT), 0, 0, CV_INTER_LINEAR);
    DescriptorFactory::computeHOG(resized, hogDescriptor);

    int descriptorSize = hogDescriptor.size();
    cv::Mat descriptorMat(1, descriptorSize, CV_32F);
    for (int i = 0; i < descriptorSize; i++) {
        descriptorMat.at<float>(0, i) = hogDescriptor[i];
    }
    cv::Mat response(1, 1, CV_32F);

    svm.predict(descriptorMat, response);
    if (response.at<float>(0, 0) == 1.0)
        return true;
    return false;
}