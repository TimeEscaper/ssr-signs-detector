#include <iostream>
#include "../include/Trainer.h"
#include "../include/SignDetector.h"
#include "../include/Util.h"

#define DATASET_PATH "/home/sibirsky/ssr-dataset/train/"
#define SVM_MODEL_PATH "/home/sibirsky/ssr-dataset/svm.xml"

#define TEST_IMAGE "/home/sibirsky/autosave13_04_2013_14_12_47_2.jpg"

#define TRAI

#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
#ifdef TRAIN
    Trainer trainer(DATASET_PATH);
    trainer.train(SVM_MODEL_PATH);
#else
    SignDetector detector(SVM_MODEL_PATH);
    cv::Mat image = cv::imread(TEST_IMAGE);
    Util::showImage(image, "Original");

    std::vector<cv::Mat> signs;
    detector.detect(image, signs);
#endif
    return 0;
}