#ifndef HistEquation_H
#define HistEquation_H
#include "base.h"

/************************************************************************
void histEq(...)            ֱ��ͼ����������
unsigned char* image		����ͼ��ָ��
int imgHeight				����ͼ��߶�
int imgWidth				����ͼ����
����;                       ��
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