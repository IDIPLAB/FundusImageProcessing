
/********************************直 方 图 均 衡 化******************************
*	文件名：HistEquation
*
*	histEq()						- 类内部计算函数
*
********************************************************************************/


#include "HistEquation.h"

HistEquation::HistEquation(){
	numsOfParam = 0;

}

HistEquation::~HistEquation(){

}

/********************************************************************
*	函数名称：histEq ()
*
*	函数功能：类内部计算函数，内部调用全局数据指针，处理结果存放入
*			  结果图像序列中。函数把原始图像的灰度直方图从比较集中的
*			  某个灰度区间变成在全部灰度范围内的均匀分布。
*
*	输入参数：const unsigned char* image 图像指针
*			  int imgHeight 图像高
*			  int imgWidth 图像宽
*
*	返 回 值：void
********************************************************************/
void HistEquation::histEq(const unsigned char* image, int imgHeight,int imgWidth){
	int outputImgPixel = 0;
	int linesNum1 = 0;
	int linesNum2 = 0;
	unsigned int grayLevel = 256;
	int imgSize = imgWidth * imgHeight;	//输入图像大小 
	int hist[256] = { 0 };				//输入图像灰度直方图
	int accHist[256] = { 0 };			//输入图像累计直返图
	unsigned char *outputImg = new unsigned char[imgWidth * imgHeight];
	//计算输入图像的直方图分布
	for (int i = 0; i < imgHeight; i++)
	{
		linesNum1 = imgWidth * i;
		for (int j = 0; j < imgWidth; j++)
		{
			++hist[image[linesNum1 + j]];
		}
	}

	//计算输入图像累计直方图
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