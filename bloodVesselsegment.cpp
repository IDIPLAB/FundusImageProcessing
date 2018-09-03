//#include<iostream>
//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//
//
//using namespace cv;
//using namespace std;
//
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
//	namedWindow("Canny边缘后填充", 0);
//	imshow("Canny边缘后填充", fill_img);
//	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
//	Mat dilate_fill_img, erose_fill_img;
//	dilate(fill_img, dilate_fill_img, element);
//	erode(fill_img, erose_fill_img, element);
//	erode(erose_fill_img, erose_fill_img, element);
//	erode(erose_fill_img, erose_fill_img, element);
//	Mat edge_ROIimg = dilate_fill_img - erose_fill_img; //白色区域像素为255不是1，在后面直接点乘会有问题！
//	namedWindow("轮廓提取", 0);
//	imshow("轮廓提取", edge_ROIimg);
//	threshold(erose_fill_img, edgeImg, 100, 1, THRESH_BINARY); //将超过阈值的设为1，而不是255。
//	temp[0] = x_nozeromin; temp[1] = x_nozeromax; temp[2] = y_nozeromin; temp[3] = y_nozeromax;
//}
//
//
//int main()
//{
//	double AreaThreshold = 1100; 
//	int thresholdvalue = 15;
//	Mat srcImage = imread("411.jpg");
//	Mat hsvImg;
//	cvtColor(srcImage, hsvImg, CV_RGB2HSV);
//	vector<Mat> HSVchannels;
//	split(hsvImg, HSVchannels);
//	Mat imageHueChannel, imageSaturationChannel, imgValueChannel;
//	imageHueChannel = HSVchannels.at(0);
//	imageSaturationChannel = HSVchannels.at(1);
//	imgValueChannel = HSVchannels.at(2);
//	double V_Mean;
//	V_Mean = mean(imgValueChannel)[0];
//	Mat hsvNormImg = hsvImg.clone();
//	cout << V_Mean;
//	if (V_Mean < 89)
//	{
//		for (int i = 0; i < hsvNormImg.size().width; i++)
//		{
//			for (int j = 0; j < hsvNormImg.size().height; j++)
//			{
//				hsvNormImg.at<Vec3b>(j, i)[2] = saturate_cast<uchar>(1.3*imgValueChannel.at<uchar>(j, i));
//				hsvNormImg.at<Vec3b>(j, i)[1] = saturate_cast<uchar>(1.2*imageSaturationChannel.at<uchar>(j, i));
//				hsvNormImg.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(imageHueChannel.at<uchar>(j, i));   //H通道变为180的
//			}
//		}
//	}
//	Mat srcNormImg;
//	cvtColor(hsvNormImg, srcNormImg,CV_HSV2RGB);
//	namedWindow("srcNormImg", 0);
//	imshow("srcNormImg", srcNormImg);
//	//Mat srcGrayImg;
//	//cvtColor(srcNormImg, srcGrayImg, CV_RGB2GRAY);
//	vector<Mat> RGBchannels;
//	split(srcNormImg, RGBchannels);
//	Mat src_GImag = RGBchannels.at(1);
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
//	//中值滤波
//	Mat medBlurImg;
//	medianBlur(N_src_GROIImg, medBlurImg, 41); //滤波器必须是奇数
//	Mat medBlurRoiImg = medBlurImg.mul(edgeImg);
//
//	Mat tophatElement = getStructuringElement(MORPH_ELLIPSE, Size(20, 20));
//	Mat blackhatElement = getStructuringElement(MORPH_ELLIPSE, Size(6, 6));
//	Mat lineElement = getStructuringElement(MORPH_RECT, Size(1, 8));
//	Mat topMedblurImg, bhatMedblurImg;
//	morphologyEx(N_src_GROIImg, topMedblurImg, MORPH_TOPHAT, tophatElement);
//	morphologyEx(N_src_GROIImg, bhatMedblurImg, MORPH_TOPHAT, blackhatElement);
//	Mat TopBlackImg;
//	//TopBlackImg = 1.2*topMedblurImg - 1.1*bhatMedblurImg + N_src_GROIImg - medBlurImg;
//	TopBlackImg = 0.8*topMedblurImg - 0.8*bhatMedblurImg + N_src_GROIImg - medBlurImg;
//	TopBlackImg = TopBlackImg.mul(edgeImg);
//	namedWindow("TopBlackImg", 0);
//	imshow("TopBlackImg", TopBlackImg);
//	Mat binaryTopBlackImg;
//	//threshold(TopBlackImg, binaryTopBlackImg, 10, 255, THRESH_BINARY);
//	
//	threshold(TopBlackImg, binaryTopBlackImg, thresholdvalue, 255, THRESH_BINARY);
//	morphologyEx(binaryTopBlackImg, binaryTopBlackImg, MORPH_CLOSE, lineElement);
//
//
//	// 保留具有一定面积连通域  
//	vector<vector<Point>> contours;
//	findContours(binaryTopBlackImg, contours, RETR_TREE, CHAIN_APPROX_NONE);
//
//	
//	vector<vector<Point>> maxContour;
//	vector<vector<Point>> ::iterator ContourIter = contours.begin();
//	double area = 0;
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		double area = contourArea(contours[i]);
//		if (area > AreaThreshold)
//		{
//			maxContour.push_back(contours[i]);
//		}
//	}
//	Mat Bloodvesselresult(binaryTopBlackImg.size(),CV_8U,Scalar(0));
//	drawContours(Bloodvesselresult, maxContour, -1, Scalar(255), -1);   //第一个-1，绘制所有轮廓；第二个-1，进行轮廓内外填充
//
//	//414图片如果用-1会出BUG
//	
//	Mat srcRoiImage = srcImage(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1));
//	namedWindow("srcRoiImage", 0);
//	imshow("srcRoiImage", srcRoiImage);
//
//	namedWindow("Bloodvesselresult", 0);
//	imshow("Bloodvesselresult", Bloodvesselresult);
//	
//	waitKey(0);
//	return 0;
//}