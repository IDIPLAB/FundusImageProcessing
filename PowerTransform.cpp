
/*********************************幂 律 变 换***********************************
*	文件名：PowerTransform
*
*	powerTrans()						- 类内部计算函数
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
	r.explain = QStringLiteral("指数\n取值范围：> 0");
	this->ListOfParam.push_back(r);

}

PowerTransform::~PowerTransform(){

}

/****************************************************************************************
*	函数名称：powerTrans ()
*
*	函数功能：类内部计算函数，内部调用全局数据指针，处理结果存放入结
*			  果图像序列中。幂律变换（伽马变换）可以很好地拉伸图像的对比度，
*			  扩展灰度级。当图像的整体灰度偏暗时，选择y<1,可以使图像增亮；
*			  当图像整体灰度偏亮时，选择y>1，可以使图像变暗，提高图像的对比度，凸显细节。
*
*	输入参数：const unsigned char* image 图像指针
*			  int imgHeight 图像高
*			  int imgWidth 图像宽
*			  double r 伽马因子
*
*	返 回 值：void
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
	//求每个图像像素点指数变换后最大最小值
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

	//图像进行对数变换，并将像素值转变为[0，255]之间
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