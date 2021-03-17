#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

enum resColor		//颜色值枚举
{
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    YELLOW = 3
};

struct thold		//存储阈值与颜色结构体
{
    Scalar low;
    Scalar high;
    Scalar color;
    string colText;
    thold(const Scalar &l, const Scalar &h, const Scalar& c, const string & t) : low(l), high(h), color(c), colText(t){}
    void operator =(const thold& t) { *this = t; }
};

class CAMERA
{
public:
    CAMERA(const int &cameraID);
    ~CAMERA();

    void resShow();
    void camRelease();
private:
    VideoCapture *cap = nullptr;
    Mat imgOriginal;

private:
    void calcColor();		//计算颜色函数
};

#endif // CAMERA_H
