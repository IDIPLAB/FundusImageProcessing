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
	ALL_OK = 0,//�޴���
	ERROR_INVALID_ARG,//�����������
	ERROR_INVALID_IMGTYPE //����ͼ�����ʹ���
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

//������ֵ�ԣ�
struct ParamValue
{
	QString name;	
	VT_VARIENT value;
	QString explain;
};

//��������������ͼ�����У�
struct TransObj{
	vector<QImage*> inputImage;
	vector<QImage*> outputImage;
};

class Base
{
public:
	vector<ParamValue> ListOfParam; //�����б�
	TransObj transObj; 
	int numsOfParam;
	vector<RotatedRect> ellipseParam;
	vector<int> IntParam;
public:
	Base();
	~Base();
	
public:
	virtual long run() = 0;
	bool isEmpty();//�жϲ����Ƿ�Ϊ��
	ParamValue getParamValue(int i);
	bool setParamValue(int i,ParamValue pv);
	vector<QImage*> getInputImage();
	vector<QImage*> getOutputImage();
	bool setInputImage(QImage* image);
	bool setOutputImage(QImage* image, bool bClean = true);
	
	int getNums();//��������
	
	void clearOutImage();
};

#endif