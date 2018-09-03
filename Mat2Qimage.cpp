#include "Mat2Qimage.h"
using namespace cv;

QImage* Mat2QImage(Mat &mat) //灰度Mat转QImage
{
	if (mat.channels() == 3){
		cvtColor(mat, mat, CV_BGR2GRAY);
	}
	int width;
	if (mat.cols % 4 == 0) 
		width = mat.cols;
	else width = mat.cols /4 * 4 + 4;//4字节对齐
	uchar *outImg = new uchar[mat.rows * width]();
	for (int i = 0; i < mat.cols; i++){
		for (int j = 0; j < mat.rows; j++){
			outImg[i + j * width] = mat.at<uchar>(j, i);
		}
	}
	QImage *img = new QImage(outImg, width, mat.rows, QImage::Format_Indexed8);
	return img;
}

Mat QImage2Mat(QImage& image) 
{
	Mat mat;
	//qDebug() << image.format();
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	default:
		mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}
QImage* Mat2QImageRgb(const cv::Mat& mat)
{
	// Copy input Mat  
	const uchar *pSrc = (const uchar*)mat.data;
	// Create QImage with same dimensions as input Mat
	int width;
	if (mat.cols % 4 == 0)
		width = mat.cols;
	else width = mat.cols / 4 * 4 + 4;//4字节对齐
	uchar *outImg = new uchar[mat.rows * width * 3]();
	for (int i = 0; i < mat.cols; i++){
		for (int j = 0; j < mat.rows; j++){
			outImg[3*i + j * width * 3] = mat.at<Vec3b>(j, i)[0];
			outImg[3*i + j * width * 3 + 1] = mat.at<Vec3b>(j, i)[1];
			outImg[3*i + j * width * 3 + 2] = mat.at<Vec3b>(j, i)[2];
		}
	}

	QImage *img = new QImage(outImg, width, mat.rows, QImage::Format_RGB888); 
	return img;
}
