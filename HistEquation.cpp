
/********************************ֱ �� ͼ �� �� ��******************************
*	�ļ�����HistEquation
*
*	histEq()						- ���ڲ����㺯��
*
********************************************************************************/


#include "HistEquation.h"

HistEquation::HistEquation(){
	numsOfParam = 0;

}

HistEquation::~HistEquation(){

}

/********************************************************************
*	�������ƣ�histEq ()
*
*	�������ܣ����ڲ����㺯�����ڲ�����ȫ������ָ�룬�����������
*			  ���ͼ�������С�������ԭʼͼ��ĻҶ�ֱ��ͼ�ӱȽϼ��е�
*			  ĳ���Ҷ���������ȫ���Ҷȷ�Χ�ڵľ��ȷֲ���
*
*	���������const unsigned char* image ͼ��ָ��
*			  int imgHeight ͼ���
*			  int imgWidth ͼ���
*
*	�� �� ֵ��void
********************************************************************/
void HistEquation::histEq(const unsigned char* image, int imgHeight,int imgWidth){
	int outputImgPixel = 0;
	int linesNum1 = 0;
	int linesNum2 = 0;
	unsigned int grayLevel = 256;
	int imgSize = imgWidth * imgHeight;	//����ͼ���С 
	int hist[256] = { 0 };				//����ͼ��Ҷ�ֱ��ͼ
	int accHist[256] = { 0 };			//����ͼ���ۼ�ֱ��ͼ
	unsigned char *outputImg = new unsigned char[imgWidth * imgHeight];
	//��������ͼ���ֱ��ͼ�ֲ�
	for (int i = 0; i < imgHeight; i++)
	{
		linesNum1 = imgWidth * i;
		for (int j = 0; j < imgWidth; j++)
		{
			++hist[image[linesNum1 + j]];
		}
	}

	//��������ͼ���ۼ�ֱ��ͼ
	for (int i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			accHist[i] = hist[i];
		}
		else
		{
			accHist[i] = hist[i] + accHist[i - 1];
		}
	}


	for (int i = 0; i < imgHeight; i++)
	{
		linesNum2 = imgWidth * i;
		for (int j = 0; j < imgWidth; j++)
		{
			outputImg[linesNum2 + j] = static_cast<unsigned char>((grayLevel - 1) *
				static_cast<double>(accHist[image[linesNum2 + j]]) / (imgSize)+0.5);
			if (outputImg[linesNum2 + j] > 255)
				outputImg[linesNum2 + j] = 255;
			else if (outputImg[linesNum2 + j] < 0)
				outputImg[linesNum2 + j] = 0;

		}
	}
	QImage* out_image = new QImage(outputImg, imgWidth, imgHeight, QImage::Format_Indexed8);
	this->setOutputImage(out_image);
}


long HistEquation::run(){
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
	histEq(net_bits0,
		transObj.inputImage.at(0)->height(),
		transObj.inputImage.at(0)->width());
	delete[] net_bits0;
	return ALL_OK;
}