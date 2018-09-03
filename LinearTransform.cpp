
/********************************线 性 变 换************************************
*	文件名：LinearTransform
*
*	linerTrans()						- 类内部计算函数
*
********************************************************************************/

#include "LinearTransform.h"
LinearTransform::LinearTransform(){
	numsOfParam = 2;

	ParamValue slope;
	slope.name = QStringLiteral( "Slope");
	slope.value.varType = T_DOUBLE;
	slope.value.dbVal = -1.0;
	slope.explain = QStringLiteral("斜率");
	this->ListOfParam.push_back(slope);

	ParamValue intercept;
	intercept.name = QStringLiteral( "Intercept");
	intercept.value.varType = T_DOUBLE;
	intercept.value.dbVal = 255.0;
	intercept.explain = QStringLiteral("截距");
	this->ListOfParam.push_back(intercept);
}

LinearTransform::~LinearTransform(){

}

/*******************************************************************************
*	函数名称：linerTrans ()
*
*	函数功能：类内部计算函数，内部调用全局数据指针，处理结果存放入结果图像序列中。
*			  线性变换利用一次函数的斜率、偏移量参数将输入图像的灰度值进行转换，得到新的灰度分布。
*
*	输入参数：const unsigned char* image 图像指针
*			  int imgHeight 图像高
*			  int imgWidth 图像宽
*			  double slope 线性变换斜率
*			  double intercept 线性变换截距
*
*	返 回 值：void
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
