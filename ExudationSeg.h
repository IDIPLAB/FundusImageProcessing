#ifndef ExudationSeg_H
#define ExudationSeg_H
#include "base.h"
/************************************************************************
void ExudationSegment(...)         �����ָ�������
Mat &srcImg                    ����ԭͼ
Mat &OD_BwImg             	   �������̶�ֵ��ͼ
RotatedRect &ellipseParam      ������Բ����
int thresholdValue			   ��ֵ����ֵ
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