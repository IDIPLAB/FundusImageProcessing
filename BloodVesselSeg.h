#ifndef BloodVesselSeg_H
#define BloodVesselSeg_H
#include "base.h"

/************************************************************************
void BVSegment(...)         血管分割主函数
Mat &img             		输入图像引用
int thresholdValue			二值化阈值
int AreaThreshold			连通域面积阈值
************************************************************************/

class BloodVesselSeg :public Base
{

public:
	BloodVesselSeg();
	~BloodVesselSeg();

public:
	long run();
	void BVSegment(Mat &img, int thresholdValue, double AreaThreshold);
	void fillHole(const Mat srcBw, Mat &dstBw);
	void edgeDetect(const Mat &srcImg, Mat &edgeImg, Mat & N_src_GROIImg, int temp[]);
};
#endif