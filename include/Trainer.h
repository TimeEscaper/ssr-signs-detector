//
// Created by sibirsky on 29.05.18.
//

#ifndef FIRSTPROJECT_TRAINER_H
#define FIRSTPROJECT_TRAINER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include "DataSet.h"

class Trainer {

public:
    Trainer(const std::string& trainDataDir);
    cv::Ptr<cv::SVM> train(const std::string& resultFile);

private:
    std::string trainDataDir;

    /** TODO: add const */
    void loadDescriptorsAndLabels(DataSet& dataSet, std::vector<std::vector<float>>& descriptors,
                                  std::vector<float>& labels);
};


#endif //FIRSTPROJECT_TRAINER_H
