#ifndef ExudationSeg_H
#define ExudationSeg_H
#include "base.h"
/************************************************************************
void ExudationSegment(...)         渗出分割主函数
Mat &srcImg                    输入原图
Mat &OD_BwImg             	   输入视盘二值化图
RotatedRect &ellipseParam      输入椭圆参数
int thresholdValue			   二值化阈值
************************************************************************/

class ExudationSeg :public Base
{

public:
	ExudationSeg();
	~ExudationSeg();

public:
	long run();
	void ExudationSegment(Mat &srcImage, Mat& ODsegment255, int stdparam);
	void fillHole(const Mat srcBw, Mat &dstBw);
	Mat strelDisk(int Radius);
	void stdfilt(const Mat &src, Mat &dst, int size);
	void morphReconstruct(Mat &marker, Mat &mask, Mat&dst);
	void edgeDetect(const Mat &srcImg, Mat &edgeImg, Mat & N_src_GROIImg, int temp[]);
};
#endif