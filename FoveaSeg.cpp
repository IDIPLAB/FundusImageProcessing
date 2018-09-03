/********************************�� �� �� ��************************************
*	�ļ�����FoveaSeg
*
*	FoveaSegment()						- ���ڲ����㺯��
*
********************************************************************************/
#include "FoveaSeg.h"
FoveaSeg::FoveaSeg(){
	numsOfParam = 0;

	ParamValue para_x;
	para_x.name = QStringLiteral("horizontalRatio");
	para_x.value.varType = T_DOUBLE;
	para_x.value.dbVal = 2.9;
	para_x.explain = QStringLiteral("����ˮƽ�ƶ�����\nȡֵ��Χ: >= 0 ; \n�Ƽ�2.9");
	this->ListOfParam.push_back(para_x);


}

FoveaSeg::~FoveaSeg(){

}


/*******************************************************************************
*	�������ƣ�FoveaSegment();
*
*	�������ܣ����ڲ����㺯�����ڲ�����ͼ�����ã��������������ͼ�������С�
*			  ���лư߷ָ
*
*	���������Mat &OD_BwImg  �������̶�ֵ��ͼ
*			  RotatedRect &ellipseParam   ������Բ����
*			  int thresholdValue  ��ֵ����ֵ
*
*	�� �� ֵ��void
********************************************************************************/
void FoveaSeg::myphong(const Mat src, Mat&dst, int param[4])
{
	int centerx = param[0];
	int centery = param[1];
	int width = param[2];
	int height = param[3];
	int halfWidth = floor(width / 2);
	int halfHeight = floor(height / 2);
	//����������;��淴��ģ��
	float I1 = 8, z = 67.5, Fn = 1;
	float Kd = 0.08; //������ϵ��
	float Ks = 2.8;  //���淴��ϵ��
	Mat cosxy = Mat::zeros(Size(width, height), CV_32FC1);
	Mat cosfy = Mat::zeros(Size(width, height), CV_32FC1);
	float temp = 0;
	int radiusCircle = 0;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (pow(j - halfHeight, 2) + pow(i - halfWidth, 2) <= pow(radiusCircle, 2))	//�ڲ�СԲ����
			{
				//cosxy.at<float>(j, i) = 0;
				//cosfy.at<float>(j, i) = 0;
				temp = z / sqrt(pow(i - halfWidth, 2) + pow((j - halfHeight), 2) + pow(z, 2));
				cosxy.at<float>(j, i) = temp;
				cosfy.at<float>(j, i) = pow(2 * pow(temp, 2) - 1, Fn);
			}
			else
			{
				temp = z / sqrt(pow(i - halfWidth, 2) + pow((j - halfHeight), 2) + pow(z, 2));
				cosxy.at<float>(j, i) = temp;
				cosfy.at<float>(j, i) = pow(2 * pow(temp, 2) - 1, Fn);
				if (2 * pow(temp, 2) - 1 < 0)
				{
					cosfy.at<float>(j, i) = 0;
				}
			}
		}
	}
	Mat DUFmask = I1*Kd*cosxy;  //
	Mat DIFmask = I1*Ks*cosfy;



	Mat Ibground = DUFmask + DIFmask;
	Mat floatsrcImg;
	dst.convertTo(dst, CV_32FC1);

	//ROI����
	floatsrcImg = dst(Range(centery - halfHeight, centery + halfHeight + 1), Range(centerx - halfWidth, centerx + halfWidth + 1));//Range�Ҳ�����
	double a = DUFmask.at<float>(71, 71);
	double b = DIFmask.at<float>(71, 71);
	floatsrcImg -= Ibground;

	dst.convertTo(dst, CV_8UC1);


}
void FoveaSeg::getConnectedDomain(Mat& src,vector<Rect>& boundingbox)//boundingboxΪ���ս������Ÿ�����ͨ��İ�Χ��  
{
	int img_row = src.rows;
	int img_col = src.cols;
	Mat flag = Mat::zeros(Size(img_col, img_row), CV_8UC1);//��־����Ϊ0��ǰ���ص�δ���ʹ�  
	for (int i = 0; i < img_row; i++)
	{
		for (int j = 0; j < img_col; j++)
		{
			if (src.ptr<uchar>(i)[j] == 255 && flag.ptr<uchar>(i)[j] == 0)
			{
				stack<Point2f> cd;
				cd.push(Point2f(j, i));
				flag.ptr<uchar>(i)[j] = 1;
				int minRow = i, minCol = j;//��Χ�����ϱ߽�  
				int maxRow = i, maxCol = j;//��Χ���ҡ��±߽�  
				while (!cd.empty())
				{
					Point2f tmp = cd.top();
					if (minRow > tmp.y)//���°�Χ��  
						minRow = tmp.y;
					if (minCol > tmp.x)
						minCol = tmp.x;
					if (maxRow < tmp.y)
						maxRow = tmp.y;
					if (maxCol < tmp.x)
						maxCol = tmp.x;
					cd.pop();
					Point2f p[4];//�������ص㣬�����õ�������  
					p[0] = Point2f(tmp.x - 1 > 0 ? tmp.x - 1 : 0, tmp.y);
					p[1] = Point2f(tmp.x + 1 < img_col - 1 ? tmp.x + 1 : img_row - 1, tmp.y);
					p[2] = Point2f(tmp.x, tmp.y - 1 > 0 ? tmp.y - 1 : 0);
					p[3] = Point2f(tmp.x, tmp.y + 1 < img_row - 1 ? tmp.y + 1 : img_row - 1);
					for (int m = 0; m < 4; m++)
					{
						int x = p[m].y;
						int y = p[m].x;
						if (src.ptr<uchar>(x)[y] == 255 && flag.ptr<uchar>(x)[y] == 0)//���δ���ʣ�����ջ������Ƿ��ʹ��õ�  
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

Point FoveaSeg::GetCenterPoint(const Mat &src) //�����ֵͼ
{
	int i, j;
	double x0 = 0, y0 = 0, sum = 0;
	Point center;
	double pixel;
	for (i = 0; i < src.size().width; i++)
	{
		for (j = 0; j < src.size().height; j++)
		{
			pixel = src.at<uchar>(j, i);
			x0 = x0 + i*pixel;
			y0 = y0 + j*pixel;
			sum = sum + pixel;
		}
	}
	center.x = x0 / sum;
	center.y = y0 / sum;
	return center;
}
void FoveaSeg::FoveaSegment(Mat &srcImage, Mat& ODsegment255, RotatedRect &ellipsemergeOD, int ODcenterX, double para_x)
{
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
				hsvNormImg.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(imageHueChannel.at<uchar>(j, i));   //Hͨ����Ϊ180��
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

	Mat imageGChannel_Fovea;
	imageGChannel_Fovea = RGBchannels.at(1);
	//���������⴦��
	int ReflectWidth = 151;
	int ReflectHeight = 151;
	int ReflectParams[4] = { 942, 777, ReflectWidth, ReflectHeight };
	Mat phong_G_srcImage = imageGChannel_Fovea.clone();
	
	myphong(imageGChannel_Fovea, phong_G_srcImage, ReflectParams);

	//����Ӧ�Աȶ���ǿ�Լ�ȥ���
	Mat CLAHE_GsrcImage;
	Mat Nphong_G_srcImage = ~phong_G_srcImage;
	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(5);  //************************�������matlab��֪����ɶ
	clahe->apply(Nphong_G_srcImage, CLAHE_GsrcImage);
	Mat decorrelationGImg = CLAHE_GsrcImage - ~phong_G_srcImage;
	normalize(decorrelationGImg, decorrelationGImg, 0, 255, NORM_MINMAX, decorrelationGImg.type()); //���ﲻ������mat2gray

	vector<Rect> boundingboxOD;
	getConnectedDomain(ODsegment255, boundingboxOD); //boundingbox1�����⣬size�ܴ󡣡�
	int Max_ODboudingbox_Iter = 0, MaxAreaOD = 0;
	if (boundingboxOD.size() != 0)
	{
		for (int i = 0; i < boundingboxOD.size(); i++)
		{
			if (boundingboxOD[i].area()>MaxAreaOD)
			{
				Max_ODboudingbox_Iter = i;
				MaxAreaOD = boundingboxOD[i].area();
			}
		}
	}


	Rect ODboundingBox = ellipsemergeOD.boundingRect();
	int OD_diameter = boundingboxOD[Max_ODboudingbox_Iter].width;

	//float para_x = 3.1;//�������ҵı���   1.1jpg 2.8 270 400 300

	//float para_x = 2.7;//�������ҵı���   
	//float para_y = 250;//�������µı���
	//int fovea_width = 600, fovea_height = 400;

	//float para_x = 2.9;//�������ҵı���   
	float para_y = 250;//�������µı���
	int fovea_width = 400, fovea_height = 400;
	float para_level = 0.7;
	if (ODcenterX >= 0.5*Nphong_G_srcImage.size().width)
	{
		ODboundingBox.x = ODcenterX - para_x*OD_diameter - 0.5*fovea_width;
	}
	else
	{
		ODboundingBox.x = ODcenterX + para_x*OD_diameter - 0.5*fovea_width;
	}
	ODboundingBox.y = ODboundingBox.y + para_y - 0.5*fovea_height;
	ODboundingBox.width = fovea_width;
	ODboundingBox.height = fovea_height;
	Mat fovea_ROI = decorrelationGImg(ODboundingBox).clone();

	//namedWindow("fovea_ROI", 0);
	//imshow("fovea_ROI", fovea_ROI);

	double maxFoveaValue;
	minMaxLoc(fovea_ROI, NULL, &maxFoveaValue, NULL, NULL);
	float fovea_threshold = para_level*maxFoveaValue;
	threshold(fovea_ROI, fovea_ROI, fovea_threshold, 255, CV_THRESH_BINARY);

	//namedWindow("��ֵ����Ļư�", 0);
	//imshow("��ֵ����Ļư�", fovea_ROI);

	Mat FoveaErode;
	Mat FoveaErodeElement = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	erode(fovea_ROI, FoveaErode, FoveaErodeElement, Point(-1, -1), 1);

	//namedWindow("��ʴ��Ļư�ROI", 0);
	//imshow("��ʴ��Ļư�ROI", FoveaErode);

	// ������������ͨ��  
	vector<vector<Point>> contours;
	findContours(FoveaErode, contours, RETR_LIST, CHAIN_APPROX_NONE);

	int Max_Fovea_Iter = 0;
	double MaxareaFovea = 0;
	vector<vector<Point>> maxContour;
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (contourArea(contours[i]) > MaxareaFovea)
		{
			Max_Fovea_Iter = i;
			MaxareaFovea = contourArea(contours[i]);

		}
	}
	Mat fovea_ROIresult(FoveaErode.size(), CV_8U, Scalar(0));
	maxContour.push_back(contours[Max_Fovea_Iter]);
	drawContours(fovea_ROIresult, maxContour, -1, Scalar(255), -1); //-1���������Ͳ���Ҫ����ˡ�

	//namedWindow("�ư������ͨ��", 0);
	//imshow("�ư������ͨ��", fovea_ROIresult);

	Mat fovea_ROIresult1;
	normalize(fovea_ROIresult, fovea_ROIresult1, 0, 1, NORM_MINMAX, fovea_ROIresult.type());
	Point centerFovea = GetCenterPoint(fovea_ROIresult1);
	Mat fovea_ROIresultshow; cvtColor(fovea_ROIresult, fovea_ROIresultshow, CV_GRAY2BGR);
	circle(fovea_ROIresultshow, centerFovea, 5, Scalar(0, 0, 255), -1);

	//namedWindow("fovea_ROIresultshow", 0);
	//imshow("fovea_ROIresultshow", fovea_ROIresultshow);
	centerFovea.x += ODboundingBox.x;
	centerFovea.y += ODboundingBox.y;
	circle(src_ROIImg, centerFovea, 100, Scalar(0, 0, 255), 5);

	//namedWindow("���̺ͻư߼��", 0);
	//imshow("���̺ͻư߼��", srcImage_ODellipseCovered);
	cvtColor(src_ROIImg, src_ROIImg, CV_BGR2RGB);
	QImage *out_image1 = Mat2QImageRgb(src_ROIImg);
	this->setOutputImage(out_image1, 0);



}

long FoveaSeg::run(){

	QImage *OD_BwImg = transObj.inputImage.at(0);
	QImage *tempimg = transObj.inputImage.at(1);
	RotatedRect myParam = ellipseParam.back();
	int ODcenterX = IntParam.back();
	QImage::Format format = tempimg->format();


	if (format == QImage::Format_Indexed8)
	{

		return ERROR_INVALID_IMGTYPE;
	}
	if (ListOfParam.at(0).value.nVal < 0)
	{
		return ERROR_INVALID_ARG;
	}

	Mat img = QImage2Mat(*tempimg);
	Mat OD_BwImgMat = QImage2Mat(*OD_BwImg);
	int SegmentType = 0;
	if (format == QImage::Format_RGB32 || format == QImage::Format_ARGB32)
	{

		cvtColor(img, img, CV_RGBA2RGB);
	}

	FoveaSegment(img, OD_BwImgMat, myParam, ODcenterX, ListOfParam.at(0).value.dbVal);
	return ALL_OK;
}