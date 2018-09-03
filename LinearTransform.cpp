
/********************************�� �� �� ��************************************
*	�ļ�����LinearTransform
*
*	linerTrans()						- ���ڲ����㺯��
*
********************************************************************************/

#include "LinearTransform.h"
LinearTransform::LinearTransform(){
	numsOfParam = 2;

	ParamValue slope;
	slope.name = QStringLiteral( "Slope");
	slope.value.varType = T_DOUBLE;
	slope.value.dbVal = -1.0;
	slope.explain = QStringLiteral("б��");
	this->ListOfParam.push_back(slope);

	ParamValue intercept;
	intercept.name = QStringLiteral( "Intercept");
	intercept.value.varType = T_DOUBLE;
	intercept.value.dbVal = 255.0;
	intercept.explain = QStringLiteral("�ؾ�");
	this->ListOfParam.push_back(intercept);
}

LinearTransform::~LinearTransform(){

}

/*******************************************************************************
*	�������ƣ�linerTrans ()
*
*	�������ܣ����ڲ����㺯�����ڲ�����ȫ������ָ�룬�������������ͼ�������С�
*			  ���Ա任����һ�κ�����б�ʡ�ƫ��������������ͼ��ĻҶ�ֵ����ת�����õ��µĻҶȷֲ���
*
*	���������const unsigned char* image ͼ��ָ��
*			  int imgHeight ͼ���
*			  int imgWidth ͼ���
*			  double slope ���Ա任б��
*			  double intercept ���Ա任�ؾ�
*
*	�� �� ֵ��void
********************************************************************************/
void LinearTransform::linerTrans(Mat &img)
{
	cvtColor(img, img, CV_RGB2GRAY);
	QImage *out_image = Mat2QImage(img);
	this->setOutputImage(out_image);
}

long LinearTransform::run(){
	

	
	QImage *tempimg = transObj.inputImage.at(0);
	QImage::Format format = tempimg->format();
	Mat img = QImage2Mat(*tempimg);
	if (format == QImage::Format_RGB32||format == QImage::Format_ARGB32)
	{
		
		cvtColor(img, img, CV_RGBA2RGB);
	}
	linerTrans(img);
	return ALL_OK;
}
