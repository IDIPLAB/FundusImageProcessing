/********************************视 盘 分 割************************************
*	文件名：OpticDiskSeg
*
*	ODSegment()						- 类内部计算函数
*
********************************************************************************/
#include "OpticDiskSeg.h"
OpticDiskSeg::OpticDiskSeg(){
	numsOfParam = 1;

	ParamValue thresholdValue;
	thresholdValue.name = QStringLiteral("thresholdValue");
	thresholdValue.value.varType = T_INT;
	thresholdValue.value.nVal = -1;
	thresholdValue.explain = QStringLiteral("二值化阈值\n取值范围: >= 0 ; -1为Otsu阈值分割\n推荐-1或242");
	this->ListOfParam.push_back(thresholdValue);


}

OpticDiskSeg::~OpticDiskSeg(){

}


/*******************************************************************************
*	函数名称：ODSegment();
*
*	函数功能：类内部计算函数，内部调用图像引用，处理结果存放入结果图像序列中。
*			  进行视盘分割。
*
*	输入参数：Mat & img 图像
*			  Mat & BVimgMat 血管图像
*			  int thresholdValue 二值化阈值
*
*	返 回 值：void
********************************************************************************/
Mat OpticDiskSeg::strelDisk(int Radius)
{

	int borderWidth; Mat sel; int m, n;
	switch (Radius){
	case 1:
	case 2:
		if (Radius == 1)
			borderWidth = 1;
		else
			borderWidth = 2;
		sel = Mat((2 * Radius + 1), (2 * Radius + 1), CV_8U, cv::Scalar(1));
		break;//当半径为1时是3X3的 ,当半径为2时是5X5的  
	case 3:
		borderWidth = 0;
		sel = Mat((2 * Radius - 1), (2 * Radius - 1), CV_8U, cv::Scalar(1));
		break;
	default:
		n = Radius / 7; m = Radius % 7;
		if (m == 0 || m >= 4)
			borderWidth = 2 * (2 * n + 1);
		else
			borderWidth = 2 * 2 * n;
		sel = Mat((2 * Radius - 1), (2 * Radius - 1), CV_8U, cv::Scalar(1));
		break;
	}
	for (int i = 0; i < borderWidth; i++){
		for (int j = 0; j < borderWidth; j++){
			if (i + j < borderWidth){
				sel.at<uchar>(i, j) = 0;
				sel.at<uchar>(i, sel.cols - 1 - j) = 0;
				sel.at<uchar>(sel.rows - 1 - i, j) = 0;
				sel.at<uchar>(sel.rows - 1 - i, sel.cols - 1 - j) = 0;
			}
		}
	}
	return sel;


}


void OpticDiskSeg::getConnectedDomain(Mat& src, vector<Rect>& boundingbox)
{
	int img_row = src.rows;
	int img_col = src.cols;
	Mat flag = Mat::zeros(Size(img_col, img_row), CV_8UC1);//标志矩阵，为0则当前像素点未访问过  
	for (int i = 0; i < img_row; i++)
	{
		for (int j = 0; j < img_col; j++)
		{
			if (src.ptr<uchar>(i)[j] == 255 && flag.ptr<uchar>(i)[j] == 0)
			{
				stack<Point2f> cd;
				cd.push(Point2f(j, i));
				flag.ptr<uchar>(i)[j] = 1;
				int minRow = i, minCol = j;//包围盒左、上边界  
				int maxRow = i, maxCol = j;//包围盒右、下边界  
				while (!cd.empty())
				{
					Point2f tmp = cd.top();
					if (minRow > tmp.y)//更新包围盒  
						minRow = tmp.y;
					if (minCol > tmp.x)
						minCol = tmp.x;
					if (maxRow < tmp.y)
						maxRow = tmp.y;
					if (maxCol < tmp.x)
						maxCol = tmp.x;
					cd.pop();
					Point2f p[4];//邻域像素点，这里用的四邻域  
					p[0] = Point2f(tmp.x - 1 > 0 ? tmp.x - 1 : 0, tmp.y);
					p[1] = Point2f(tmp.x + 1 < img_col - 1 ? tmp.x + 1 : img_row - 1, tmp.y);
					p[2] = Point2f(tmp.x, tmp.y - 1 > 0 ? tmp.y - 1 : 0);
					p[3] = Point2f(tmp.x, tmp.y + 1 < img_row - 1 ? tmp.y + 1 : img_row - 1);
					for (int m = 0; m < 4; m++)
					{
						int x = p[m].y;
						int y = p[m].x;
						if (src.ptr<uchar>(x)[y] == 255 && flag.ptr<uchar>(x)[y] == 0)//如果未访问，则入栈，并标记访问过该点  
						{
							cd.push(p[m]);
							flag.ptr<uchar>(x)[y] = 1;
						}
					}
				}
				Rect rect(Point2f(minCol, minRow), Point2f(maxCol + 1, maxRow + 1));
				boundingbox.push_back(rect);
			}
		}
	}




}



void OpticDiskSeg::ODSegment(Mat &srcImage, Mat &BVimg, int thresholdvalue, int segtype)
{

	//src图片取ROI，这里的xmin是norm后取G通道得出来的，还不是原图的xmin；
	
	Mat hsvImg;
	cvtColor(srcImage, hsvImg, CV_RGB2HSV);
	vector<Mat> HSVchannels;
	split(hsvImg, HSVchannels);
	Mat imageHueChannel, imageSaturationChannel, imgValueChannel;
	imageHueChannel = HSVchannels.at(0);
	imageSaturationChannel = HSVchannels.at(1);
	imgValueChannel = HSVchannels.at(2);
	double V_Mean;
	V_Mean = mean(imgValueChannel)[0];
	Mat hsvNormImg = hsvImg.clone();
	//cout << V_Mean;
	if (V_Mean < 89)
	{
		for (int i = 0; i < hsvNormImg.size().width; i++)
		{
			for (int j = 0; j < hsvNormImg.size().height; j++)
			{
				hsvNormImg.at<Vec3b>(j, i)[2] = saturate_cast<uchar>(1.3*imgValueChannel.at<uchar>(j, i));
				hsvNormImg.at<Vec3b>(j, i)[1] = saturate_cast<uchar>(1.2*imageSaturationChannel.at<uchar>(j, i));
				hsvNormImg.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(imageHueChannel.at<uchar>(j, i));   //H通道变为180的
			}
		}
	}
	Mat srcNormImg;
	cvtColor(hsvNormImg, srcNormImg, CV_HSV2RGB);
	//namedWindow("srcNormImg", 0);
	//imshow("srcNormImg", srcNormImg);

	//Mat srcGrayImg;
	//cvtColor(srcNormImg, srcGrayImg, CV_RGB2GRAY);
	vector<Mat> RGBchannels1;
	split(srcNormImg, RGBchannels1);
	Mat src_GImag = RGBchannels1.at(1);
	vector<int> xLocation, yLocation;
	for (int i = 0; i < src_GImag.size().width; i++)
	{
		for (int j = 0; j < src_GImag.size().height; j++)
		{
			if (src_GImag.at<uchar>(j, i)>10)
			{
				xLocation.push_back(i);
				yLocation.push_back(j);
			}

		}

	}
	int x_nozeromin, x_nozeromax, y_nozeromin, y_nozeromax;
	x_nozeromin = *min_element(xLocation.begin(), xLocation.end());
	x_nozeromax = *max_element(xLocation.begin(), xLocation.end());
	y_nozeromin = *min_element(yLocation.begin(), yLocation.end());
	y_nozeromax = *max_element(yLocation.begin(), yLocation.end());
	int xlength = x_nozeromax - x_nozeromin;
	int ylength = y_nozeromax - y_nozeromin;
	Mat src_CloneImg = srcImage.clone();
	Mat src_ROIImg = src_CloneImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1));




	vector<Mat> RGBchannels;
	split(src_ROIImg, RGBchannels);
	Mat imageRChannel, imageGChannel;
	imageRChannel = RGBchannels.at(2).clone();
	imageGChannel = RGBchannels.at(1).clone();
	threshold(imageRChannel, imageRChannel, 250, 255, THRESH_BINARY);
	threshold(imageGChannel, imageGChannel, 160, 255, THRESH_BINARY);
	Mat RGthreshold_Image;
	RGthreshold_Image = imageGChannel.mul(imageRChannel);

	//连通域扩张
	vector<Rect> boundingbox;
	getConnectedDomain(RGthreshold_Image, boundingbox);

	Mat bwExpandImg(imageGChannel.size(), CV_8U, Scalar(0));
	for (size_t i = 0; i < boundingbox.size(); i++)
	{
		Rect currentbox = boundingbox[i];
		if (currentbox.x>0.5*currentbox.width)
		{
			currentbox.x = currentbox.x - 0.5*currentbox.width;
		}
		else
		{
			currentbox.x = currentbox.x;
		}
		if (currentbox.x + 2 * currentbox.width<src_ROIImg.size().width)
		{
			currentbox.width = 2 * currentbox.width;
		}
		else
		{
			currentbox.width = src_ROIImg.size().width - currentbox.x + currentbox.width - 1;
		}
		rectangle(bwExpandImg, currentbox, Scalar(255), -1);
	}
	Mat bwExpandImg1;
	threshold(bwExpandImg, bwExpandImg1, 100, 1, THRESH_BINARY);
	Mat srcExpandBVImg;
	srcExpandBVImg = bwExpandImg1.mul(BVimg);

	//滤波模板
	float RMask[9][9] = { { 135, 150, 165, 180, 180, 90, 75, 60, 45 }, { 120, 135, 150, 165, 180, 75, 60, 45, 30 }, { 105, 120, 135, 150, 180, 60, 45, 30, 15 },
	{ 90, 105, 120, 135, 180, 45, 30, 15, 0 }, { -20, -20, -20, -20, 180, -20, -20, -20, -20 }, { 90, 105, 120, 135, 180, 45, 30, 15, 0 }, { 105, 120, 135, 150, 180, 60, 45, 30, 15 },
	{ 120, 135, 150, 165, 180, 75, 60, 45, 30 }, { 135, 150, 165, 180, 180, 90, 75, 60, 45 } };

	float LMask[9][9] = { { 45, 60, 75, 90, 180, 180, 165, 150, 135 }, { 30, 45, 60, 75, 180, 165, 150, 135, 120 }, { 15, 30, 45, 60, 180, 150, 135, 120, 105 },
	{ 0, 15, 30, 45, 180, 135, 120, 105, 90 }, { -20, -20, -20, -20, 180, -20, -20, -20, -20 }, { 0, 15, 30, 45, 180, 135, 120, 105, 90 }, { 15, 30, 45, 60, 180, 150, 135, 120, 105 },
	{ 30, 45, 60, 75, 180, 165, 150, 135, 120 }, { 45, 60, 75, 90, 180, 180, 165, 150, 135 } };

	Mat Rmaskmetrix = Mat(Size(9, 9), CV_32F, RMask);
	//Mat Lmaskmetrix = Mat(Size(9, 9), CV_32FC1, LMask);
	Mat Lmaskmetrix = Mat(Size(9, 9), CV_32F, LMask);
	int leftNumber = 0, rightNumber = 0;
	for (int i = 0; i < srcExpandBVImg.size().width; i++)
	{
		for (int j = 0; j < srcExpandBVImg.size().height; j++)
		{
			if (0 != srcExpandBVImg.at<uchar>(j, i))
			{
				if (i<0.5*srcExpandBVImg.size().width)
				{
					leftNumber++;
				}
				else
				{
					rightNumber++;
				}
			}

		}
	}
	Mat Lmask;

	Mat Rmask(Size(81, 61), Rmaskmetrix.type());
	Mat mask;
	resize(Lmaskmetrix, Lmask, Size(0, 0), 20.0 / 9, 20.0 / 9, CV_INTER_CUBIC);//resize不支持int型
	resize(Rmaskmetrix, Rmask, Rmask.size(), 0.5, 0.5, CV_INTER_CUBIC);
	if (leftNumber>rightNumber)
	{
		mask = Lmask.clone();
	}
	else
	{
		mask = Rmask.clone();
	}

	Mat filter_ExpandBVImg(srcExpandBVImg.size(), CV_32F); //滤波后输出

	//对模板矩阵旋转180度
	Point center = Point(mask.cols / 2, mask.rows / 2);
	Mat rotmat = getRotationMatrix2D(center, 180, 1);
	Mat maskRotate(mask.size(), mask.type());
	warpAffine(mask, maskRotate, rotmat, mask.size());

	Mat float_srcExpandBVImg;
	srcExpandBVImg.convertTo(float_srcExpandBVImg, CV_32F);

	Scalar Bordervalue = Scalar(0);
	int top = 0.5*mask.rows, bottom = top;
	int left = 0.5*mask.cols, right = left;
	copyMakeBorder(float_srcExpandBVImg, float_srcExpandBVImg, top, bottom, left, right, BORDER_CONSTANT, Bordervalue);
	filter2D(float_srcExpandBVImg, filter_ExpandBVImg, -1, maskRotate, Point(-1, -1), 0, BORDER_CONSTANT);
	//////////////滤波结果不一样，matlab默认是FULL，应该是用0填。 上面这样补零了还是差几个像素

	//中心定位和画十字符号
	Point OpticDiskCenter;
	minMaxLoc(filter_ExpandBVImg, NULL, NULL, NULL, &OpticDiskCenter);

	

	//基于中心定位的视盘ROI区域确定
	int roughrx = 220, roughry = 200;
	int ymin, ymax, xmin, xmax;
	int ODcenterX = OpticDiskCenter.x; int ODcenterY = OpticDiskCenter.y;
	IntParam.push_back(ODcenterX);
	if (ODcenterX - roughrx <= 0)
	{
		xmin = 1;
	}
	else xmin = ODcenterX - roughrx;

	if (ODcenterX + roughrx >= src_ROIImg.size().width)
	{
		xmax = src_ROIImg.size().width;
	}
	else xmax = ODcenterX + roughrx;

	if (ODcenterY - roughry <= 0)
	{
		ymin = 1;
	}
	else ymin = ODcenterY - roughry;

	if (ODcenterY + roughry >= src_ROIImg.size().height)
	{
		ymax = src_ROIImg.size().height;
	}
	else ymax = ODcenterY + roughry;

	//处理周围黑色背景转成白色
	Vec3b zeroChangedVec; zeroChangedVec[0] = 255; zeroChangedVec[1] = 255; zeroChangedVec[2] = 255;
	Mat Cut_OpticDiskROI = src_ROIImg(Range(ymin, ymax), Range(xmin, xmax)).clone();
	for (int i = 0; i < Cut_OpticDiskROI.size().width; i++)
	{
		for (int j = 0; j < Cut_OpticDiskROI.size().height; j++)
		{
			if (20> Cut_OpticDiskROI.at<Vec3b>(j, i)[0] &&         //2(16)在这边颜色阈值分割失败
				20 > Cut_OpticDiskROI.at<Vec3b>(j, i)[1] &&
				20 > Cut_OpticDiskROI.at<Vec3b>(j, i)[2])
			{
				Cut_OpticDiskROI.at<Vec3b>(j, i) = zeroChangedVec;
			}
		}
	}
	vector<Mat> OD_RGBchannels;
	split(Cut_OpticDiskROI, OD_RGBchannels);

	//处理内部血管和白色背景
	Mat element20 = getStructuringElement(MORPH_ELLIPSE, Size(20, 20));
	Mat element22 = getStructuringElement(MORPH_ELLIPSE, Size(22, 22));
	Mat element24 = getStructuringElement(MORPH_ELLIPSE, Size(24, 24));
	//Mat element65 = getStructuringElement(MORPH_ELLIPSE, Size(80, 80));
	Mat element65 = strelDisk(65);
	Mat OpticDiskROI_Dilate_Erode = OD_RGBchannels.at(2); //分出来的是BGR
	for (int i = 0; i < OpticDiskROI_Dilate_Erode.size().width; i++)
	{
		for (int j = 0; j < OpticDiskROI_Dilate_Erode.size().height; j++)
		{
			if (0 == OpticDiskROI_Dilate_Erode.at<uchar>(j, i))
			{
				OpticDiskROI_Dilate_Erode.at<uchar>(j, i) = 255;
			}
		}
	}
	
	morphologyEx(OpticDiskROI_Dilate_Erode, OpticDiskROI_Dilate_Erode, MORPH_CLOSE, element20, Point(-1, -1), 1);
	morphologyEx(OpticDiskROI_Dilate_Erode, OpticDiskROI_Dilate_Erode, MORPH_OPEN, element22, Point(-1, -1), 1);
	morphologyEx(OpticDiskROI_Dilate_Erode, OpticDiskROI_Dilate_Erode, MORPH_CLOSE, element24, Point(-1, -1), 1);
	morphologyEx(OpticDiskROI_Dilate_Erode, OpticDiskROI_Dilate_Erode, MORPH_OPEN, element65, Point(-1, -1), 1);

	//椭圆拟合
	Mat bwOpticDiskROI; 
	threshold(OpticDiskROI_Dilate_Erode, bwOpticDiskROI, thresholdvalue, 255, segtype);  // 1.2/1.8/2(12)/2（13）/3(14)/4(11)的视盘没问题，但是由于图像原因本身黄斑处一片黑，没找出/4（14）
	//threshold(OpticDiskROI_Dilate_Erode, bwOpticDiskROI, 242, 255, CV_THRESH_BINARY); // 1.1/1.5 /3(16) 
	//threshold(OpticDiskROI_Dilate_Erode, bwOpticDiskROI, 254, 255, CV_THRESH_BINARY);  // 2(1) 但是21由于椭圆分割过大（背景太亮了）导致forve_ROI太远，没有包括fovea
	//threshold(OpticDiskROI_Dilate_Erode, bwOpticDiskROI, 220, 255, CV_THRESH_BINARY);  //2(11)


	Canny(bwOpticDiskROI, bwOpticDiskROI, 3, 30);
	vector<Point>points;
	for (int i = 0; i < bwOpticDiskROI.size().width; i++)
	{
		for (int j = 0; j < bwOpticDiskROI.size().height; j++)
		{
			if (0 != bwOpticDiskROI.at<uchar>(j, i))
			{
				Point currentOpticDiskPoint = Point(i, j);
				points.push_back(currentOpticDiskPoint);
			}
		}
	}
	RotatedRect ellipsemergeOD = fitEllipse(points);
	ellipse(bwOpticDiskROI, ellipsemergeOD, Scalar(255, 0, 0), 3);   
	

	//原图上覆盖视盘分割区域
	Mat srcImage_ODellipseCovered = src_ROIImg.clone();
	ellipsemergeOD.center.x += xmin;
	ellipsemergeOD.center.y += ymin;
	ellipse(srcImage_ODellipseCovered, ellipsemergeOD, Scalar(0, 0, 255), 5);//**********************在视盘处上画椭圆
	ellipseParam.push_back(ellipsemergeOD);

	cvtColor(srcImage_ODellipseCovered, srcImage_ODellipseCovered, CV_BGR2RGB);


	QImage *out_image1 = Mat2QImageRgb(srcImage_ODellipseCovered);
	this->setOutputImage(out_image1,0);

	//把视盘区域分割出来
	Mat ODsegment255 = Mat::zeros(src_ROIImg.size(), CV_8UC1);
	ellipse(ODsegment255, ellipsemergeOD, Scalar(255), -1);

	QImage *out_image2 = Mat2QImage(ODsegment255);
	this->setOutputImage(out_image2, 0);


	Mat ODsegment1 = Mat::zeros(src_ROIImg.size(), CV_8UC1);
	ellipse(ODsegment1, ellipsemergeOD, Scalar(1, 1, 1), -1);
	vector<Mat> ODsegmentChannels;
	for (int i = 0; i<3; i++)
	{
		ODsegmentChannels.push_back(ODsegment1);
	}

	Mat ODsegment3C = Mat::zeros(ODsegment255.rows, ODsegment255.cols, CV_8UC3);
	merge(ODsegmentChannels, ODsegment3C);
	Mat ODsegment = ODsegment3C.mul(src_ROIImg);
	cvtColor(ODsegment, ODsegment, CV_BGR2RGB);
	
	//视盘区域单独分割出来
	/*QImage *out_image3 = Mat2QImageRgb(ODsegment);
	this->setOutputImage(out_image3,0);*/



}

long OpticDiskSeg::run(){

    QImage *BVimg = transObj.inputImage.at(0);  
	QImage *tempimg = transObj.inputImage.at(1);
	QImage::Format format = tempimg->format();
	

	if (format == QImage::Format_Indexed8)
	{

		return ERROR_INVALID_IMGTYPE;
	}
	if (!(ListOfParam.at(0).value.nVal >= 0 || ListOfParam.at(0).value.nVal==-1))
	{
		return ERROR_INVALID_ARG;
	}

	Mat img = QImage2Mat(*tempimg);
	Mat BVimgMat = QImage2Mat(*BVimg);
	int SegmentType = 0;
	if (format == QImage::Format_RGB32 || format == QImage::Format_ARGB32)
	{

		cvtColor(img, img, CV_RGBA2RGB);
	}
	if (ListOfParam.at(0).value.nVal==-1)
	{
		SegmentType = 8;
	}
	ODSegment(img, BVimgMat, ListOfParam.at(0).value.nVal, SegmentType);
	return ALL_OK;
}