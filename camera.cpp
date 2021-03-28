#include "camera.h"

CAMERA::CAMERA(const int &cameraID)
{
    cap = new VideoCapture(cameraID); //创建摄像头实例
    com = new Posix_QextSerialPort("/dev/ttyUSB0", QextSerialBase::Polling);

    cap->set(3, 640);	//长：640 宽480
    cap->set(4, 480);

    com->setBaudRate(BAUD9600);
    com->setDataBits(DATA_8);
    com->setParity(PAR_NONE);
    com->setStopBits(STOP_1);
    com->setFlowControl(FLOW_OFF);
    com->open(QIODevice::ReadWrite);

    if (!cap->isOpened())  //打开视像头
    {
        cout << "Cannot open the web cam" << endl;
        return;
    }

    connect(com, &Posix_QextSerialPort::readyRead, this, [=](){ cout << "success" << endl; });
}

CAMERA::~CAMERA()
{
    delete cap;             //delete video object
    delete com;
}

resColor CAMERA::calcColor()
{
    Mat imgHSV;								//用于保存hsv图像
    vector<thold> vecThold;					//存储阈值的容器向量
    vector<Mat> hsvSplit;					//分别储存hsv三通道向量容器

    vecThold.push_back(thold(Scalar(156, 43, 46), Scalar(180, 255, 255), Scalar(0, 0, 255), resColor::RED, R"(Red)"));//红色
    vecThold.push_back(thold(Scalar(35, 43, 46), Scalar(77, 255, 255), Scalar(0, 255, 0), resColor::GREEN, R"(Green)"));//绿色
    vecThold.push_back(thold(Scalar(100, 43, 46), Scalar(124, 255, 255), Scalar(255, 0, 0), resColor::BLUE, R"(Blue)"));//蓝色
    vecThold.push_back(thold(Scalar(26, 43, 46), Scalar(34, 255, 255), Scalar(0, 255, 255), resColor::YELLOW, R"(Yellow)"));//黄色

    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //bgr转hsv

    split(imgHSV, hsvSplit);						//分离hsv三通道
    equalizeHist(hsvSplit[2], hsvSplit[2]);			//直方图均衡化
    merge(hsvSplit, imgHSV);						//三通道合并
    Mat imgThresholded;								//形态学滤波变量

    for (const auto& vp : vecThold)
    {
        inRange(imgHSV, vp.low, vp.high, imgThresholded);//确定颜色显示的范围

        Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));		//得到卷积核
        morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);//去除噪点（开操作）

        morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);//连接连通域（闭操作）

        vector<vector<Point>> contours;										//储存边缘点的向量容器
        findContours(imgThresholded, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //https://www.cnblogs.com/GaloisY/p/11062065.html （定位）
        for (const auto& cp : contours)										//遍历
        {
            Rect rect = boundingRect(cp);									//根据边缘节点得到边缘方框
            if (rect.area() > 40000 && rect.area() < 100000) {									//方框面积大于30000设别出来
                rectangle(imgOriginal, rect, vp.color, 2, 8);				//框选识别内容
                putText(imgOriginal, vp.colText, Point(rect.x, rect.y+25), FONT_HERSHEY_SIMPLEX, 1, vp.color);//添加文字
                cout << vp.colText << endl;
                return vp.enum_color;
            }
        }
    }
    return resColor::NONE;
}

void CAMERA::processResult()
{
    bool bSuccess = cap->read(imgOriginal); //读取摄像头并返回bool

    if (!bSuccess) //判断是否读到数据
    {
        cout << "Cannot read a frame from video stream" << endl;
    }

    const resColor& enum_res = calcColor();				//调用计算颜色的函数
    switch(enum_res) {
    case resColor::RED:
        com->write("@M100 S\r\n", 10);
        break;
    case resColor::GREEN:
        com->write("@M100 y\r\n", 10);
        break;
    case resColor::BLUE:
        com->write("@M100 L\r\n", 10);
        break;
    case resColor::YELLOW:
        com->write("@M100 B\r\n", 10);
        break;
    default:
        com->write("@M100 G\r\n", 10);
        break;
    }

    imshow("Original", imgOriginal);	//显示窗口
}

void CAMERA::camRelease()
{
    cap->release();   //release the camera
}

void CAMERA::GammaTransform(const Mat& srcImage,Mat& dstImage,double gamma)
{
    unsigned char lut[256];
    for(int i=0;i < 256;i++)
    {
        lut[i] = saturate_cast<uchar>(pow((float)i/255.0,gamma) * 255.0f);
    }
    dstImage = srcImage.clone();
    int channels = srcImage.channels();
    switch(channels)
    {
        case 1:
        {
            MatIterator_<uchar> it = dstImage.begin<uchar>();
            MatIterator_<uchar> end = dstImage.end<uchar>();
            while(it != end)
            {
                *it = lut[(*it)];
                it ++;
            }
            break;
        }
        case 3:
            {
                MatIterator_<Vec3b> it = dstImage.begin<Vec3b>();
                MatIterator_<Vec3b> end = dstImage.end<Vec3b>();
                while(it != end)
                {
                    (*it)[0] = lut[(*it)[0]];
                    (*it)[1] = lut[(*it)[1]];
                    (*it)[2] = lut[(*it)[2]];
                    it ++;
                }
            break;
            }
        default:
            break;
    }
}
