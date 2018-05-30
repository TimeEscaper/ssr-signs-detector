//
// Created by sibirsky on 30.05.18.
//

#include <fstream>
#include <sstream>
#include "../include/DataSet.h"


void DataSet::load(const std::string& dir) throw(std::runtime_error) {
    std::ifstream fin(dir + INDEX_FILE);
    if (!fin.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    std::string line;
    std::string prefix(FILE_PREFIX);
    DataSetImage* currentImage = NULL;
    while (std::getline(fin, line)) {
        if (line.compare(0, prefix.length(), prefix) == 0) {
            if (currentImage != NULL) {
                images.push_back(*currentImage);
            }
            delete currentImage;
            currentImage = new DataSetImage(dir + line);
        } else {
            int x, y, width, height;
            std::istringstream instream(line);
            instream >> x >> y >> width >> height;
            currentImage->getSigns()->push_back(DataSetSign(x, y, width, height));
        }
    }
    if (currentImage != NULL) {
        images.push_back(*currentImage);
    }
    delete currentImage;
    fin.close();
}

std::vector<DataSetImage>* DataSet::getImages() {
    return &images;
}