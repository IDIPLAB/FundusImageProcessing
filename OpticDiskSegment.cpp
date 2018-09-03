//#include<iostream>
//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//#include<cmath>
//#include<stack>
//
////写一个视盘类的时候，记得保存boudingbox，继承于血管类；然后在黄斑类里面继承视盘基类，得到一个boudingbox
//using namespace cv;
//using namespace std;
//
//inline void mat2gray(const Mat &src, Mat &dst)
//{
//	normalize(src, dst, 0, 1, NORM_MINMAX, src.type());
//	dst *= 255;
//	dst.convertTo(dst, CV_8UC1); //目前只适合单通
//
//}
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
//Point GetCenterPoint(const Mat &src) //输入二值图
//{
//	int i, j;
//	double x0 = 0, y0 = 0, sum = 0;
//	Point center;
//	double pixel;
//	for (i = 0; i < src.size().width; i++)
//	{
//		for (j = 0; j < src.size().height; j++)
//		{
//			pixel = src.at<uchar>(j, i);
//			x0 = x0 + i*pixel;
//			y0 = y0 + j*pixel;
//			sum = sum + pixel;
//		}
//	}
//	center.x = x0 / sum;
//	center.y = y0 / sum;
//	return center;
//}
//void fillHole(const Mat srcBw, Mat &dstBw)  //好像没用到
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
//void getConnectedDomain(Mat& src, vector<Rect>& boundingbox)//boundingbox为最终结果，存放各个连通域的包围盒  
//{
//	int img_row = src.rows;
//	int img_col = src.cols;
//	Mat flag = Mat::zeros(Size(img_col, img_row), CV_8UC1);//标志矩阵，为0则当前像素点未访问过  
//	for (int i = 0; i < img_row; i++)
//	{
//		for (int j = 0; j < img_col; j++)
//		{
//			if (src.ptr<uchar>(i)[j] == 255 && flag.ptr<uchar>(i)[j] == 0)
//			{
//				stack<Point2f> cd;
//				cd.push(Point2f(j, i));
//				flag.ptr<uchar>(i)[j] = 1;
//				int minRow = i, minCol = j;//包围盒左、上边界  
//				int maxRow = i, maxCol = j;//包围盒右、下边界  
//				while (!cd.empty())
//				{
//					Point2f tmp = cd.top();
//					if (minRow > tmp.y)//更新包围盒  
//						minRow = tmp.y;
//					if (minCol > tmp.x)
//						minCol = tmp.x;
//					if (maxRow < tmp.y)
//						maxRow = tmp.y;
//					if (maxCol < tmp.x)
//						maxCol = tmp.x;
//					cd.pop();
//					Point2f p[4];//邻域像素点，这里用的四邻域  
//					p[0] = Point2f(tmp.x - 1 > 0 ? tmp.x - 1 : 0, tmp.y);
//					p[1] = Point2f(tmp.x + 1 < img_col - 1 ? tmp.x + 1 : img_row - 1, tmp.y);
//					p[2] = Point2f(tmp.x, tmp.y - 1 > 0 ? tmp.y - 1 : 0);
//					p[3] = Point2f(tmp.x, tmp.y + 1 < img_row - 1 ? tmp.y + 1 : img_row - 1);
//					for (int m = 0; m < 4; m++)
//					{
//						int x = p[m].y;
//						int y = p[m].x;
//						if (src.ptr<uchar>(x)[y] == 255 && flag.ptr<uchar>(x)[y] == 0)//如果未访问，则入栈，并标记访问过该点  
//						{
//							cd.push(p[m]);
//							flag.ptr<uchar>(x)[y] = 1;
//						}
//					}
//				}
//				Rect rect(Point2f(minCol, minRow), Point2f(maxCol + 1, maxRow + 1));
//				boundingbox.push_back(rect);
//			}
//		}
//	}
//}
//void myphong(const Mat src, Mat&dst, int param[4])
//{
//	int centerx = param[0];
//	int centery = param[1];
//	int width = param[2];
//	int height = param[3];
//	int halfWidth = floor(width / 2);
//	int halfHeight = floor(height / 2);
//	//构造漫反射和镜面反射模板
//	float I1 = 8, z = 67.5, Fn = 1;
//	float Kd = 0.08; //漫反射系数
//	float Ks = 2.8;  //镜面反射系数
//	Mat cosxy = Mat::zeros(Size(width, height), CV_32FC1);
//	Mat cosfy = Mat::zeros(Size(width, height), CV_32FC1);
//	float temp = 0;
//	int radiusCircle = 0;
//	for (int i = 0; i < width; i++)
//	{
//		for (int j = 0; j < height; j++)
//		{
//			if (pow(j-halfHeight,2)+pow(i-halfWidth,2)<=pow(radiusCircle,2))	//内部小圆处理
//			{
//				//cosxy.at<float>(j, i) = 0;
//				//cosfy.at<float>(j, i) = 0;
//				temp = z / sqrt(pow(i - halfWidth, 2) + pow((j - halfHeight), 2) + pow(z, 2));
//				cosxy.at<float>(j, i) = temp;
//				cosfy.at<float>(j, i) = pow(2 * pow(temp, 2) - 1, Fn);
//			}
//			else
//			{
//				temp = z / sqrt( pow(i - halfWidth, 2) + pow((j - halfHeight), 2) + pow(z,2));
//				cosxy.at<float>(j, i) = temp;
//				cosfy.at<float>(j, i) = pow(2 * pow(temp, 2) - 1, Fn);
//				if (2 * pow(temp, 2) - 1 < 0)
//				{
//					cosfy.at<float>(j, i) = 0;
//				}
//			}
//		}
//	}
//	Mat DUFmask = I1*Kd*cosxy;  //
//	Mat DIFmask = I1*Ks*cosfy;
//
//	Mat DUFmaskShow; mat2gray(DIFmask, DUFmaskShow);
//	namedWindow("DUFmaskShow", 0);
//	imshow("DUFmaskShow", DUFmaskShow); 
//
//	Mat Ibground = DUFmask + DIFmask;
//	Mat floatsrcImg;
//	dst.convertTo(dst, CV_32FC1);
//
//	//ROI处理
//	floatsrcImg = dst(Range(centery - halfHeight, centery + halfHeight + 1), Range(centerx - halfWidth, centerx + halfWidth + 1));//Range右不包含
//	double a = DUFmask.at<float>(71, 71);
//	double b = DIFmask.at<float>(71, 71);
//	floatsrcImg -= Ibground;
//
//	dst.convertTo(dst, CV_8UC1);
//
//
//
//
//}
//
//
//
//int main()
//{
//	Mat srcImage = imread("ROIorigin_1 (1).jpg");  //1.2/1.8/2(12)/2（13）/3(14)/4(11)/4（14）
//	Mat BloodvesselImage_RGB = imread("Vessel_1 (1).jpg"); //居然是三通道的图。。
//	namedWindow("srcImg", 0);
//	imshow("srcImg", srcImage);
//	Mat BloodvesselImage;
//	cvtColor(BloodvesselImage_RGB, BloodvesselImage, CV_RGB2GRAY);
//
//	vector<Mat> RGBchannels;
//	split(srcImage, RGBchannels);
//	Mat imageRChannel, imageGChannel;
//	imageRChannel = RGBchannels.at(2).clone();
//	imageGChannel = RGBchannels.at(1).clone();
//	threshold(imageRChannel, imageRChannel, 250, 255, THRESH_BINARY);
//	threshold(imageGChannel, imageGChannel, 160, 255, THRESH_BINARY);
//	Mat RGthreshold_Image;
//	RGthreshold_Image = imageGChannel.mul(imageRChannel);
//
//	//连通域扩张
//	vector<Rect> boundingbox;
//	getConnectedDomain(RGthreshold_Image, boundingbox);
//
//	Mat bwExpandImg(imageGChannel.size(), CV_8U, Scalar(0));
//	for (size_t i = 0; i < boundingbox.size(); i++)
//	{
//		Rect currentbox = boundingbox[i];
//		if (currentbox.x>0.5*currentbox.width)
//		{
//			currentbox.x = currentbox.x - 0.5*currentbox.width;
//		}
//		else
//		{
//			currentbox.x = currentbox.x;
//		}
//		if (currentbox.x + 2 * currentbox.width<srcImage.size().width)
//		{
//			currentbox.width = 2 * currentbox.width;
//		}
//		else
//		{
//			currentbox.width = srcImage.size().width - currentbox.x + currentbox.width - 1;
//		}
//		rectangle(bwExpandImg, currentbox, Scalar(255),-1);
//	}
//	Mat bwExpandImg1;
//	threshold(bwExpandImg, bwExpandImg1, 100, 1, THRESH_BINARY);
//	Mat srcExpandBVImg;
//	srcExpandBVImg = bwExpandImg1.mul(BloodvesselImage);
//
//	//滤波模板
//	float RMask[9][9] = { { 135, 150, 165, 180, 180, 90, 75, 60, 45 }, { 120, 135, 150, 165, 180, 75, 60, 45, 30 }, { 105, 120, 135, 150, 180, 60, 45, 30, 15 },
//	{ 90, 105, 120, 135, 180, 45, 30, 15, 0 }, { -20, -20, -20, -20, 180, -20, -20, -20, -20 }, { 90, 105, 120, 135, 180, 45, 30, 15, 0 }, { 105, 120, 135, 150, 180, 60, 45, 30, 15 },
//	{ 120, 135, 150, 165, 180, 75, 60, 45, 30 }, { 135, 150, 165, 180, 180, 90, 75, 60, 45 } };
//
//	float LMask[9][9] = { { 45, 60, 75, 90, 180, 180, 165, 150, 135 }, { 30, 45, 60, 75, 180, 165, 150, 135, 120 }, { 15, 30, 45, 60, 180, 150, 135, 120, 105 },
//	{ 0, 15, 30, 45, 180, 135, 120, 105, 90 }, { -20, -20, -20, -20, 180, -20, -20, -20, -20 }, { 0, 15, 30, 45, 180, 135, 120, 105, 90 }, { 15, 30, 45, 60, 180, 150, 135, 120, 105 },
//	{ 30, 45, 60, 75, 180, 165, 150, 135, 120 }, { 45, 60, 75, 90, 180, 180, 165, 150, 135 } };
//	
//	Mat Rmaskmetrix = Mat(Size(9, 9), CV_32F, RMask);
//	//Mat Lmaskmetrix = Mat(Size(9, 9), CV_32FC1, LMask);
//	Mat Lmaskmetrix = Mat(Size(9, 9), CV_32F, LMask);
//	int leftNumber = 0, rightNumber = 0;
//	for (int i = 0; i < srcExpandBVImg.size().width; i++)
//	{
//		for (int j = 0; j < srcExpandBVImg.size().height; j++)
//		{
//			if (0!=srcExpandBVImg.at<uchar>(j, i))
//			{
//				if (i<0.5*srcExpandBVImg.size().width)
//				{
//					leftNumber++;
//				}
//				else
//				{
//					rightNumber++;
//				}
//			}
//
//		}
//	}
//	Mat Lmask;
//		
//	Mat Rmask(Size(81, 61), Rmaskmetrix.type());
//	Mat mask;
//	resize(Lmaskmetrix, Lmask, Size(0, 0), 20.0 / 9, 20.0 / 9, CV_INTER_CUBIC);//resize不支持int型
//	resize(Rmaskmetrix, Rmask, Rmask.size(), 0.5, 0.5, CV_INTER_CUBIC);
//	if (leftNumber>rightNumber)
//	{
//		mask = Lmask.clone();
//	}
//	else
//	{
//		mask = Rmask.clone();
//	}
//	//显示各部分模板
//	Mat LmaskmetrixShow, RmaskmetrixShow;
//	mat2gray(Lmask, LmaskmetrixShow);
//	//mat2gray(Lmaskmetrix, LmaskmetrixShow);
//	Mat maskShow;
//	mat2gray(mask, maskShow);
//
//	Mat filter_ExpandBVImg(srcExpandBVImg.size(), CV_32F); //滤波后输出
//
//	//对模板矩阵旋转180度
//	Point center = Point(mask.cols / 2, mask.rows / 2);
//	Mat rotmat=getRotationMatrix2D(center, 180, 1);
//	Mat maskRotate(mask.size(), mask.type());
//	warpAffine(mask, maskRotate, rotmat, mask.size());
//
//	Mat float_srcExpandBVImg;
//	srcExpandBVImg.convertTo(float_srcExpandBVImg, CV_32F);
//	
//	Scalar Bordervalue = Scalar(0);
//	int top = 0.5*mask.rows, bottom = top;
//	int left = 0.5*mask.cols, right = left;
//	copyMakeBorder(float_srcExpandBVImg, float_srcExpandBVImg, top, bottom, left, right, BORDER_CONSTANT, Bordervalue);
//	filter2D(float_srcExpandBVImg, filter_ExpandBVImg, -1, maskRotate, Point(-1, -1), 0,BORDER_CONSTANT);
//	//////////////滤波结果不一样，matlab默认是FULL，应该是用0填。 上面这样补零了还是差几个像素
//
//	//中心定位和画十字符号
//	Point OpticDiskCenter;
//	double maxValue;
//	minMaxLoc(filter_ExpandBVImg, NULL,&maxValue, NULL, &OpticDiskCenter);
//	Mat filter_ExpandBVImgShow;
//	mat2gray(filter_ExpandBVImg, filter_ExpandBVImgShow);
//	Mat srcImage_ODLine = srcImage.clone();
//	line(srcImage_ODLine, Point(OpticDiskCenter.x - 20, OpticDiskCenter.y), Point(OpticDiskCenter.x + 20, OpticDiskCenter.y), CV_RGB(255, 0, 0), 10, CV_AA, 0);
//	line(srcImage_ODLine, Point(OpticDiskCenter.x, OpticDiskCenter.y - 20), Point(OpticDiskCenter.x, OpticDiskCenter.y + 20), CV_RGB(255, 0, 0), 10, CV_AA, 0);
//	namedWindow("OpticDiskCenter", 0);
//	imshow("OpticDiskCenter", srcImage_ODLine);
//
//	//基于中心定位的视盘ROI区域确定
//	int roughrx = 220, roughry = 200;
//	int ymin, ymax, xmin, xmax;
//	int ODcenterX = OpticDiskCenter.x; int ODcenterY = OpticDiskCenter.y;
//	
//	if (ODcenterX - roughrx <= 0)
//	{
//		xmin = 1;
//	}
//	else xmin = ODcenterX - roughrx;
//
//	if (ODcenterX+roughrx>=srcImage.size().width)
//	{
//		xmax = srcImage.size().width;
//	}
//	else xmax = ODcenterX + roughrx;
//
//	if (ODcenterY - roughry <= 0)
//	{
//		ymin = 1;
//	}
//	else ymin = ODcenterY - roughry;
//
//	if (ODcenterY + roughry >= srcImage.size().height)
//	{
//		ymax = srcImage.size().height;
//	}
//	else ymax = ODcenterY + roughry;
//
//	//处理周围黑色背景转成白色
//	Vec3b zeroChangedVec; zeroChangedVec[0] = 255; zeroChangedVec[1] = 255; zeroChangedVec[2] = 255;
//	Mat Cut_OpticDiskROI = srcImage(Range(ymin, ymax), Range(xmin, xmax)).clone();
//	for (int i = 0; i < Cut_OpticDiskROI.size().width; i++)
//	{
//		for (int j = 0; j < Cut_OpticDiskROI.size().height; j++)
//		{
//			if (20> Cut_OpticDiskROI.at<Vec3b>(j, i)[0]&&         //2(16)在这边颜色阈值分割失败
//				20 > Cut_OpticDiskROI.at<Vec3b>(j, i)[1]&&
//				20 > Cut_OpticDiskROI.at<Vec3b>(j, i)[2])
//			{
//				Cut_OpticDiskROI.at<Vec3b>(j, i) = zeroChangedVec;
//			}
//		}
//	}
//	vector<Mat> OD_RGBchannels;
//	split(Cut_OpticDiskROI, OD_RGBchannels);
//
//	//处理内部血管和白色背景
//	Mat element20 = getStructuringElement(MORPH_ELLIPSE, Size(20, 20));
//	Mat element22 = getStructuringElement(MORPH_ELLIPSE, Size(22, 22));
//	Mat element24 = getStructuringElement(MORPH_ELLIPSE, Size(24, 24));
//	//Mat element65 = getStructuringElement(MORPH_ELLIPSE, Size(80, 80));
//	Mat element65 = strelDisk(65);
//	Mat OpticDiskROI_Dilate_Erode = OD_RGBchannels.at(2); //分出来的是BGR
//	for (int i = 0; i < OpticDiskROI_Dilate_Erode.size().width; i++)
//	{
//		for (int j = 0; j < OpticDiskROI_Dilate_Erode.size().height; j++)
//		{
//			if (0 == OpticDiskROI_Dilate_Erode.at<uchar>(j, i) )
//			{
//				OpticDiskROI_Dilate_Erode.at<uchar>(j, i)  = 255;
//			}
//		}
//	}
//	namedWindow("G通道", 0);
//	imshow("G通道", OpticDiskROI_Dilate_Erode);
//	morphologyEx(OpticDiskROI_Dilate_Erode, OpticDiskROI_Dilate_Erode, MORPH_CLOSE, element20, Point(-1, -1), 1);
//	morphologyEx(OpticDiskROI_Dilate_Erode, OpticDiskROI_Dilate_Erode, MORPH_OPEN, element22, Point(-1, -1), 1);
//	morphologyEx(OpticDiskROI_Dilate_Erode, OpticDiskROI_Dilate_Erode, MORPH_CLOSE, element24, Point(-1, -1), 1);
//	morphologyEx(OpticDiskROI_Dilate_Erode, OpticDiskROI_Dilate_Erode, MORPH_OPEN, element65,Point(-1, -1), 1);
//	
//	//椭圆拟合
//	Mat bwOpticDiskROI;
//	threshold(OpticDiskROI_Dilate_Erode, bwOpticDiskROI,0, 255, CV_THRESH_OTSU);  // 1.2/1.8/2(12)/2（13）/3(14)/4(11)的视盘没问题，但是由于图像原因本身黄斑处一片黑，没找出/4（14）
//	//threshold(OpticDiskROI_Dilate_Erode, bwOpticDiskROI,242,255, CV_THRESH_BINARY); // 1.1/1.5 
//	//threshold(OpticDiskROI_Dilate_Erode, bwOpticDiskROI, 254, 255, CV_THRESH_BINARY);  // 3(16) 2(1) 但是21由于椭圆分割过大（背景太亮了）导致forve_ROI太远，没有包括fovea
//	//threshold(OpticDiskROI_Dilate_Erode, bwOpticDiskROI, 220, 255, CV_THRESH_BINARY);  //2(11)
//	namedWindow("OpticDiskROI_Dilate_Erode", 0);
//	imshow("OpticDiskROI_Dilate_Erode", OpticDiskROI_Dilate_Erode);
//
//	Canny(bwOpticDiskROI, bwOpticDiskROI, 3, 30);
//	vector<Point>points;
//	for (int i = 0; i < bwOpticDiskROI.size().width; i++)
//	{
//		for (int j = 0; j < bwOpticDiskROI.size().height; j++)
//		{
//			if (0 != bwOpticDiskROI.at<uchar>(j, i))
//			{
//				Point currentOpticDiskPoint = Point(i, j);
//				points.push_back(currentOpticDiskPoint);
//			}
//		}
//	}
//	RotatedRect ellipsemergeOD = fitEllipse(points);
//	ellipse(bwOpticDiskROI, ellipsemergeOD, Scalar(255, 0, 0), 3);
//	imshow("椭圆拟合", bwOpticDiskROI);
//
//	//原图上覆盖视盘分割区域
//	Mat srcImage_ODellipseCovered = srcImage.clone();
//	ellipsemergeOD.center.x += xmin;
//	ellipsemergeOD.center.y += ymin;
//	ellipse(srcImage_ODellipseCovered, ellipsemergeOD, Scalar(0, 0, 255), 5);
//	namedWindow("原图椭圆拟合", 0);
//	imshow("原图椭圆拟合", srcImage_ODellipseCovered);
//
//	//把视盘区域分割出来
//	Mat ODsegment255 = Mat::zeros(srcImage.size(),CV_8UC1);
//	ellipse(ODsegment255, ellipsemergeOD, Scalar(255), -1);
//	Mat ODsegment1 = Mat::zeros(srcImage.size(), CV_8UC1);
//	ellipse(ODsegment1, ellipsemergeOD, Scalar(1, 1, 1), -1);
//	vector<Mat> ODsegmentChannels;
//	for (int i = 0; i<3; i++)
//	{
//		ODsegmentChannels.push_back(ODsegment1);
//	}
//
//	Mat ODsegment3C = Mat::zeros(ODsegment255.rows, ODsegment255.cols, CV_8UC3);
//	merge(ODsegmentChannels, ODsegment3C);
//	Mat ODsegment = ODsegment3C.mul(srcImage);
//	namedWindow("视盘分割", 0);
//	imshow("视盘分割", ODsegment);
//
//	/////////黄斑定位部分 需要提供视盘的ODsegment255图像  ellipsemergeOD
//	Mat imageGChannel_Fovea;
//	imageGChannel_Fovea = RGBchannels.at(1);
//	//中心区反光处理
//	int ReflectWidth = 151;
//	int ReflectHeight = 151;
//	int ReflectParams[4] = { 942, 777, ReflectWidth, ReflectHeight };
//	Mat phong_G_srcImage = imageGChannel_Fovea.clone();
//	namedWindow("imageGChannel_Fovea", 0);
//	imshow("imageGChannel_Fovea", imageGChannel_Fovea);
//	myphong(imageGChannel_Fovea, phong_G_srcImage, ReflectParams);
//
//	//自适应对比度增强以及去相关
//	Mat CLAHE_GsrcImage;
//	Mat Nphong_G_srcImage = ~phong_G_srcImage;
//	Ptr<CLAHE> clahe = createCLAHE();
//	clahe->setClipLimit(5);  //************************这个参数matlab不知道是啥
//	clahe->apply(Nphong_G_srcImage, CLAHE_GsrcImage);
//	Mat decorrelationGImg = CLAHE_GsrcImage - ~phong_G_srcImage;
//	normalize(decorrelationGImg, decorrelationGImg, 0, 255, NORM_MINMAX, decorrelationGImg.type()); //这里不可以用mat2gray
//	
//	vector<Rect> boundingboxOD;
//	getConnectedDomain(ODsegment255, boundingboxOD); //boundingbox1有问题，size很大。。
//	int Max_ODboudingbox_Iter = 0, MaxAreaOD = 0;
//	if (boundingboxOD.size() != 0)
//	{
//		for (int i = 0; i < boundingboxOD.size(); i++)
//		{
//			if (boundingboxOD[i].area()>MaxAreaOD)
//			{
//				Max_ODboudingbox_Iter = i;
//				MaxAreaOD = boundingboxOD[i].area();
//			}
//		}
//	}
//
//
//	Rect ODboundingBox= ellipsemergeOD.boundingRect();
//	int OD_diameter = boundingboxOD[Max_ODboudingbox_Iter].width;
//	
//	//float para_x = 3.1;//控制左右的倍率   1.1jpg 2.8 270 400 300
//
//	//float para_x = 2.7;//控制左右的倍率   
//	//float para_y = 250;//控制上下的倍率
//	//int fovea_width = 600, fovea_height = 400;
//
//	float para_x = 2.9;//控制左右的倍率   
//	float para_y = 250;//控制上下的倍率
//	int fovea_width = 400, fovea_height = 400;
//	float para_level = 0.7;
//	if (ODcenterX >= 0.5*Nphong_G_srcImage.size().width)
//	{
//		ODboundingBox.x = ODcenterX - para_x*OD_diameter-0.5*fovea_width;
//	}
//	else
//	{
//		ODboundingBox.x = ODcenterX + para_x*OD_diameter - 0.5*fovea_width;
//	}
//	ODboundingBox.y = ODboundingBox.y + para_y - 0.5*fovea_height;
//	ODboundingBox.width = fovea_width;
//	ODboundingBox.height = fovea_height;
//	Mat fovea_ROI = decorrelationGImg(ODboundingBox).clone();
//	namedWindow("fovea_ROI", 0);
//	imshow("fovea_ROI", fovea_ROI);
//
//	double maxFoveaValue;
//	minMaxLoc(fovea_ROI, NULL, &maxFoveaValue, NULL, NULL);
//	float fovea_threshold = para_level*maxFoveaValue;
//	threshold(fovea_ROI, fovea_ROI, fovea_threshold, 255, CV_THRESH_BINARY);
//	namedWindow("二值化后的黄斑", 0);
//	imshow("二值化后的黄斑", fovea_ROI);
//	Mat FoveaErode;
//	Mat FoveaErodeElement = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
//	erode(fovea_ROI, FoveaErode, FoveaErodeElement, Point(-1, -1), 1);
//	namedWindow("腐蚀后的黄斑ROI", 0);
//	imshow("腐蚀后的黄斑ROI", FoveaErode);
//
//	// 保留最大面积连通域  
//	vector<vector<Point>> contours;
//	findContours(FoveaErode, contours, RETR_LIST, CHAIN_APPROX_NONE);
//
//	int Max_Fovea_Iter = 0;	
//	double MaxareaFovea = 0;
//	vector<vector<Point>> maxContour;
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		if (contourArea(contours[i]) > MaxareaFovea)
//		{
//			Max_Fovea_Iter = i;
//			MaxareaFovea = contourArea(contours[i]);
//
//		}
//	}
//	Mat fovea_ROIresult(FoveaErode.size(), CV_8U, Scalar(0));
//	maxContour.push_back(contours[Max_Fovea_Iter]);
//	drawContours(fovea_ROIresult, maxContour, -1, Scalar(255), -1); //-1，接下来就不需要填充了。
//	namedWindow("黄斑最大连通域", 0);
//	imshow("黄斑最大连通域", fovea_ROIresult);
//	Mat fovea_ROIresult1;
//	normalize(fovea_ROIresult, fovea_ROIresult1, 0, 1, NORM_MINMAX, fovea_ROIresult.type());
//	Point centerFovea = GetCenterPoint(fovea_ROIresult1);
//	Mat fovea_ROIresultshow; cvtColor(fovea_ROIresult, fovea_ROIresultshow, CV_GRAY2BGR);
//	circle(fovea_ROIresultshow, centerFovea, 5, Scalar(0, 0, 255), -1);
//	namedWindow("fovea_ROIresultshow", 0);
//	imshow("fovea_ROIresultshow", fovea_ROIresultshow);
//	centerFovea.x += ODboundingBox.x;
//	centerFovea.y += ODboundingBox.y;
//	circle(srcImage_ODellipseCovered, centerFovea, 100, Scalar(0, 0, 255),5);
//	namedWindow("视盘和黄斑检测", 0);
//	imshow("视盘和黄斑检测", srcImage_ODellipseCovered);
//
//
//
//	waitKey(0);
//	return 0;
//}