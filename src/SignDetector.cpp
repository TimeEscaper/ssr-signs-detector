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

    std::vector<std::vector<cv::Point>> rects;
    for (int i = 0; i < contours.size(); i++) {
        std::vector<cv::Point> rect;
        cv::approxPolyDP(contours[i], rect, 3, true);
        if (rect.size() == 4)
            rects.push_back(rect);
    }
    cv::drawContours(workingImage, rects, -1, cv::Scalar(255, 0, 0), 3, 8);
    Util::showImage(workingImage);

/**
    std::vector<cv::Rect> rects;
    findRectangles(workingImage, rects);

    cv::Mat canvas = image;
    for (int i = 0; i < rects.size(); i++) {
        cv::rectangle
                (canvas, rects[i], cv::Scalar(255, 0, 0));
    }
    Util::showImage(canvas);
   */

}

void SignDetector::findRectangles(const cv::Mat& image, std::vector<cv::Rect>& rectangles) {
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(image, lines, 1, CV_PI/180, 15);

    /**
    cv::Mat canvas = image;
    for (int i = 0; i < lines.size(); i++) {
        cv::line(canvas, cv::Point2f(lines[i][0], lines[i][1]), cv::Point2f(lines[i][2], lines[i][3]), cv::Scalar(255,0,0), 2);
    }
    Util::showImage(canvas);
     */


    int* poly = new int[lines.size()];
    for(int i=0;i<lines.size();i++)poly[i] = - 1;
    int curPoly = 0;
    std::vector<std::vector<cv::Point2f>> corners;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = i+1; j < lines.size(); j++)
        {

            cv::Point2f pt = lineIntersection(lines[i], lines[j]);
            if (pt.x >= 0 && pt.y >= 0){

                if(poly[i]==-1&&poly[j] == -1){
                    std::vector<cv::Point2f> v;
                    v.push_back(pt);
                    corners.push_back(v);
                    poly[i] = curPoly;
                    poly[j] = curPoly;
                    curPoly++;
                    continue;
                }
                if(poly[i]==-1&&poly[j]>=0){
                    corners[poly[j]].push_back(pt);
                    poly[i] = poly[j];
                    continue;
                }
                if(poly[i]>=0&&poly[j]==-1){
                    corners[poly[i]].push_back(pt);
                    poly[j] = poly[i];
                    continue;
                }
                if(poly[i]>=0&&poly[j]>=0){
                    if(poly[i]==poly[j]){
                        corners[poly[i]].push_back(pt);
                        continue;
                    }

                    for(int k=0;k<corners[poly[j]].size();k++){
                        corners[poly[i]].push_back(corners[poly[j]][k]);
                    }

                    corners[poly[j]].clear();
                    poly[j] = poly[i];
                    continue;
                }
            }
        }
    }

    for(int i=0;i<corners.size();i++){
        cv::Point2f center(0,0);
        if(corners[i].size()<4)continue;
        for(int j=0;j<corners[i].size();j++){
            center += corners[i][j];
        }
        center *= (1. / corners[i].size());
        sortCorners(corners[i], center);

        cv::Rect rect(corners[i][0].x, corners[i][0].y, corners[i][2].x - corners[i][0].x,
            corners[i][2].y - corners[i][0].y);
        rectangles.push_back(rect);
    }

    delete[] poly;
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
    std::vector<cv::Point2f> top, bot;
    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }
    std::sort(top.begin(),top.end(),comparator);
    std::sort(bot.begin(),bot.end(),comparator);
    cv::Point2f tl = top[0];
    cv::Point2f tr = top[top.size()-1];
    cv::Point2f bl = bot[0];
    cv::Point2f br = bot[bot.size()-1];
    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
}

bool SignDetector::comparator(cv::Point2f a, cv::Point2f b) {
    return a.x<b.x;
}

