#ifndef PowerTransform_H
#define PowerTransform_H
#include "base.h"

/************************************************************************
void powerTrans��...��      �ݱ任������
unsigned char* image		����ͼ��ָ��
int imgHeight				����ͼ��߶�
int imgWidth				����ͼ����
double dC					�����任�߶ȱ�������
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