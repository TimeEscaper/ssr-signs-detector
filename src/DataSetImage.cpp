//
// Created by sibirsky on 30.05.18.
//

#include "../include/DataSetImage.h"

DataSetImage::DataSetImage(const std::string &fileName) {
    this->fileName = fileName;
}

DataSetImage::DataSetImage(const DataSetImage &other) {
    this->fileName = other.fileName;
    this->signs = other.signs;
}

std::string DataSetImage::getFileName() {
    return std::string(fileName);
}

std::vector<DataSetSign>* DataSetImage::getSigns() {
    return &signs;
}