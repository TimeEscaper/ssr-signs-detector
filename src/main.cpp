#include <iostream>
#include "../include/Trainer.h"
#include "../include/SignDetector.h"
#include "../include/Util.h"

#define DATASET_PATH "/home/sibirsky/ssr-dataset/train/"
#define SVM_MODEL_PATH "/home/sibirsky/ssr-dataset/svm.xml"

#define TEST_IMAGE "/home/sibirsky/autosave16_04_2013_11_46_48_0.jpg"

#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
#ifdef TRAIN
    Trainer trainer(DATASET_PATH);
    trainer.train("SVM_MODEL_PATH");
    return 0;
#else
    SignDetector detector(SVM_MODEL_PATH);
    cv::Mat image = cv::imread(TEST_IMAGE);
    Util::showImage(image);

    std::vector<cv::Mat> signs;
    detector.detect(image, signs);
#endif
}