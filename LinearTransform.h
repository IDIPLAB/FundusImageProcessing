#ifndef LinearTransform_H
#define LinearTransform_H
#include "base.h"

/************************************************************************
void linerTrans(...)        ���Ա任������
unsigned char* image		����ͼ��ָ��
int imgHeight				����ͼ��߶�
int imgWidth				����ͼ����
float slope					���Ա任��б��
float intercept				���Ա任�Ľؾ�
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