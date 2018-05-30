//
// Created by sibirsky on 30.05.18.
//

#ifndef FIRSTPROJECT_DATASETSIGN_H
#define FIRSTPROJECT_DATASETSIGN_H


class DataSetSign {

public:
    DataSetSign(int xFrom, int yFrom, int width, int height);
    DataSetSign(const DataSetSign& other);
    int getXFrom();
    int getYFrom();
    int getWidth();
    int getHeight();

private:
    int xFrom;
    int yFrom;
    int width;
    int height;

};


#endif //FIRSTPROJECT_DATASETSIGN_H
