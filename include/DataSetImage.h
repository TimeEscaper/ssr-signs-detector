//
// Created by sibirsky on 30.05.18.
//

#ifndef FIRSTPROJECT_DATASETIMAGE_H
#define FIRSTPROJECT_DATASETIMAGE_H

#include <string>
#include <vector>
#include "DataSetSign.h"

class DataSetImage {

public:
    DataSetImage(const std::string& fileName);
    DataSetImage(const DataSetImage& other);
    std::string getFileName();
    std::vector<DataSetSign>* getSigns();

private:
    std::string fileName;
    std::vector<DataSetSign> signs;

};


#endif //FIRSTPROJECT_DATASETIMAGE_H
