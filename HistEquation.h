#ifndef HistEquation_H
#define HistEquation_H
#include "base.h"

/************************************************************************
void histEq(...)            直方图均衡主函数
unsigned char* image		输入图像指针
int imgHeight				输入图像高度
int imgWidth				输入图像宽度
参数;                       无
************************************************************************/

class HistEquation :public Base
{

public:
	HistEquation();
	~HistEquation();

public:
	long run();
	void histEq(const unsigned char* image, int imgHeight,int imgWidth);
};
#endif