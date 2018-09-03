#ifndef Mat2Qimage_H
#define Mat2Qimage_H

#include <opencv2/core/core.hpp> // Mat�Ȼ����ṹ
#include <opencv2/imgproc/imgproc.hpp> // ɫ�ʿռ�ת��
#include <QString>
#include <QStringList>
#include <QList>
#include <QScreen>
#include <QGuiApplication>
#include <QImage>

using namespace cv;
Mat QImage2Mat(QImage& image);
QImage* Mat2QImage(Mat &mat);
QImage* Mat2QImageRgb(const cv::Mat& mat);

#endif
