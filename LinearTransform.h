#ifndef LinearTransform_H
#define LinearTransform_H
#include "base.h"

/************************************************************************
void linerTrans(...)        线性变换主函数
unsigned char* image		输入图像指针
int imgHeight				输入图像高度
int imgWidth				输入图像宽度
float slope					线性变换的斜率
float intercept				线性变换的截距
************************************************************************/

class LinearTransform :public Base
{

public:
	LinearTransform();
	~LinearTransform();

public:
	long run();
	void linerTrans(Mat &img);
	
	
};
#endif