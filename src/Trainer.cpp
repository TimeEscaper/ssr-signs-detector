//
// Created by sibirsky on 29.05.18.
//

#include "../include/Trainer.h"
#include "../include/ImageConstants.h"
#include "../include/DataSet.h"
#include "../include/DescriptorFactory.h"

Trainer::Trainer(const std::string &trainDataDir) {
    this->trainDataDir = trainDataDir;
}

void Trainer::train(const std::string &resultFile) {
    cv::SVM svm;
    cv::SVMParams svmParams;

    std::cout << "Setting SVM parameters" << std::endl;
    svmParams.svm_type = cv::SVM::C_SVC;
    svmParams.kernel_type = cv::SVM::LINEAR;
    svmParams.C = 10;
    svmParams.p = 0;
    svmParams.nu = 0;
    svmParams.coef0 = 0;
    svmParams.gamma = 0;
    svmParams.degree = 0;
    svmParams.term_crit = cv::TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001);

    std::cout << "Loading dataset" << std::endl;
    DataSet dataSet;
    dataSet.load(trainDataDir);
    std::vector<std::vector<float>> allDescriptors;
    std::vector<float> labels;
    loadDescriptorsAndLabels(dataSet, allDescriptors, labels);

    std::cout << "Converitng to cv::Mat" << std::endl;
    int descriptorSize = allDescriptors[0].size();
    cv::Mat descriptorsMat(allDescriptors.size(), descriptorSize, CV_32F);
    cv::Mat labelsMat(labels.size(), 1, CV_32F);
    for (int i = 0; i < allDescriptors.size(); i++) {
        labelsMat.at<float>(i,0) = labels[i];
        for (int j = 0; j < descriptorSize; j++) {
            descriptorsMat.at<float>(i, j) = allDescriptors[i][j];
        }
    }

    std::cout << "Training SVM" << std::endl;
    svm.train_auto(descriptorsMat, labelsMat, cv::Mat(), cv::Mat(), svmParams, 10);
    cv::SVMParams newParams = svm.get_params();
    std::cout << "\ndegree " << newParams.degree << "\ngamma " << newParams.gamma <<
              "\nnu " << newParams.nu << "\ncoef0"  << newParams.coef0 <<
              "\np " << newParams.p << "\nc " << newParams.C << std::endl;
    svm.save(resultFile.c_str());

    std::cout << "SVM trained" << std::endl;
}

void Trainer::loadDescriptorsAndLabels(DataSet &dataSet, std::vector<std::vector<float>>& descriptors,
                                  std::vector<float>& labels) {
    for (int i = 0; i < dataSet.getImages()->size(); i++) {
        DataSetImage* img = &((*dataSet.getImages())[i]);
        cv::Mat mat = cv::imread(img->getFileName());
        for (int j = 0; j < img->getSigns()->size(); j++) {

            DataSetSign* sign = &((*img->getSigns())[j]);
            cv::Rect signRoi = cv::Rect(sign->getXFrom(), sign->getYFrom(), sign->getWidth(),
                    sign->getHeight());
            cv::Mat signImage = cv::Mat(mat, signRoi);
            cv::resize(signImage, signImage, cv::Size(IMG_ROI_WIDTH, IMG_ROI_HEIGHT), 0, 0, CV_INTER_LINEAR);
            std::vector<float> signDescriptor;
            DescriptorFactory::computeHOG(signImage, signDescriptor);
            descriptors.push_back(signDescriptor);
            labels.push_back(1.0);

            cv::Rect randRoi = cv::Rect((std::rand()%500+1), (std::rand()%500+1), IMG_ROI_WIDTH, IMG_ROI_HEIGHT);
            cv::Mat randImage = cv::Mat(mat, randRoi);
            std::vector<float> randDescriptor;
            DescriptorFactory::computeHOG(randImage, randDescriptor);
            descriptors.push_back(randDescriptor);
            labels.push_back((float)-1.0);

        }
    }
}