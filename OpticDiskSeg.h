#ifndef OpticDiskSeg_H
#define OpticDiskSeg_H
#include "base.h"
#include<stack>
/************************************************************************
void ODSegment(...)         ���̷ָ�������
Mat &img             		����ͼ������
Mat &BVimg                  ����Ѫ��ͼ��
int thresholdValue			��ֵ����ֵ
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