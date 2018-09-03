#ifndef LogTransform_H
#define LogTransform_H
#include "base.h"

/************************************************************************
void logTrans(...)          对数变换主函数
unsigned char* image		输入图像指针
int imgHeight				输入图像高度
int imgWidth				输入图像宽度
double dC					对数变换尺度比例常数
************************************************************************/

class LogTransform :public Base
{

public:
	LogTransform();
	~LogTransform(void);

public:
	long run();
	void logTrans(const unsigned char* image, int imgHeight,
		int imgWidth, double dC = 1.0);

};
#endif