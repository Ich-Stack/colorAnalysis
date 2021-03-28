#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include "posix_qextserialport.h"

using namespace cv;
using namespace std;

enum resColor		//颜色值枚举
{
    NONE = 0,
    RED = 1,
    GREEN = 2,
    BLUE = 3,
    YELLOW = 4
};

struct thold		//存储阈值与颜色结构体
{
    Scalar low;
    Scalar high;
    Scalar color;
    resColor enum_color;
    string colText;
    thold(const Scalar &l, const Scalar &h, const Scalar& c, const resColor& ec, const string & t) : low(l), high(h), color(c), enum_color(ec), colText(t){}
    void operator =(const thold& t) { *this = t; }
};

class CAMERA : public QObject
{
public:
    CAMERA(const int &cameraID);
    ~CAMERA();

    void processResult();
    void camRelease();
private:
    VideoCapture *cap = nullptr;
    Mat imgOriginal;
    Posix_QextSerialPort *com;

private:
    resColor calcColor();		//计算颜色函数
    void GammaTransform(const Mat& srcImage,Mat& dstImage,double gamma);
};

#endif // CAMERA_H
