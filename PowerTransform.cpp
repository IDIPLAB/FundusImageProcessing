
/*********************************�� �� �� ��***********************************
*	�ļ�����PowerTransform
*
*	powerTrans()						- ���ڲ����㺯��
*
********************************************************************************/

#include "PowerTransform.h"
//#include <assert.h>

PowerTransform::PowerTransform()
{
	numsOfParam = 1;

	ParamValue r;
	r.name = QStringLiteral( "PowerFactor");
	r.value.varType = T_DOUBLE;
	r.value.dbVal = 2.0;
	r.explain = QStringLiteral("ָ��\nȡֵ��Χ��> 0");
	this->ListOfParam.push_back(r);

}

PowerTransform::~PowerTransform(){

}

/****************************************************************************************
*	�������ƣ�powerTrans ()
*
*	�������ܣ����ڲ����㺯�����ڲ�����ȫ������ָ�룬������������
*			  ��ͼ�������С����ɱ任��٤��任�����Ժܺõ�����ͼ��ĶԱȶȣ�
*			  ��չ�Ҷȼ�����ͼ�������Ҷ�ƫ��ʱ��ѡ��y<1,����ʹͼ��������
*			  ��ͼ������Ҷ�ƫ��ʱ��ѡ��y>1������ʹͼ��䰵�����ͼ��ĶԱȶȣ�͹��ϸ�ڡ�
*
*	���������const unsigned char* image ͼ��ָ��
*			  int imgHeight ͼ���
*			  int imgWidth ͼ���
*			  double r ٤������
*
*	�� �� ֵ��void
*****************************************************************************************/
void PowerTransform::powerTrans(const unsigned char* image,
	int imgHeight, int imgWidth, double r)
{

	int outputImgPixel = 0;
	int linesNum1 = 0;
	int linesNum2 = 0;

	double maxPixel = 0.0;
	double minPixel = 10000.0;
	unsigned char *outputImg = new unsigned char[imgWidth * imgHeight];
	double *tmpImg = new double[imgWidth * imgHeight];
	//��ÿ��ͼ�����ص�ָ���任�������Сֵ
	for (int i = 0; i < imgHeight; i++)
	{
		linesNum1 = imgWidth * i;
		for (int j = 0; j < imgWidth; j++)
		{
			double res = tmpImg[linesNum1 + j] = pow(image[linesNum1 + j], r);
			if (res >= maxPixel)
				maxPixel = res;
			if (res <= minPixel)
				minPixel = res;
		}
	}

	//ͼ����ж����任����������ֵת��Ϊ[0��255]֮��
	for (int i = 0; i < imgHeight; i++)
	{
		linesNum2 = imgWidth * i;
		for (int j = 0; j < imgWidth; j++)
		{
			outputImg[linesNum2 + j] = static_cast<unsigned char>(((tmpImg[linesNum2 + j] - minPixel) / (maxPixel - minPixel)) * 255.0);
			if (outputImg[linesNum2 + j] > 255)
				outputImg[linesNum2 + j] = 255;
			else if (outputImg[i] < 0)
				outputImg[linesNum2 + j] = 0;
		}
	}
	QImage* out_image = new QImage(outputImg, imgWidth, imgHeight, QImage::Format_Indexed8);
	this->setOutputImage(out_image);
	delete[] tmpImg;
}

long PowerTransform::run()
{
	if (ListOfParam.at(0).value.dbVal < 0) return ERROR_INVALID_ARG;
	int imgHeight0 = transObj.inputImage.at(0)->height();

	int imgWidth0 = transObj.inputImage.at(0)->width();

	int nEffWidth0 = transObj.inputImage.at(0)->bytesPerLine();
	unsigned char* bits0 = transObj.inputImage.at(0)->bits();
	unsigned char* net_bits0 = new unsigned char[imgHeight0*imgWidth0];

	for (int row = 0; row < imgHeight0; row++)
	{
		long index = row*imgWidth0;
		for (int col = 0; col < imgWidth0; col++)
		{
			net_bits0[index + col] = bits0[row*nEffWidth0 + col];
		}
	}

	powerTrans(net_bits0,
		transObj.inputImage.at(0)->height(),
		transObj.inputImage.at(0)->width(),
		ListOfParam.at(0).value.dbVal);
	delete[] net_bits0;
	return ALL_OK;
}