#ifndef PowerTransform_H
#define PowerTransform_H
#include "base.h"

/************************************************************************
void powerTrans（...）      幂变换主函数
unsigned char* image		输入图像指针
int imgHeight				输入图像高度
int imgWidth				输入图像宽度
double dC					对数变换尺度比例常数
************************************************************************/

class PowerTransform :public Base
{

public:
	PowerTransform();
	~PowerTransform();

public:
	long run();
	void powerTrans(const unsigned char* image, int imgHeight,
		int imgWidth, double r);

};
#endif