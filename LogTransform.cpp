

/******************************对 数 变 换**************************************
*	文件名：LogTransform
*
*	logTrans()						- 类内部计算函数
*
********************************************************************************/


#include "LogTransform.h"

LogTransform::LogTransform(){
	numsOfParam = 1;
	ParamValue dC;
	dC.name = QStringLiteral( "Ratio");
	dC.value.varType = T_DOUBLE;
	dC.value.dbVal = 1.0;
	dC.explain = QStringLiteral("系数\n取值范围: > 0");
	this->ListOfParam.push_back(dC);
}

LogTransform::~LogTransform(void){

}

/**********************************************************************
*	函数名称：logTrans ()
*
*	函数功能：类内部计算函数，内部调用全局数据指针，处理结果存放入结果图像序列中。
*			  函数利用对数变换每个像素点取对数处理，将输入中范围较窄的低灰度值映射为
*			  输出中较宽范围的灰度值，输入中范围较宽的高灰度值映射为输出中较低范围的
*			  灰度值相反地，对高的输入灰度值也是如此。

*	输入参数：const unsigned char* image 图像指针
*			  int imgHeight 图像高
*			  int imgWidth 图像宽
*			  double dC 增益因子，缺省值1.0
*
*	返 回 值：void
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
	//求每个图像像素点对数变换后最大最小值
	for (int i = 0; i < imgHeight; i++)
	{
		linesNum1 = imgWidth * i;
		for (int j = 0; j < imgWidth; j++)
		{
			//处理每一个像素
			double res = tmpImg[linesNum1 + j] = dC * log(image[linesNum1 + j] + 1.0);
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