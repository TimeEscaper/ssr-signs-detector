//
// Created by sibirsky on 30.05.18.
//

#include "../include/DataSetSign.h"

DataSetSign::DataSetSign(int xFrom, int yFrom, int width, int height) {
    this->xFrom = xFrom;
    this->yFrom = yFrom;
    this->width = width;
    this->height = height;
}

DataSetSign::DataSetSign(const DataSetSign &other) {
    this->xFrom = other.xFrom;
    this->yFrom = other.yFrom;
    this->width = other.width;
    this->height = other.height;
}

int DataSetSign::getXFrom() { return xFrom; }
int DataSetSign::getYFrom() { return yFrom; }
int DataSetSign::getWidth() { return width; }
int DataSetSign::getHeight() { return height; }


