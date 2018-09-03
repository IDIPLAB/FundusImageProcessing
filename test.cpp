//#include<iostream>
//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//#include<cmath>
//#include<stack>
//
//using namespace cv;
//using namespace std;
//
//void stdfilt(const Mat &src, Mat &dst) //大小一样，dst为float型。
//{
//	Mat mask = Mat::ones(7, 7, CV_32FC1);
//	float summask = 7 * 7;
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
//	filter2D(powimg, c1, -1, mask / n1summask, Point(-1, -1), BORDER_REFLECT);
//	filter2D(src, c2, -1, mask, Point(-1, -1), BORDER_REFLECT);
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
//int main()
//{
//	Mat ii = imread("testo.jpg");
//	Mat grayii;
//	cvtColor(ii, grayii, CV_RGB2GRAY);
//	grayii.convertTo(grayii, CV_32FC1);
//
//	double maxFoveaValue1;
//	minMaxLoc(grayii, NULL, &maxFoveaValue1, NULL, NULL);  //最大值和matlab差255
//
//	Mat gstdrayii = Mat(grayii.size(), grayii.type());
//	stdfilt(grayii, gstdrayii);
//
//	double maxFoveaValue;
//	minMaxLoc(gstdrayii, NULL, &maxFoveaValue, NULL, NULL);  //最大值和matlab查0.2
//
//	gstdrayii.convertTo(gstdrayii, CV_8UC1);
//
//
//
//
//
//	namedWindow("局部方差", 0);
//	imshow("局部方差", gstdrayii);
//	waitKey(0);
//	return 0;
//}