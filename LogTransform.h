#ifndef LogTransform_H
#define LogTransform_H
#include "base.h"

/************************************************************************
void logTrans(...)          �����任������
unsigned char* image		����ͼ��ָ��
int imgHeight				����ͼ��߶�
int imgWidth				����ͼ����
double dC					�����任�߶ȱ�������
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