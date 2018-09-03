#ifndef FoveaSeg_H
#define FoveaSeg_H
#include "base.h"
#include<stack>
/************************************************************************
void FoveaSegment(...)         黄斑分割主函数
Mat &srcImg                    输入原图
Mat &OD_BwImg             	   输入视盘二值化图
RotatedRect &ellipseParam      输入椭圆参数
int thresholdValue			   二值化阈值
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
	void getConnectedDomain(Mat& src, vector<Rect>& boundingbox);//boundingbox为最终结果，存放各个连通域的包围盒  
	Point GetCenterPoint(const Mat &src);
	//void mat2gray(const Mat &src, Mat &dst);
	//Mat strelDisk(int Radius);
	//void fillHole(const Mat srcBw, Mat &dstBw);
	//void getConnectedDomain(Mat& src, vector<Rect>& boundingbox);
};
#endif