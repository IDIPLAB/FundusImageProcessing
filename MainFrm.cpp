#include "MainFrm.h"
#include <qmessagebox.h>
#include <vector>

#include "HistEquation.h"
#include "LogTransform.h"
#include "LinearTransform.h"
#include "PowerTransform.h"
#include "paramwindow.h"
#include"BloodVesselSeg.h"
#include"OpticDiskSeg.h"
#include"FoveaSeg.h"
#include"ExudationSeg.h"
#include<opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
list<QImage*> MainWindow::ImgList = {}; 
vector<RotatedRect> MainWindow::ellipsemergeOD_MainFrm = {};
vector<int> MainWindow::intParams_MainFrm;
MainWindow::MainWindow()
: imageLabel(new QLabel)
, scrollArea(new ImgScrollArea)
, scaleFactor(1.)
{
	
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);
	
	//设置窗口的背景图片
	setObjectName("w1");
	//setStyleSheet("#w1{border-image:url(images/background.png);}");
	setStyleSheet("#w1{border-image:url(images/m3.jpg);}");
	imageLabel->adjustSize();
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	scrollArea->setVisible(false);
	setCentralWidget(scrollArea);
	
	createActions();
	initialMenu();
	
	//feature = new FeatureExtract;
	//similaritymeasurement = new SimilarityMeasurement();
	
	//this->setMinimumWidth(1200);
	//this->setMinimumHeight(700);
	this->resize(QSize(1200, 700));
	setWindowIcon(QIcon(tr("images/robot_64px.png")));//设置窗口图标；
	//setWindowState(Qt::WindowMaximized);//使窗口的状态为最大；
	QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
	move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QMainWindow::closeEvent(event);
}



void MainWindow::createActions()
{
	//创建“文件”菜单中对应的动作；
	openOneAction = new QAction(QStringLiteral("打开图片"), this);
	openOneAction->setIcon(QIcon("images/circle_green_32px.ico"));
	connect(openOneAction, SIGNAL(triggered()), this, SLOT(openOne()));
	/*openTwoAction = new QAction(QStringLiteral("打开两幅图"), this);
	openTwoAction->setIcon(QIcon("images/open_64px.png"));
	connect(openTwoAction, SIGNAL(triggered()), this, SLOT(openTwo()));

	openSeriesAction = new QAction(QStringLiteral("打开图像序列"), this);
	connect(openSeriesAction, SIGNAL(triggered()), this, SLOT(openSeries()));

	openVideoAction = new QAction(QStringLiteral("视频处理模块"), this);
	connect(openVideoAction, SIGNAL(triggered()), this, SLOT(openVideo()));*/

	saveAction = new QAction(QStringLiteral("保存图片"), this);
	saveAction->setIcon(QIcon("images/Save_32px.png"));
	saveAction->setEnabled(false);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	exitAction = new QAction(QStringLiteral("关闭"), this);
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	//创建about动作;
	aboutaction = new QAction(QStringLiteral("关于软件"), this);
	connect(aboutaction, SIGNAL(triggered()), this, SLOT(aboutSlot()));

	//创建糖尿病图像处理动作;
	vesselSegmentation = new QAction(QStringLiteral("血管分割"), this);
	connect(vesselSegmentation, SIGNAL(triggered()), this, SLOT(BloodVeseelSegSlot()));
	opticDiskSegmentation = new QAction(QStringLiteral("视盘分割"), this);
	connect(opticDiskSegmentation, SIGNAL(triggered()), this, SLOT(OpticDiskSegSlot()));
	foveaLocalization = new QAction(QStringLiteral("黄斑定位"), this);
	connect(foveaLocalization, SIGNAL(triggered()), this, SLOT(FoveaSegSlot())); 
	exudationSegmentation = new QAction(QStringLiteral("渗出分割"), this);
	connect(exudationSegmentation, SIGNAL(triggered()), this, SLOT(ExudationSlot()));
	DRClassification = new QAction(QStringLiteral("分级诊断"), this);
	
	//创建白内障处理动作
	pupilExtraction = new QAction(QStringLiteral("瞳孔提取"), this);
	CAClassfication = new QAction(QStringLiteral("分级诊断"), this);

	//创建后续处理动作
	saveImg = new QAction(QStringLiteral("存储图像"), this);
	connect(saveImg, SIGNAL(triggered()), this, SLOT(SaveImgSlot()));
	clearImgList = new QAction(QStringLiteral("清空"), this);
	connect(clearImgList, SIGNAL(triggered()), this, SLOT(ClearImgListSlot()));
}

//初始化菜单
void MainWindow::initialMenu()
{
	//创建文件菜单；
	fileMenu = new QMenu(QStringLiteral("文件"));
	menuBar()->setFont(QFont("Times", 12, QFont::Normal));
	fileMenu->addAction(openOneAction);
	/*fileMenu->addAction(openTwoAction);
	fileMenu->addAction(openSeriesAction);
	fileMenu->addAction(openVideoAction);*/

	fileMenu->addSeparator();
	fileMenu->addAction(saveAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	fileMenu->setFont(QFont("Times", 12, QFont::Normal));

	//about菜单;
	helpmenu = new QMenu(QStringLiteral("帮助"));
	helpmenu->addAction(aboutaction);
	helpmenu->setFont(QFont("Times", 12, QFont::Normal));

	//糖尿病图像处理菜单;
	graytranformmenu = new QMenu(QStringLiteral("糖尿病视网膜图像处理"));
	graytranformmenu->addAction(vesselSegmentation);
	graytranformmenu->addAction(opticDiskSegmentation);
	graytranformmenu->addAction(foveaLocalization);
	graytranformmenu->addAction(exudationSegmentation);
	graytranformmenu->addAction(DRClassification);
	//graytranformmenu->addAction(histmatchaction);
	graytranformmenu->setFont(QFont("Times", 12, QFont::Normal));

	//白内障图像处理菜单
	cataractmenu = new QMenu(QStringLiteral("白内障图像处理"));
	cataractmenu->addAction(pupilExtraction);
	cataractmenu->addAction(CAClassfication);
	cataractmenu->setFont(QFont("Times", 12, QFont::Normal));


	//保存当前图像用于后续处理/清空
	nextprocess = new QMenu(QStringLiteral("后续处理"));
	nextprocess->addAction(saveImg);
	nextprocess->addAction(clearImgList);
	nextprocess->setFont(QFont("Times", 12, QFont::Normal));

	//view menu
	viewMenu = menuBar()->addMenu(QStringLiteral("视图"));

	zoomInAct = viewMenu->addAction(QStringLiteral("放大 (+25%)"), this, &MainWindow::zoomIn);
	zoomInAct->setEnabled(false);

	zoomOutAct = viewMenu->addAction(QStringLiteral("缩小 (-25%)"), this, &MainWindow::zoomOut);
	zoomOutAct->setEnabled(false);

	normalSizeAct = viewMenu->addAction(QStringLiteral("原始尺寸"), this, &MainWindow::normalSize);
	normalSizeAct->setEnabled(false);
	viewMenu->setFont(QFont("Times", 12, QFont::Normal));
	setMenu(0);
}

//设置菜单类型
void MainWindow::setMenu(int picturenum)
{
	menuBar()->clear();
	openOneAction->setEnabled(true);
	/*openTwoAction->setEnabled(true);
	openSeriesAction->setEnabled(true);
	openVideoAction->setEnabled(true);*/
	switch (picturenum)
	{
	case 0:
	{
			  menuBar()->addMenu(fileMenu);
			  menuBar()->addSeparator();
			  menuBar()->addMenu(helpmenu);
			  break;
	}

	case 1:
	{
			  //openTwoAction->setDisabled(true);
			  saveAction->setDisabled(false);
			  menuBar()->addMenu(fileMenu);
			  menuBar()->addMenu(viewMenu);
			  menuBar()->addMenu(graytranformmenu);
			  menuBar()->addMenu(cataractmenu);
			  menuBar()->addMenu(nextprocess);
			  //menuBar()->addMenu(tranformmenu);
			  //menuBar()->addMenu(featuremenu);
			  //menuBar()->addMenu(jointmenu);
			  menuBar()->addMenu(helpmenu);
			  break;
	}
	default:
		break;
	}
}

//加载图片,动作处理时
void MainWindow::LoadFromImage(QImage* pImg, const QString& title)
{
	/*if (pImg->format() == QImage::Format_Indexed8)
	{
		QImage* out_image = new QImage(pImg->bits(), pImg->width(), pImg->height(), QImage::Format_Grayscale8);
		m_img = *out_image; 
		delete out_image;
	}*/
	/*else*/
	m_img =*pImg;

	imageLabel->setPixmap(QPixmap::fromImage(m_img));
	scaleFactor = 1.0;

	scrollArea->setVisible(true);

	updateViewActions();

	imageLabel->adjustSize();

	setWindowFilePath(title);

	setMenu(1);
}

void MainWindow::LoadFile(QString filename)
{
	QImage* originimg = new QImage;
	originimg->load(filename);
	QImage::Format format = originimg->format();
	m_img = *originimg;
	
	//if (format == QImage::Format_RGB32)
	//{
	//	m_img = *rgb2gray(originimg);//rgb2gray
	//	

	//}
	//else
	//{
	//	m_img = *originimg;
	//}	

	imageLabel->setPixmap(QPixmap::fromImage(m_img));
	scaleFactor = 1.0;

	scrollArea->setVisible(true);
	
	updateViewActions();

	imageLabel->adjustSize();

	setWindowFilePath(filename);

	setMenu(1);

	delete originimg;
}



void MainWindow::zoomIn()
{
	scaleImage(1.25);
}

void MainWindow::zoomOut()
{
	scaleImage(0.8);
}


void MainWindow::normalSize()
{
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}

void MainWindow::scaleImage(double factor)
{
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void MainWindow::updateViewActions()
{
	zoomInAct->setEnabled(true);
	zoomOutAct->setEnabled(true);
	normalSizeAct->setEnabled(true);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(int(factor * scrollBar->value()
		+ ((factor - 1) * scrollBar->pageStep() / 2)));
}

//打开单幅图像
void MainWindow::openOne()
{
	QString filename = QFileDialog::getOpenFileName(0, "Choose Picture", "", "*.jpg *.png *.bmp", 0);
	if (filename.isNull()) 
	{
		return;
	}
		
	if (!m_img.isNull())
	{
		QString curimg = windowFilePath();
		if (curimg.compare(filename, Qt::CaseInsensitive) == 0) //already opened
			return;

		MainWindow *pNewWnd = new MainWindow();
	    pNewWnd->showMaximized();
		//pNewWnd->resize(1200, 700);
		pNewWnd->LoadFile(filename);

		return;
	}	
	
	LoadFile(filename);	
}

//打开两幅图像
//void MainWindow::openTwo()
//{
//	//读取第一幅图并显示;
//	QString filename1 = QFileDialog::getOpenFileName(0, "Choose Picture", tr(""), "*.jpg *.png *.bmp", 0);
//	if (filename1.isNull()) {
//		return;
//	}
//	
//
//	//读取第二幅图并显示;
//	QString filename2 = QFileDialog::getOpenFileName(0, "Choose Picture", tr(""), "*.jpg *.png *.bmp", 0);
//	if (filename2.isNull()) {
//		return;
//	}
//	
//	MainWindow2 *pMain2 = new MainWindow2();
//	pMain2->showMaximized();
//	pMain2->LoadFromImages(filename1, filename2);
//}
//
//void MainWindow::openSeries()
//{
//	//打开序列图时若想关闭窗口关不了；
//	QStringList filename = QFileDialog::getOpenFileNames(0, "Select one or more files to open", tr("E:/宝宝/其他/桌面杂七杂八/录视频/图像序列/小飞机"), "Images (*.png *.xpm *.jpg *.bmp)", 0);
//	if (filename.length() == 0)
//	{
//		return;
//	}
//	const int size = filename.size();//length()和size是一样的；
//	for (int i = 0; i < size; i++)
//	{
//		QString string = filename.at(i);
//		QImage *img = new QImage(string);
//		if (img->format() == QImage::Format_RGB32)
//		{
//			vecimageseries.push_back(rgb2gray(img));//rgb2gray(originimage2)
//		}
//		else
//		{
//			vecimageseries.push_back(img);
//		}
//		delete img;
//
//	}
//	QImage * image1 = vecimageseries.at(0);
//	int width = image1->width();
//	ImageSeriesWindow *w3 = new ImageSeriesWindow(image1, image1);
//	w3->setParent(this);
//	w3->setFloating(true);
//	w3->setAllowedAreas(Qt::NoDockWidgetArea);
//	addDockWidget(Qt::BottomDockWidgetArea, w3);
//	connect(w3, SIGNAL(output()), this, SLOT(imageSeriesSlot()));
//	setMenu(3);
//}
//


//QImage* MainWindow::rgb2gray(QImage * image)
//{
//	const int width = image->width();
//	const int height = image->height();
//	uchar* imagedata = image->bits();
//
//
//	int W_8 = (width * 8 + 31) / 32 * 4;
//	uchar *graydata = new uchar[height*W_8];//这里不用width而用W_8的原因是这是存储机制造成的，要使其32位对齐，除了32位图像，其他图像都要做这样的处理；
//	int  lineNum_32 = 0;     //行数;
//	for (int i = 0; i<height; i++)
//	{
//		lineNum_32 = i * width * 4;
//		for (int j = 0; j<width; j++)
//		{
//			uchar r_32 = imagedata[lineNum_32 + j * 4 + 2];
//			uchar g_32 = imagedata[lineNum_32 + j * 4 + 1];
//			uchar b_32 = imagedata[lineNum_32 + j * 4];
//
//			graydata[i*W_8 + j] = r_32*0.299 + g_32*0.581 + b_32*0.12;
//		}
//	}
//
//	QVector<QRgb>  colorTable;
//	for (int k = 0; k<256; ++k)
//	{
//		colorTable.push_back(qRgb(k, k, k));
//	}
//	QImage *img = new QImage(graydata, W_8, height, QImage::Format_Indexed8);
//	img->setColorTable(colorTable);
//	return img;
//}

//QImage* MainWindow::gray2rgb(QImage * image)
//{
//	int width = image->width();
//	int height = image->height();
//	uchar* imagedata = image->bits();
//
//	uchar* rgbdata = new uchar[width*height * 4];
//	for (int i = 0; i<height; i++)
//	{
//		for (int j = 0; j<width; j++)
//		{
//			rgbdata[i*width * 4 + j * 4 + 2] = imagedata[i*width + j];
//			rgbdata[i*width * 4 + j * 4 + 1] = imagedata[i*width + j];
//			rgbdata[i*width * 4 + j * 4] = imagedata[i*width + j];
//			rgbdata[i*width * 4 + j * 4 + 3] = 255;
//		}
//	}
//	QImage *img = new QImage(rgbdata, width, height, QImage::Format_RGB32);
//	return img;
//}

void MainWindow::save()
{
	QString filename = QFileDialog::getSaveFileName(0, "Choose Picture", "", "*.jpg *.png *.bmp", 0);
	if (filename.isNull())
	{
		return;
	}
	
	m_img.save(filename);
}

//void MainWindow::sleep(unsigned int msec)
//{
//	QTime dieTime = QTime::currentTime().addMSecs(msec);
//	while (QTime::currentTime() < dieTime)
//		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
//}


void MainWindow::lineTransSlot()
{
	Base* b = new LinearTransform();
	b->setInputImage(&m_img);
	if (!RunWithParam(b)) return;
	if (b->getOutputImage().size())
	{
		int cnt = b->transObj.outputImage.size();
		display(
			b->getOutputImage().at(cnt - 1),
			QStringLiteral("线性变换")
			);
	}
	delete b;
}

void MainWindow::BloodVeseelSegSlot()
{
	Base* b = new BloodVesselSeg();
	b->setInputImage(&m_img);
	if (!RunWithParam(b)) return;
	if (b->getOutputImage().size())
	{
		int cnt = b->transObj.outputImage.size();
		display(
			b->getOutputImage().at(cnt - 1), //显示最后一张存入transObj.outputImage的图片
			QStringLiteral("血管分割")
			);
	}
	delete b;
}

void MainWindow::OpticDiskSegSlot()
{
	Base* b = new OpticDiskSeg();
	if (ImgList.size()==0)
	{
		QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("还未保存血管图像"));
		return;
	}
	QImage *SaveBVimgInput = ImgList.back();
	b->setInputImage(SaveBVimgInput);
	b->setInputImage(&m_img);
	if (!RunWithParam(b)) return;
	if (b->getOutputImage().size())
	{
		int cnt = b->transObj.outputImage.size();
		display(
			b->getOutputImage().at(cnt - 1), 
			QStringLiteral("视盘分割")
			);
		display(
			b->getOutputImage().at(cnt - 2),
			QStringLiteral("视盘模板图像")
			);
		/*display(
			b->getOutputImage().at(cnt - 3),
			QStringLiteral("视盘框定")
			);*/
		
	}
	intParams_MainFrm.push_back(b->IntParam.back());
	ellipsemergeOD_MainFrm.push_back(b->ellipseParam.back());
	ImgList.clear();
	delete b;
}

void MainWindow::FoveaSegSlot()
{
	Base* b = new FoveaSeg();
	if (ImgList.size() == 0)
	{
		QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("还未保存视盘模板图像"));
		return;
	}
	QImage *SaveimgInput = ImgList.back();
	b->setInputImage(SaveimgInput);
	b->setInputImage(&m_img);
	b->ellipseParam.push_back(ellipsemergeOD_MainFrm.back());
	b->IntParam.push_back(intParams_MainFrm.back());
	if (!RunWithParam(b)) return;
	if (b->getOutputImage().size())
	{
		int cnt = b->transObj.outputImage.size();
		//display(
		//	b->getOutputImage().at(cnt - 1),
		//	QStringLiteral("黄斑分割")
		//	);
		display(
			b->getOutputImage().at(cnt - 1),
			QStringLiteral("黄斑定位")
			);

	}
	ellipsemergeOD_MainFrm.clear();
	intParams_MainFrm.clear();
	//ImgList.clear();//渗出需要用到这个模板
	
	delete b;
}


void MainWindow::ExudationSlot()
{
	Base* b = new ExudationSeg();
	if (ImgList.size() == 0)
	{
		QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("还未保存视盘模板图像"));
		return;
	}
	QImage *SaveimgInput = ImgList.back();
	b->setInputImage(SaveimgInput);
	b->setInputImage(&m_img);
	
	if (!RunWithParam(b)) return;
	if (b->getOutputImage().size())
	{
		int cnt = b->transObj.outputImage.size();
		//display(
		//	b->getOutputImage().at(cnt - 1),
		//	QStringLiteral("黄斑分割")
		//	);
		display(
			b->getOutputImage().at(cnt - 1),
			QStringLiteral("渗出")
			);
		/*display(
			b->getOutputImage().at(cnt - 2),
			QStringLiteral("渗出模板")
			);*/

	}
	ImgList.clear();
	delete b;
}
void MainWindow::SaveImgSlot()
{
	if (m_img.format() != QImage::Format_Indexed8)
	{
		QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("存储图像格式错误"));
		return;
	}
	ImgList.push_back(&m_img);
	
	QMessageBox::about(NULL, QStringLiteral("存储图像"), QStringLiteral("存储图像完毕") );
}

void MainWindow::ClearImgListSlot()
{
	ImgList.clear();
	QMessageBox::about(NULL, QStringLiteral("清空"), QStringLiteral("后台图像已清空"));
}

void MainWindow::powerTransSlot()
{
	Base* b = new PowerTransform();
	b->setInputImage(&m_img);
	if (!RunWithParam(b)) return;
	if (b->getOutputImage().size())
	{
		int cnt = b->transObj.outputImage.size();
		display(
			b->getOutputImage().at(cnt - 1),
			QStringLiteral("幂率变换")
			);
	}
	delete b;
	
}

void MainWindow::histEqSlot()  //没有参数输入，所以直接run了
{
	Base* b = new HistEquation();
	b->setInputImage(&m_img);
	QTime time;
	time.start();
	b->run();
	QMessageBox dlg;
	QString info;
	info = QString::asprintf(" runtime =  %d ms",
		time.elapsed());
	dlg.setText(info);
	dlg.exec();
	if (b->getOutputImage().size())
	{
		int cnt = b->transObj.outputImage.size();
		display(
			b->getOutputImage().at(cnt - 1),
			QStringLiteral("直方图均衡")
			);
	}
	delete b;
	
}




void MainWindow::aboutSlot()
{
	QDialog *w = new QDialog;
	QLabel * label1 = new QLabel(QStringLiteral("开发：眼底医学图像处理"));
	QLabel * label2 = new QLabel(QStringLiteral("       电子科技大学.成像探测与智能感知实验室（IDIP）"));
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label1);
	layout->addWidget(label2);
	w->setLayout(layout);
	w->setWindowTitle(QStringLiteral("关于"));
	w->resize(150, 60);
	
	w->setWindowIcon(QIcon(QStringLiteral("images/about.png")));
	w->exec();
}

void MainWindow::display(unsigned char* data, int width, int height)
{
	QVector<QRgb>  colorTable;
	for (int k = 0; k<256; ++k)
	{
		colorTable.push_back(qRgb(k, k, k));
	}
	QImage *img = new QImage(data, width, height, QImage::Format_Indexed8);
	img->setColorTable(colorTable);
	QLabel *label = new QLabel;
	label->setPixmap(QPixmap::fromImage(*img));
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addWidget(label);
	QWidget* wn = new QWidget;
	wn->setLayout(hlayout);
	wn->show();
}

QDialog::DialogCode MainWindow::initParamWindow(Base* base)
{
	
	if (base->ListOfParam.size() == 0)
		return QDialog::Accepted;

	ParamWindow pw(base->ListOfParam, this);
	QString *para = new QString();
	if (pw.exec() == QDialog::Accepted) //直到有返回信号，进入读取参数
	{
		for (int i = 0; i < base->getNums(); i++)
		{
			para = pw.getPara(i); //getPara将界面上输入的参数保存起来

			switch (base->ListOfParam.at(i).value.varType)
			{
			case T_INT:
				base->ListOfParam.at(i).value.nVal = para->toInt();  //如果原本这个参数定义的是int就把字符转为int
				break;
			case T_DOUBLE:
				base->ListOfParam.at(i).value.dbVal = para->toDouble();
				break;
			default:
				Q_ASSERT(0);//not implemented yet
				break;
			}
		}
		return QDialog::Accepted;
	}
	else
	{
		return QDialog::Rejected;
	}
	//}

}

void MainWindow::display(QImage* img, const QString& title)
{
	MainWindow *pNewWnd = new MainWindow();
	pNewWnd->showMaximized();
	pNewWnd->LoadFromImage(img,title);
}

bool MainWindow::RunWithParam(Base* b)
{
	while (1){
		if (initParamWindow(b) == QDialog::Rejected)
		{
			delete b;
			return false;
		}
		long code = b->run();  //运行该参数
		switch (code){
		case ERROR_INVALID_ARG:
			QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("输入参数有误"));
			break;
		case ERROR_INVALID_IMGTYPE:
			QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("输入图像类型错误(非彩色图像)，不适用此算法"));
			break;
		case ALL_OK:
			return true;
		default:
			return false;
		}
	}
}
