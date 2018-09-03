/********************************��  �� �� ��************************************
*	�ļ�����ExudationSeg
*
*	ExudationSegment()						- ���ڲ����㺯��
*
********************************************************************************/
#include "ExudationSeg.h"
ExudationSeg::ExudationSeg(){
	numsOfParam = 0;

	ParamValue stdparam;
	stdparam.name = QStringLiteral("stdparam");
	stdparam.value.varType = T_INT;
	stdparam.value.nVal = 5;
	stdparam.explain = QStringLiteral("�ֲ�������ο��С\nȡֵ��Χ: >= 0 ; \n�Ƽ�5��7");
	this->ListOfParam.push_back(stdparam);


}

ExudationSeg::~ExudationSeg(){

}


/*******************************************************************************
*	�������ƣ�ExudationSegment();
*
*	�������ܣ����ڲ����㺯�����ڲ�����ͼ�����ã��������������ͼ�������С�
*			  ���������ָ
*
*	���������Mat &OD_BwImg  �������̶�ֵ��ͼ
*			  RotatedRect &ellipseParam   ������Բ����
*			  int thresholdValue  ��ֵ����ֵ
*
*	�� �� ֵ��void
********************************************************************************/
Mat ExudationSeg::strelDisk(int Radius)
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
		break;//���뾶Ϊ1ʱ��3X3�� ,���뾶Ϊ2ʱ��5X5��  
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
void ExudationSeg::fillHole(const Mat srcBw, Mat &dstBw)
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//��չͼ��
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//�ü���չ��ͼ��
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}
void ExudationSeg::edgeDetect(const Mat &srcImg, Mat &edgeImg, Mat & N_src_GROIImg, int temp[])
{
	vector<int> xLocation, yLocation;
	for (int i = 0; i < srcImg.size().width; i++)
	{
		for (int j = 0; j < srcImg.size().height; j++)
		{
			if (srcImg.at<uchar>(j, i)>10)
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
	Mat src_GCloneImg = srcImg.clone();
	Mat src_GROIImg = src_GCloneImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1));
	N_src_GROIImg = ~src_GROIImg;

	// ��ȡ��Ե
	Mat N_src_GROIImg_edge;
	blur(N_src_GROIImg, N_src_GROIImg_edge, Size(3, 3));
	Canny(N_src_GROIImg_edge, N_src_GROIImg_edge, 3, 30);
	Mat fill_img;
	fillHole(N_src_GROIImg_edge, fill_img);
	//namedWindow("Canny��Ե�����", 0);
	//imshow("Canny��Ե�����", fill_img);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	Mat dilate_fill_img, erose_fill_img;
	dilate(fill_img, dilate_fill_img, element);
	erode(fill_img, erose_fill_img, element);
	erode(erose_fill_img, erose_fill_img, element);
	erode(erose_fill_img, erose_fill_img, element);
	Mat edge_ROIimg = dilate_fill_img - erose_fill_img; //��ɫ��������Ϊ255����1���ں���ֱ�ӵ�˻������⣡
	//namedWindow("������ȡ", 0);
	//imshow("������ȡ", edge_ROIimg);
	threshold(erose_fill_img, edgeImg, 100, 1, THRESH_BINARY); //��������ֵ����Ϊ1��������255��

	temp[0] = x_nozeromin; temp[1] = x_nozeromax; temp[2] = y_nozeromin; temp[3] = y_nozeromax;
}

void ExudationSeg::morphReconstruct(Mat &marker, Mat &mask, Mat&dst)
{

	min(marker, mask, dst);
	dilate(dst, dst, Mat());
	min(dst, mask, dst);
	Mat temp1 = Mat(marker.size(), CV_8UC1);
	Mat temp2 = Mat(marker.size(), CV_8UC1);
	do
	{
		dst.copyTo(temp1);
		dilate(dst, dst, Mat());
		cv::min(dst, mask, dst);
		compare(temp1, dst, temp2, CV_CMP_NE);
	} while (sum(temp2).val[0] != 0);

}
void ExudationSeg::stdfilt(const Mat &src, Mat &dst, int size) //��Сһ����dstΪfloat�͡�
{
	Mat mask = Mat::ones(size, size, CV_32FC1);
	float summask = pow(size, 2);
	float n1summask = summask - 1.0;

	Mat powimg = Mat(src.size(), src.type());
	Mat squreimg = Mat(src.size(), src.type());
	for (int i = 0; i < src.size().width; i++)
	{
		for (int j = 0; j < src.size().height; j++)
		{
			powimg.at<float>(j, i) = pow(src.at<float>(j, i), 2);
			//squreimg.at<float>(j, i) = sqrt(src.at<float>(j, i));
		}
	}

	Mat c1, c2;

	filter2D(powimg, c1, -1, mask / n1summask, Point(-1, -1), BORDER_CONSTANT);
	filter2D(src, c2, -1, mask, Point(-1, -1), BORDER_CONSTANT);


	float maxtemp;
	for (int i = 0; i < c2.size().width; i++)
	{
		for (int j = 0; j < c2.size().height; j++)
		{
			c2.at<float>(j, i) = pow(c2.at<float>(j, i), 2) / (summask*n1summask);
			maxtemp = max<float>(c1.at<float>(j, i) - c2.at<float>(j, i), 0);
			dst.at<float>(j, i) = sqrt(maxtemp);
		}
	}

}

void ExudationSeg::ExudationSegment(Mat &srcImage, Mat& ODsegment255, int stdparam)
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
	Mat hsvNormImg1 = hsvImg.clone();
	//cout << V_Mean;
	if (V_Mean < 89)
	{
		for (int i = 0; i < hsvNormImg1.size().width; i++)
		{
			for (int j = 0; j < hsvNormImg1.size().height; j++)
			{
				hsvNormImg1.at<Vec3b>(j, i)[2] = saturate_cast<uchar>(1.3*imgValueChannel.at<uchar>(j, i));
				hsvNormImg1.at<Vec3b>(j, i)[1] = saturate_cast<uchar>(1.2*imageSaturationChannel.at<uchar>(j, i));
				hsvNormImg1.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(imageHueChannel.at<uchar>(j, i));   //Hͨ����Ϊ180��
			}
		}
	}
	Mat srcNormImg1;
	cvtColor(hsvNormImg1, srcNormImg1, CV_HSV2RGB);
	//namedWindow("srcNormImg", 0);
	//imshow("srcNormImg", srcNormImg);

	//Mat srcGrayImg;
	//cvtColor(srcNormImg, srcGrayImg, CV_RGB2GRAY);
	vector<Mat> RGBchannels1;
	split(srcNormImg1, RGBchannels1);
	Mat src_GImag = RGBchannels1.at(1);

	//Ѱ����ɫͨ��ͼ���ϵ����±߽磬opencvû��matlab��find��������QAQ
	//MatIterator_<uchar> iterBegin = src_GImag.begin<uchar>();
	//MatIterator_<uchar> iterEnd = src_GImag.end<uchar>();
	Mat edgeImg, N_src_GROIImg;
	int minMaxPara[4];
	edgeDetect(src_GImag, edgeImg, N_src_GROIImg, minMaxPara);
	int x_nozeromin = minMaxPara[0];
	int x_nozeromax = minMaxPara[1];
	int y_nozeromin = minMaxPara[2];
	int y_nozeromax = minMaxPara[3];





	Mat hsvNormImg = hsvImg.clone();
	if (V_Mean < 89)
	{
		for (int i = 0; i < hsvNormImg.size().width; i++)
		{
			for (int j = 0; j < hsvNormImg.size().height; j++)
			{
				hsvNormImg.at<Vec3b>(j, i)[2] = 1.5*imgValueChannel.at<uchar>(j, i);
				hsvNormImg.at<Vec3b>(j, i)[1] = saturate_cast<uchar>(imageSaturationChannel.at<uchar>(j, i));
				hsvNormImg.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(imageHueChannel.at<uchar>(j, i));
			}
		}
	}
	Mat srcNormImg;
	cvtColor(hsvNormImg, srcNormImg, CV_HSV2RGB);

	Mat graySrcNormImg;
	cvtColor(srcNormImg, graySrcNormImg, CV_RGB2GRAY);
	vector<Mat> srcNormChannels;
	split(srcNormImg, srcNormChannels);
	Mat imageNormChannel = srcNormChannels.at(1);
	Mat CLAHE_GsrcImage;
	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(3);
	clahe->apply(imageNormChannel, CLAHE_GsrcImage);
	//normalize(CLAHE_GsrcImage, CLAHE_GsrcImage, 0, 255, NORM_MINMAX, CLAHE_GsrcImage.type());
	//namedWindow("CLAHE�Աȶ���ǿ", 0);
	//imshow("CLAHE�Աȶ���ǿ", CLAHE_GsrcImage);
	Mat prepsrcImg;
	medianBlur(CLAHE_GsrcImage, prepsrcImg, 15); //�˲�������������



	Mat srcGNormImg = prepsrcImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1)).clone();


	//namedWindow("�Աȶ���ǿROI", 0);
	//imshow("�Աȶ���ǿROI", srcGNormImg);

	//��ֵ�˲�����̬�ؽ�
	Mat medSrcNormImg;
	medianBlur(srcGNormImg, medSrcNormImg, 49); //�˲�������������
	Mat srcGNormReconstruct;
	morphReconstruct(medSrcNormImg, srcGNormImg, srcGNormReconstruct);//�ٶȾ����ޱ�
	//namedWindow("��̬ѧ�ؽ�", 0);
	//imshow("��̬ѧ�ؽ�", srcGNormReconstruct);
	Mat rebuiltImg = srcGNormImg - srcGNormReconstruct;
	//namedWindow("�ؽ���ͼ��", 0);
	//imshow("�ؽ���ͼ��", rebuiltImg);
	Mat bwReconstrucImg;
	threshold(rebuiltImg, rebuiltImg, 21, 255, CV_THRESH_BINARY);


	//�ղ���ȥ��Ѫ��

	Mat closeElement = strelDisk(30); //�ṹԪһ��
	//Mat closeElement = getStructuringElement(MORPH_ELLIPSE, Size(99, 99)); //matlab��opencv�ĽṹԪ�ǲ�һ����
	Mat graySrcNormImgRoi = graySrcNormImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1)).clone();
	Mat grayopecnImg;
	morphologyEx(graySrcNormImg, graySrcNormImg, MORPH_CLOSE, closeElement, Point(-1, -1), 2, BORDER_REFLECT); //�����һ��...�ٶȺ�����
	grayopecnImg = graySrcNormImg(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1)).clone();


	//����ֲ�������ȡ��������

	Mat Float_graySrcNormImgRoi;
	grayopecnImg.convertTo(Float_graySrcNormImgRoi, CV_32FC1);
	normalize(Float_graySrcNormImgRoi, Float_graySrcNormImgRoi, 0, 1, NORM_MINMAX, CV_32FC1); //����Χ��С��0-1

	Mat stdflitImg = Mat(Float_graySrcNormImgRoi.size(), Float_graySrcNormImgRoi.type());
	stdfilt(Float_graySrcNormImgRoi, stdflitImg, stdparam);
	Mat edgeImgFloat;
	edgeImg.convertTo(edgeImgFloat, CV_32FC1);
	Mat stdshow;
	stdflitImg = stdflitImg.mul(edgeImgFloat);



	//mat2gray(stdflitImg, stdflitImg);
	//namedWindow("�ֲ�����ͼ", 0);
	//imshow("�ֲ�����ͼ", stdflitImg);


	//�ֲ���׼����ͼ���������⣬ֵ̫С�ˡ���ʹȫ���ֳ���Ҳ��������
	Mat openElement2 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	Mat openElement3 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat openElement4 = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
	Mat stdflitImg_morph, bwstdImg_morph1;
	morphologyEx(stdflitImg, stdflitImg_morph, MORPH_OPEN, openElement2, Point(-1, -1), 1);
	threshold(stdflitImg_morph, bwstdImg_morph1, 0.006, 255, CV_THRESH_BINARY);
	
	//namedWindow("test", 0);
	//imshow("test", bwstdImg_morph1);

	morphologyEx(bwstdImg_morph1, bwstdImg_morph1, MORPH_DILATE, openElement3, Point(-1, -1), 1);
	morphologyEx(bwstdImg_morph1, bwstdImg_morph1, MORPH_CLOSE, openElement4, Point(-1, -1), 1);
	bwstdImg_morph1.convertTo(bwstdImg_morph1, CV_8UC1);
	stdflitImg_morph = bwstdImg_morph1.mul(edgeImg); //stdflitImg_morph��32λ
	Mat morph_Fill_Img;
	fillHole(stdflitImg_morph, morph_Fill_Img);
	
	//namedWindow("morph_Fill_Img", 0);
	//imshow("morph_Fill_Img", morph_Fill_Img);

	Mat exudateROI = morph_Fill_Img + rebuiltImg;
	//exudateROIҪ���0-1
	Mat exudateROI01;
	Mat N_exudateROI01 = ~exudateROI;
	threshold(N_exudateROI01, exudateROI01, 100, 1, CV_THRESH_BINARY);
	Mat black_exudateROI = srcGNormImg.mul(exudateROI01);

	//namedWindow("black_exudateROI", 0);
	//imshow("black_exudateROI", black_exudateROI);

	//�ڶ�����̬ѧ�ؽ�
	Mat ExudateReconstruct;
	morphReconstruct(black_exudateROI, srcGNormImg, ExudateReconstruct);
	Mat FinalExuadate = srcGNormImg - ExudateReconstruct;
	//namedWindow("�ؽ���ͼ��", 0);
	//imshow("�ؽ���ͼ��", rebuiltImg);
	Mat bwExudate;
	threshold(FinalExuadate, bwExudate, 0, 255, CV_THRESH_BINARY);
	
	//
	//namedWindow("�����ؽ�ͼ��", 0);
	//imshow("�����ؽ�ͼ��", bwExudate);
	Mat ODsegment_N1 = ~ODsegment255;
	threshold(ODsegment_N1, ODsegment_N1, 254, 1, CV_THRESH_BINARY);
	Mat bwExudate_ROI;
	bwExudate_ROI = bwExudate.mul(ODsegment_N1);


	Mat RGBbwExuadate = Mat::zeros(bwExudate_ROI.size(), CV_8UC1);
	threshold(bwExudate_ROI, RGBbwExuadate, 254, 1, CV_THRESH_BINARY);

	vector<Mat> ExudatetChannels;
	for (int i = 0; i<3; i++)
	{
		ExudatetChannels.push_back(RGBbwExuadate);
	}

	Mat Exuadate3C = Mat::zeros(RGBbwExuadate.rows, RGBbwExuadate.cols, CV_8UC3);
	merge(ExudatetChannels, Exuadate3C);
	Mat srcShowImg = srcImage(Range(y_nozeromin, y_nozeromax + 1), Range(x_nozeromin, x_nozeromax + 1)).clone();
	Mat src_Exudate = Exuadate3C.mul(srcShowImg);
	cvtColor(src_Exudate, src_Exudate, CV_BGR2RGB);


	//cvtColor(src_ROIImg, src_ROIImg, CV_BGR2RGB);
	/*QImage *out_image1 = Mat2QImage(bwExudate_ROI);
	this->setOutputImage(out_image1, 0);*/

	QImage *out_image2 = Mat2QImageRgb(src_Exudate);
	this->setOutputImage(out_image2, 0);


}

long ExudationSeg::run(){

	QImage *OD_BwImg = transObj.inputImage.at(0);
	QImage *tempimg = transObj.inputImage.at(1);


	QImage::Format format = tempimg->format();


	if (format == QImage::Format_Indexed8)
	{

		return ERROR_INVALID_IMGTYPE;
	}
	if (ListOfParam.at(0).value.nVal < 0 )
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

	ExudationSegment(img, OD_BwImgMat, ListOfParam.at(0).value.nVal);
	return ALL_OK;
}