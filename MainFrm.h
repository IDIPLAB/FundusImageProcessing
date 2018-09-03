#ifndef _PROJECT716_MAINFRM_H
#define _PROJECT716_MAINFRM_H

#include <QtWidgets/QtWidgets>
#include <QtGui>
#include <vector>
//#include "FeatureExtract.h"
//#include "SimilarityMeasurement.h"
//#include "QImage1.h"
//#include "Image1Window.h"
#include "base.h"

#include <qlabel.h>
#include "imagescrollarea.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event);



private slots:
	void openOne();
	//void openTwo();
	//void openSeries();
	//void openVideo();
	void save();

	void zoomIn();
	void zoomOut();
	void normalSize();

	//灰度变换槽;
	void lineTransSlot();
	void BloodVeseelSegSlot();
	void OpticDiskSegSlot();
	void FoveaSegSlot();
	void ExudationSlot();
	void powerTransSlot();
	void histEqSlot();

	//about槽;
	void aboutSlot();

	//后续处理slot
	void SaveImgSlot();
    void ClearImgListSlot();

private:
	void createActions();
	//void createMenus();
	//void createToolBars();

	void initialMenu();
	void setMenu(int picturenum);

private:
	//文件相关的"动作";
	//QAction *openTwoAction;
	QAction *openOneAction;
	//QAction *openSeriesAction;
	//QAction *openVideoAction;
	QAction *saveAction;
	QAction *exitAction;

	//糖尿病图像处理动作；
	QAction *vesselSegmentation;
	QAction *opticDiskSegmentation;
	QAction *foveaLocalization;
	QAction *exudationSegmentation;
	QAction *DRClassification;
	//QAction *histmatchaction;
	
	//白内障图像处理动作
	QAction *pupilExtraction;
	QAction *CAClassfication;
	

	QAction *aboutaction;
	QMenuBar *menubar;

	QMenu *fileMenu;
	QMenu *graytranformmenu;
	QMenu *cataractmenu;

	QMenu *nextprocess;
	QAction *saveImg;
	QAction *clearImgList;
	//QMenu *tranformmenu;
	//QMenu *featuremenu;
	//QMenu *jointmenu;

	QMenu *helpmenu;

	QToolBar *openToolbar;
	QToolBar *saveToolbar;
	//Image1Window *image1window;


	// view actions
	QMenu* viewMenu;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;


	QLabel *imageLabel;
	
	ImgScrollArea *scrollArea;
	double scaleFactor;

	void adjustScrollBar(QScrollBar *scrollBar, double factor);
	void scaleImage(double scale);
	void updateViewActions();
public:
	
	void LoadFile(QString filename);
	void LoadFromImage(QImage* pImg, const QString& title);
	
private:
	QImage m_img;
	static list<QImage*> ImgList;
	static vector<RotatedRect> ellipsemergeOD_MainFrm;
	static vector<int> intParams_MainFrm;
	QVector<QImage *> vecimageseries;

	QImage* imagerect;
	

	//DisplayPictureWindow * dp;

private:
	//void sleep(unsigned int msec);
public:
	/*QImage* rgb2gray(QImage * image);*/
	//QImage* gray2rgb(QImage * image);

	/* 2017/04/08 */
	void display(unsigned char* data, int width, int height);
	QDialog::DialogCode initParamWindow(Base* base);

	void display(QImage* img, const QString& title);
	bool RunWithParam(Base* b);
};







#endif // this head file
