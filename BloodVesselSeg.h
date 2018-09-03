#ifndef BloodVesselSeg_H
#define BloodVesselSeg_H
#include "base.h"

/************************************************************************
void BVSegment(...)         Ѫ�ָܷ�������
Mat &img             		����ͼ������
int thresholdValue			��ֵ����ֵ
int AreaThreshold			��ͨ�������ֵ
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