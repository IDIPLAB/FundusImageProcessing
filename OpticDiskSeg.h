#ifndef OpticDiskSeg_H
#define OpticDiskSeg_H
#include "base.h"
#include<stack>
/************************************************************************
void ODSegment(...)         视盘分割主函数
Mat &img             		输入图像引用
Mat &BVimg                  输入血管图像
int thresholdValue			二值化阈值
************************************************************************/

class OpticDiskSeg :public Base
{

public:
	OpticDiskSeg();
	~OpticDiskSeg();

public:
	long run();
	void ODSegment(Mat &srcImg, Mat &BVimg, int thresholdValue, int segType);
	//void mat2gray(const Mat &src, Mat &dst);
	Mat strelDisk(int Radius);
	//void fillHole(const Mat srcBw, Mat &dstBw);
	void getConnectedDomain(Mat& src, vector<Rect>& boundingbox);
};
#endif