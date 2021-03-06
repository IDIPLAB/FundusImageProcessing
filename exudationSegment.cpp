//#include<iostream>
//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//#include<cmath>
//#include<stack>
//
//using namespace cv;
//using namespace std;
////matlab strel('disk',Radius)  
//Mat strelDisk(int Radius)
//{
//	int borderWidth; Mat sel; int m, n;
//	switch (Radius){
//	case 1:
//	case 2:
//		if (Radius == 1)
//			borderWidth = 1;
//		else
//			borderWidth = 2;
//		sel = Mat((2 * Radius + 1), (2 * Radius + 1), CV_8U, cv::Scalar(1));
//		break;//当半径为1时是3X3的 ,当半径为2时是5X5的  
//	case 3:
//		borderWidth = 0;
//		sel = Mat((2 * Radius - 1), (2 * Radius - 1), CV_8U, cv::Scalar(1));
//		break;
//	default:
//		n = Radius / 7; m = Radius % 7;
//		if (m == 0 || m >= 4)
//			borderWidth = 2 * (2 * n + 1);
//		else
//			borderWidth = 2 * 2 * n;
//		sel = Mat((2 * Radius - 1), (2 * Radius - 1), CV_8U, cv::Scalar(1));
//		break;
//	}
//	for (int i = 0; i < borderWidth; i++){
//		for (int j = 0; j < borderWidth; j++){
//			if (i + j < borderWidth){
//				sel.at<uchar>(i, j) = 0;
//				sel.at<uchar>(i, sel.cols - 1 - j) = 0;
//				sel.at<uchar>(sel.rows - 1 - i, j) = 0;
//				sel.at<uchar>(sel.rows - 1 - i, sel.cols - 1 - j) = 0;
//			}
//		}
//	}
//	return sel;
//}
//
//void fillHole(const Mat srcBw, Mat &dstBw)
//{
//	Size m_Size = srcBw.size();
//	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像
//	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));
//
//	cv::floodFill(Temp, Point(0, 0), Scalar(255));
//
//	Mat cutImg;//裁剪延展的图像
//	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);
//
//	dstBw = srcBw | (~cutImg);
//}
//inline void mat2gray(const Mat &src, Mat &dst)
//{
//	normalize(src, dst, 0, 1, NORM_MINMAX, src.type());
//	dst *= 255;
//	dst.convertTo(dst, CV_8UC1); //目前只适合单通
//
//}
//void edgeDetect(const Mat &srcImg, Mat &edgeImg, Mat & N_src_GROIImg,int temp[])
//{
//	vector<int> xLocation, yLocation;
//	for (int i = 0; i < srcImg.size().width; i++)
//	{
//		for (int j = 0; j < srcImg.size().height; j++)
//		{
//			if (srcImg.at<uchar>(j, i)>10)
//			{
//				xLocation.push_back(i);
//				yLocation.push_back(j);
//			}
//
//		}
//
//	}
//	int x_nozeromin, x_nozeromax, y_nozeromin, y_nozeromax;
//	x_nozeromin = *min_element(xLocation.begin(), xLocation.end());
//	x_nozeromax = *max_element(xLocation.begin(), xLocation.end());
//	y_nozeromin = *min_element(yLocation.begin(), yLocation.end());
//	y_nozeromax = *max_element(yLocation.begin(), yLocation.end());
//	int xlength = x_nozeromax - x_nozeromin;
//	int ylength = y_nozeromax - y_nozeromin;
//	Mat src_GCloneImg = srcImg.clone();
//	Mat src_GROIImg = src_GCloneImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1));
//	N_src_GROIImg = ~src_GROIImg;
//
//	// 提取边缘
//	Mat N_src_GROIImg_edge;
//	blur(N_src_GROIImg, N_src_GROIImg_edge, Size(3, 3));
//	Canny(N_src_GROIImg_edge, N_src_GROIImg_edge, 3, 30);
//	Mat fill_img;
//	fillHole(N_src_GROIImg_edge, fill_img);
//	//namedWindow("Canny边缘后填充", 0);
//	//imshow("Canny边缘后填充", fill_img);
//	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
//	Mat dilate_fill_img, erose_fill_img;
//	dilate(fill_img, dilate_fill_img, element);
//	erode(fill_img, erose_fill_img, element);
//	erode(erose_fill_img, erose_fill_img, element);
//	erode(erose_fill_img, erose_fill_img, element);
//	Mat edge_ROIimg = dilate_fill_img - erose_fill_img; //白色区域像素为255不是1，在后面直接点乘会有问题！
//	//namedWindow("轮廓提取", 0);
//	//imshow("轮廓提取", edge_ROIimg);
//	threshold(erose_fill_img, edgeImg, 100, 1, THRESH_BINARY); //将超过阈值的设为1，而不是255。
//	
//	temp[0] = x_nozeromin; temp[1] = x_nozeromax; temp[2] = y_nozeromin; temp[3] = y_nozeromax;
//}
//
//inline void morphReconstruct(Mat &marker, Mat &mask,Mat&dst)
//{
//
//	min(marker, mask, dst);
//	dilate(dst, dst, Mat());
//	min(dst, mask, dst);
//	Mat temp1 = Mat(marker.size(), CV_8UC1);
//	Mat temp2 = Mat(marker.size(), CV_8UC1);
//	do
//	{
//		dst.copyTo(temp1);
//		dilate(dst, dst, Mat());
//		cv::min(dst, mask, dst);
//		compare(temp1, dst, temp2, CV_CMP_NE);
//	} while (sum(temp2).val[0] != 0);
//
//}
//void stdfilt(const Mat &src, Mat &dst,int size) //大小一样，dst为float型。
//{
//	Mat mask = Mat::ones(size, size, CV_32FC1);
//	float summask = pow(size,2);
//	float n1summask = summask - 1.0;
//
//	Mat powimg = Mat(src.size(), src.type());
//	Mat squreimg = Mat(src.size(), src.type());
//	for (int i = 0; i < src.size().width; i++)
//	{
//		for (int j = 0; j < src.size().height; j++)
//		{	
//			powimg.at<float>(j, i) = pow(src.at<float>(j, i), 2);
//			//squreimg.at<float>(j, i) = sqrt(src.at<float>(j, i));
//		}
//	}
//
//	Mat c1, c2;
//	
//	filter2D(powimg, c1, -1, mask / n1summask, Point(-1, -1), BORDER_CONSTANT);
//	filter2D(src, c2, -1, mask, Point(-1, -1), BORDER_CONSTANT);
//	
//	
//	float maxtemp;
//	for (int i = 0; i < c2.size().width; i++)
//	{
//		for (int j = 0; j < c2.size().height; j++)
//		{
//			c2.at<float>(j, i) = pow(c2.at<float>(j, i), 2) / (summask*n1summask);
//			maxtemp = max<float>(c1.at<float>(j, i) - c2.at<float>(j, i), 0);
//			dst.at<float>(j, i) = sqrt(maxtemp);
//		}
//	}
//
//}
//
//
//
//
//int main()
//{
//	//Mat srcImage = imread("313.jpg");  //总感觉显示出来的图像和matlab不一样
//	Mat srcImage = imread("image001.png");  //总感觉显示出来的图像和matlab不一样
//	namedWindow("原图", 0);
//	imshow("原图", srcImage);
//	Mat hsvImg;
//	cvtColor(srcImage, hsvImg, CV_RGB2HSV);
//
//
//
//	vector<Mat> HSVchannels;
//	split(hsvImg, HSVchannels);
//	Mat imageHueChannel, imageSaturationChannel, imgValueChannel;
//	imageHueChannel = HSVchannels.at(0);
//	imageSaturationChannel = HSVchannels.at(1);
//	imgValueChannel = HSVchannels.at(2);
//	double V_Mean;
//	V_Mean = mean(imgValueChannel)[0];
//	Mat hsvNormImg1 = hsvImg.clone();
//	//cout << V_Mean;
//	if (V_Mean < 89)
//	{
//		for (int i = 0; i < hsvNormImg1.size().width; i++)
//		{
//			for (int j = 0; j < hsvNormImg1.size().height; j++)
//			{
//				hsvNormImg1.at<Vec3b>(j, i)[2] = saturate_cast<uchar>(1.3*imgValueChannel.at<uchar>(j, i));
//				hsvNormImg1.at<Vec3b>(j, i)[1] = saturate_cast<uchar>(1.2*imageSaturationChannel.at<uchar>(j, i));
//				hsvNormImg1.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(imageHueChannel.at<uchar>(j, i));   //H通道变为180的
//			}
//		}
//	}
//	Mat srcNormImg1;
//	cvtColor(hsvNormImg1, srcNormImg1, CV_HSV2RGB);
//	//namedWindow("srcNormImg", 0);
//	//imshow("srcNormImg", srcNormImg);
//
//	//Mat srcGrayImg;
//	//cvtColor(srcNormImg, srcGrayImg, CV_RGB2GRAY);
//	vector<Mat> RGBchannels1;
//	split(srcNormImg1, RGBchannels1);
//	Mat src_GImag = RGBchannels1.at(1);
//
//	//寻找绿色通道图像上的上下边界，opencv没有matlab的find函数！！QAQ
//	//MatIterator_<uchar> iterBegin = src_GImag.begin<uchar>();
//	//MatIterator_<uchar> iterEnd = src_GImag.end<uchar>();
//	Mat edgeImg, N_src_GROIImg;
//	int minMaxPara[4];
//	edgeDetect(src_GImag, edgeImg, N_src_GROIImg, minMaxPara);
//	int x_nozeromin = minMaxPara[0];
//	int x_nozeromax = minMaxPara[1];
//	int y_nozeromin = minMaxPara[2];
//	int y_nozeromax = minMaxPara[3];
//
//
//
//
//
//	Mat hsvNormImg = hsvImg.clone();
//	if (V_Mean < 89)
//	{
//		for (int i = 0; i < hsvNormImg.size().width; i++)
//		{
//			for (int j = 0; j < hsvNormImg.size().height; j++)
//			{
//				hsvNormImg.at<Vec3b>(j, i)[2] = 1.5*imgValueChannel.at<uchar>(j, i); 
//				hsvNormImg.at<Vec3b>(j, i)[1] = saturate_cast<uchar>(imageSaturationChannel.at<uchar>(j, i));
//				hsvNormImg.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(imageHueChannel.at<uchar>(j, i));
//			}
//		}
//	}
//	Mat srcNormImg;
//	cvtColor(hsvNormImg, srcNormImg, CV_HSV2RGB);
//	namedWindow("HSV通道处理后的原图", 0);
//	imshow("HSV通道处理后的原图", srcNormImg);
//	Mat graySrcNormImg;
//	cvtColor(srcNormImg, graySrcNormImg, CV_RGB2GRAY);
//	vector<Mat> srcNormChannels;
//	split(srcNormImg, srcNormChannels);
//	Mat imageNormChannel = srcNormChannels.at(1);
//	Mat CLAHE_GsrcImage;
//	Ptr<CLAHE> clahe = createCLAHE();
//	clahe->setClipLimit(3);
//	clahe->apply(imageNormChannel, CLAHE_GsrcImage);
//	//normalize(CLAHE_GsrcImage, CLAHE_GsrcImage, 0, 255, NORM_MINMAX, CLAHE_GsrcImage.type());
//	//namedWindow("CLAHE对比度增强", 0);
//	//imshow("CLAHE对比度增强", CLAHE_GsrcImage);
//	Mat prepsrcImg;
//	medianBlur(CLAHE_GsrcImage, prepsrcImg, 15); //滤波器必须是奇数
//
//
//	Mat srcGNormImg = prepsrcImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1)).clone();
//
//	
//
//
//
//
//	//namedWindow("对比度增强ROI", 0);
//	//imshow("对比度增强ROI", srcGNormImg);
//
//	//中值滤波和形态重建
//	Mat medSrcNormImg;
//	medianBlur(srcGNormImg, medSrcNormImg, 49); //滤波器必须是奇数
//	Mat srcGNormReconstruct;
//	morphReconstruct(medSrcNormImg, srcGNormImg, srcGNormReconstruct);//速度慢
//	//namedWindow("形态学重建", 0);
//	//imshow("形态学重建", srcGNormReconstruct);
//	Mat rebuiltImg = srcGNormImg - srcGNormReconstruct;
//	//namedWindow("重建后图像", 0);
//	//imshow("重建后图像", rebuiltImg);
//	Mat bwReconstrucImg;
//	threshold(rebuiltImg, rebuiltImg, 21, 255, CV_THRESH_BINARY);
//	namedWindow("二值化重建图像", 0);
//	imshow("二值化重建图像", rebuiltImg);
//	
//	//闭操作去除血管
//	namedWindow("原灰度图", 0);
//	imshow("原灰度图", graySrcNormImg);
//	Mat closeElement = strelDisk(30); //结构元一样
//	//Mat closeElement = getStructuringElement(MORPH_ELLIPSE, Size(99, 99)); //matlab和opencv的结构元是不一样的
//	Mat graySrcNormImgRoi = graySrcNormImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1)).clone();
//	Mat grayopecnImg;
//	morphologyEx(graySrcNormImg, graySrcNormImg, MORPH_CLOSE, closeElement, Point(-1, -1), 2,BORDER_REFLECT); //速度最慢！
//	grayopecnImg = graySrcNormImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1)).clone();
//	namedWindow("血管去除图像", 0);
//	imshow("血管去除图像", grayopecnImg);
//
//	//计算局部方差提取明亮区域
//	
//	Mat Float_graySrcNormImgRoi;
//	grayopecnImg.convertTo(Float_graySrcNormImgRoi, CV_32FC1);
//	normalize(Float_graySrcNormImgRoi, Float_graySrcNormImgRoi, 0, 1, NORM_MINMAX, CV_32FC1); //将范围缩小到0-1
//
//	Mat stdflitImg = Mat(Float_graySrcNormImgRoi.size(), Float_graySrcNormImgRoi.type());
//	stdfilt(Float_graySrcNormImgRoi, stdflitImg,9);//修改这个参数
//	Mat edgeImgFloat;
//	edgeImg.convertTo(edgeImgFloat, CV_32FC1);
//	Mat stdshow;
//	stdflitImg = stdflitImg.mul(edgeImgFloat);
//	
//	mat2gray(stdflitImg, stdshow);
//	namedWindow("stdshow", 0);
//	imshow("stdshow", stdshow);
//
//
//	//mat2gray(stdflitImg, stdflitImg);
//	//namedWindow("局部方差图", 0);
//	//imshow("局部方差图", stdflitImg);
//
//
//	//局部标准方差图还是有问题，值太小了。即使全部分出来也仅仅少数
//	Mat openElement2 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
//	Mat openElement3 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
//	Mat openElement4 = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
//	Mat stdflitImg_morph,bwstdImg_morph1;
//	morphologyEx(stdflitImg, stdflitImg_morph, MORPH_OPEN, openElement2, Point(-1, -1), 1);
//	threshold(stdflitImg_morph, bwstdImg_morph1, 0.006, 255, CV_THRESH_BINARY);
//	namedWindow("test", 0);
//	imshow("test", bwstdImg_morph1);
//
//	morphologyEx(bwstdImg_morph1, bwstdImg_morph1, MORPH_DILATE, openElement3, Point(-1, -1), 1);
//	morphologyEx(bwstdImg_morph1, bwstdImg_morph1, MORPH_CLOSE, openElement4, Point(-1, -1), 1);
//	bwstdImg_morph1.convertTo(bwstdImg_morph1, CV_8UC1);
//	stdflitImg_morph = bwstdImg_morph1.mul(edgeImg); //stdflitImg_morph是32位
//	Mat morph_Fill_Img;
//	fillHole(stdflitImg_morph, morph_Fill_Img);
//	namedWindow("morph_Fill_Img", 0);
//	imshow("morph_Fill_Img", morph_Fill_Img);
//
//	Mat exudateROI = morph_Fill_Img + rebuiltImg;
//	//exudateROI要变成0-1
//	Mat exudateROI01;
//	Mat N_exudateROI01 = ~exudateROI;
//	threshold(N_exudateROI01, exudateROI01, 100, 1, CV_THRESH_BINARY);
//	Mat black_exudateROI = srcGNormImg.mul(exudateROI01);
//	namedWindow("black_exudateROI", 0);
//	imshow("black_exudateROI", black_exudateROI);
//
//	//第二次形态学重建
//	Mat ExudateReconstruct;
//	morphReconstruct(black_exudateROI, srcGNormImg, ExudateReconstruct);
//	Mat FinalExuadate = srcGNormImg - ExudateReconstruct;
//	//namedWindow("重建后图像", 0);
//	//imshow("重建后图像", rebuiltImg);
//	Mat bwExudate;
//	threshold(FinalExuadate, bwExudate, 0, 255, CV_THRESH_BINARY);
//	namedWindow("渗出重建图像", 0);
//	imshow("渗出重建图像", bwExudate);
//
//
//
//
//	/*morphologyEx(stdflitImg_morph, stdflitImg_morph, MORPH_OPEN, openElement2, Point(-1, -1), 1);
//	morphologyEx(stdflitImg, stdflitImg_morph, MORPH_OPEN, openElement2, Point(-1, -1), 1);*/
//
//
//	waitKey(0);
//	return 0;
//}