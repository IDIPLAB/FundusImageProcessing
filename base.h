#ifndef Base_H
#define Base_H
#define MAX_PATH_LENGTH 512

#include <vector>
#include <list>
#include "Image.h"
#include <QtWidgets/QtWidgets>
#include<qstring.h>
#include "Mat2Qimage.h"
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
using namespace std;
using namespace cv;

enum ERROR_CODE{
	ALL_OK = 0,//无错误
	ERROR_INVALID_ARG,//输入参数有误
	ERROR_INVALID_IMGTYPE //输入图像类型错误
};

enum VarEnum{
	T_EMPTY = 0,
	T_DOUBLE = 1,
	T_INT = 2,
	T_BOOL = 3,
	T_SIZE = 4,
	T_RECT = 5,
	T_COLORREF = 6,
	T_TSTR = 7
};

struct VT_VARIENT{
	VarEnum varType;
	union
	{
		double dbVal;
		int nVal;
		bool boolVal;
		unsigned char* imgVal;
	};
};

//参数名值对；
struct ParamValue
{
	QString name;	
	VT_VARIENT value;
	QString explain;
};

//存放输入与输出的图像序列；
struct TransObj{
	vector<QImage*> inputImage;
	vector<QImage*> outputImage;
};

class Base
{
public:
	vector<ParamValue> ListOfParam; //参数列表；
	TransObj transObj; 
	int numsOfParam;
	vector<RotatedRect> ellipseParam;
	vector<int> IntParam;
public:
	Base();
	~Base();
	
public:
	virtual long run() = 0;
	bool isEmpty();//判断参数是否为空
	ParamValue getParamValue(int i);
	bool setParamValue(int i,ParamValue pv);
	vector<QImage*> getInputImage();
	vector<QImage*> getOutputImage();
	bool setInputImage(QImage* image);
	bool setOutputImage(QImage* image, bool bClean = true);
	
	int getNums();//参数个数
	
	void clearOutImage();
};

#endif