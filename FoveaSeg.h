#ifndef FoveaSeg_H
#define FoveaSeg_H
#include "base.h"
#include<stack>
/************************************************************************
void FoveaSegment(...)         �ư߷ָ�������
Mat &srcImg                    ����ԭͼ
Mat &OD_BwImg             	   �������̶�ֵ��ͼ
RotatedRect &ellipseParam      ������Բ����
int thresholdValue			   ��ֵ����ֵ
************************************************************************/

class FoveaSeg :public Base
{

public:
	FoveaSeg();
	~FoveaSeg();

public:
	long run();
	void FoveaSegment(Mat &srcImg, Mat& ODsegment255, RotatedRect &ellipsemergeOD, int ODcenterX,double param_x);
	void myphong(const Mat src, Mat&dst, int param[4]);
	void getConnectedDomain(Mat& src, vector<Rect>& boundingbox);//boundingboxΪ���ս������Ÿ�����ͨ��İ�Χ��  
	Point GetCenterPoint(const Mat &src);
	//void mat2gray(const Mat &src, Mat &dst);
	//Mat strelDisk(int Radius);
	//void fillHole(const Mat srcBw, Mat &dstBw);
	//void getConnectedDomain(Mat& src, vector<Rect>& boundingbox);
};
#endif