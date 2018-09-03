

/******************************�� �� �� ��**************************************
*	�ļ�����LogTransform
*
*	logTrans()						- ���ڲ����㺯��
*
********************************************************************************/


#include "LogTransform.h"

LogTransform::LogTransform(){
	numsOfParam = 1;
	ParamValue dC;
	dC.name = QStringLiteral( "Ratio");
	dC.value.varType = T_DOUBLE;
	dC.value.dbVal = 1.0;
	dC.explain = QStringLiteral("ϵ��\nȡֵ��Χ: > 0");
	this->ListOfParam.push_back(dC);
}

LogTransform::~LogTransform(void){

}

/**********************************************************************
*	�������ƣ�logTrans ()
*
*	�������ܣ����ڲ����㺯�����ڲ�����ȫ������ָ�룬�������������ͼ�������С�
*			  �������ö����任ÿ�����ص�ȡ���������������з�Χ��խ�ĵͻҶ�ֵӳ��Ϊ
*			  ����нϿ�Χ�ĻҶ�ֵ�������з�Χ�Ͽ�ĸ߻Ҷ�ֵӳ��Ϊ����нϵͷ�Χ��
*			  �Ҷ�ֵ�෴�أ��Ըߵ�����Ҷ�ֵҲ����ˡ�

*	���������const unsigned char* image ͼ��ָ��
*			  int imgHeight ͼ���
*			  int imgWidth ͼ���
*			  double dC �������ӣ�ȱʡֵ1.0
*
*	�� �� ֵ��void
********************************************************************/
void LogTransform::logTrans(const unsigned char* image, 
	int imgHeight, int imgWidth, double dC)
{

	unsigned char *outputImg = new unsigned char[imgWidth * imgHeight];
	int linesNum1 = 0;
	int linesNum2 = 0;

	double maxPixel = 0.0;
	double minPixel = 100000.0;
	double *tmpImg = new double[imgWidth * imgHeight];
	//��ÿ��ͼ�����ص�����任�������Сֵ
	for (int i = 0; i < imgHeight; i++)
	{
		linesNum1 = imgWidth * i;
		for (int j = 0; j < imgWidth; j++)
		{
			//����ÿһ������
			double res = tmpImg[linesNum1 + j] = dC * log(image[linesNum1 + j] + 1.0);
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
		}
	}

	QImage* out_image = new QImage(outputImg, imgWidth, imgHeight, QImage::Format_Indexed8);
	this->setOutputImage(out_image);
	delete[] tmpImg;
}

long LogTransform::run()
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
	logTrans(net_bits0,
		transObj.inputImage.at(0)->height(),
		transObj.inputImage.at(0)->width(),
		ListOfParam.at(0).value.dbVal
		);
	delete[] net_bits0;
	return ALL_OK;
}