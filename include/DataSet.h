//
// Created by sibirsky on 30.05.18.
//

#ifndef FIRSTPROJECT_DATASET_H
#define FIRSTPROJECT_DATASET_H


#include <string>
#include <stdexcept>
#include <vector>
#include "DataSetImage.h"

#define INDEX_FILE "index.txt"
#define FILE_PREFIX "autosave"

class DataSet {

public:
    void load(const std::string& dir) throw(std::runtime_error);
    std::vector<DataSetImage>* getImages();


private:
    std::vector<DataSetImage> images;

};


#endif //FIRSTPROJECT_DATASET_H
