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
	
	//���ô��ڵı���ͼƬ
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
	setWindowIcon(QIcon(tr("images/robot_64px.png")));//���ô���ͼ�ꣻ
	//setWindowState(Qt::WindowMaximized);//ʹ���ڵ�״̬Ϊ���
	QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();Ҳ����
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
	//�������ļ����˵��ж�Ӧ�Ķ�����
	openOneAction = new QAction(QStringLiteral("��ͼƬ"), this);
	openOneAction->setIcon(QIcon("images/circle_green_32px.ico"));
	connect(openOneAction, SIGNAL(triggered()), this, SLOT(openOne()));
	/*openTwoAction = new QAction(QStringLiteral("������ͼ"), this);
	openTwoAction->setIcon(QIcon("images/open_64px.png"));
	connect(openTwoAction, SIGNAL(triggered()), this, SLOT(openTwo()));

	openSeriesAction = new QAction(QStringLiteral("��ͼ������"), this);
	connect(openSeriesAction, SIGNAL(triggered()), this, SLOT(openSeries()));

	openVideoAction = new QAction(QStringLiteral("��Ƶ����ģ��"), this);
	connect(openVideoAction, SIGNAL(triggered()), this, SLOT(openVideo()));*/

	saveAction = new QAction(QStringLiteral("����ͼƬ"), this);
	saveAction->setIcon(QIcon("images/Save_32px.png"));
	saveAction->setEnabled(false);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	exitAction = new QAction(QStringLiteral("�ر�"), this);
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	//����about����;
	aboutaction = new QAction(QStringLiteral("�������"), this);
	connect(aboutaction, SIGNAL(triggered()), this, SLOT(aboutSlot()));

	//��������ͼ������;
	vesselSegmentation = new QAction(QStringLiteral("Ѫ�ָܷ�"), this);
	connect(vesselSegmentation, SIGNAL(triggered()), this, SLOT(BloodVeseelSegSlot()));
	opticDiskSegmentation = new QAction(QStringLiteral("���̷ָ�"), this);
	connect(opticDiskSegmentation, SIGNAL(triggered()), this, SLOT(OpticDiskSegSlot()));
	foveaLocalization = new QAction(QStringLiteral("�ư߶�λ"), this);
	connect(foveaLocalization, SIGNAL(triggered()), this, SLOT(FoveaSegSlot())); 
	exudationSegmentation = new QAction(QStringLiteral("�����ָ�"), this);
	connect(exudationSegmentation, SIGNAL(triggered()), this, SLOT(ExudationSlot()));
	DRClassification = new QAction(QStringLiteral("�ּ����"), this);
	
	//���������ϴ�����
	pupilExtraction = new QAction(QStringLiteral("ͫ����ȡ"), this);
	CAClassfication = new QAction(QStringLiteral("�ּ����"), this);

	//��������������
	saveImg = new QAction(QStringLiteral("�洢ͼ��"), this);
	connect(saveImg, SIGNAL(triggered()), this, SLOT(SaveImgSlot()));
	clearImgList = new QAction(QStringLiteral("���"), this);
	connect(clearImgList, SIGNAL(triggered()), this, SLOT(ClearImgListSlot()));
}

//��ʼ���˵�
void MainWindow::initialMenu()
{
	//�����ļ��˵���
	fileMenu = new QMenu(QStringLiteral("�ļ�"));
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

	//about�˵�;
	helpmenu = new QMenu(QStringLiteral("����"));
	helpmenu->addAction(aboutaction);
	helpmenu->setFont(QFont("Times", 12, QFont::Normal));

	//����ͼ����˵�;
	graytranformmenu = new QMenu(QStringLiteral("��������Ĥͼ����"));
	graytranformmenu->addAction(vesselSegmentation);
	graytranformmenu->addAction(opticDiskSegmentation);
	graytranformmenu->addAction(foveaLocalization);
	graytranformmenu->addAction(exudationSegmentation);
	graytranformmenu->addAction(DRClassification);
	//graytranformmenu->addAction(histmatchaction);
	graytranformmenu->setFont(QFont("Times", 12, QFont::Normal));

	//������ͼ����˵�
	cataractmenu = new QMenu(QStringLiteral("������ͼ����"));
	cataractmenu->addAction(pupilExtraction);
	cataractmenu->addAction(CAClassfication);
	cataractmenu->setFont(QFont("Times", 12, QFont::Normal));


	//���浱ǰͼ�����ں�������/���
	nextprocess = new QMenu(QStringLiteral("��������"));
	nextprocess->addAction(saveImg);
	nextprocess->addAction(clearImgList);
	nextprocess->setFont(QFont("Times", 12, QFont::Normal));

	//view menu
	viewMenu = menuBar()->addMenu(QStringLiteral("��ͼ"));

	zoomInAct = viewMenu->addAction(QStringLiteral("�Ŵ� (+25%)"), this, &MainWindow::zoomIn);
	zoomInAct->setEnabled(false);

	zoomOutAct = viewMenu->addAction(QStringLiteral("��С (-25%)"), this, &MainWindow::zoomOut);
	zoomOutAct->setEnabled(false);

	normalSizeAct = viewMenu->addAction(QStringLiteral("ԭʼ�ߴ�"), this, &MainWindow::normalSize);
	normalSizeAct->setEnabled(false);
	viewMenu->setFont(QFont("Times", 12, QFont::Normal));
	setMenu(0);
}

//���ò˵�����
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

//����ͼƬ,��������ʱ
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

//�򿪵���ͼ��
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

//������ͼ��
//void MainWindow::openTwo()
//{
//	//��ȡ��һ��ͼ����ʾ;
//	QString filename1 = QFileDialog::getOpenFileName(0, "Choose Picture", tr(""), "*.jpg *.png *.bmp", 0);
//	if (filename1.isNull()) {
//		return;
//	}
//	
//
//	//��ȡ�ڶ���ͼ����ʾ;
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
//	//������ͼʱ����رմ��ڹز��ˣ�
//	QStringList filename = QFileDialog::getOpenFileNames(0, "Select one or more files to open", tr("E:/����/����/���������Ӱ�/¼��Ƶ/ͼ������/С�ɻ�"), "Images (*.png *.xpm *.jpg *.bmp)", 0);
//	if (filename.length() == 0)
//	{
//		return;
//	}
//	const int size = filename.size();//length()��size��һ���ģ�
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
//	uchar *graydata = new uchar[height*W_8];//���ﲻ��width����W_8��ԭ�������Ǵ洢������ɵģ�Ҫʹ��32λ���룬����32λͼ������ͼ��Ҫ�������Ĵ���
//	int  lineNum_32 = 0;     //����;
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
			QStringLiteral("���Ա任")
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
			b->getOutputImage().at(cnt - 1), //��ʾ���һ�Ŵ���transObj.outputImage��ͼƬ
			QStringLiteral("Ѫ�ָܷ�")
			);
	}
	delete b;
}

void MainWindow::OpticDiskSegSlot()
{
	Base* b = new OpticDiskSeg();
	if (ImgList.size()==0)
	{
		QMessageBox::warning(this, QStringLiteral("������Ϣ"), QStringLiteral("��δ����Ѫ��ͼ��"));
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
			QStringLiteral("���̷ָ�")
			);
		display(
			b->getOutputImage().at(cnt - 2),
			QStringLiteral("����ģ��ͼ��")
			);
		/*display(
			b->getOutputImage().at(cnt - 3),
			QStringLiteral("���̿�")
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
		QMessageBox::warning(this, QStringLiteral("������Ϣ"), QStringLiteral("��δ��������ģ��ͼ��"));
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
		//	QStringLiteral("�ư߷ָ�")
		//	);
		display(
			b->getOutputImage().at(cnt - 1),
			QStringLiteral("�ư߶�λ")
			);

	}
	ellipsemergeOD_MainFrm.clear();
	intParams_MainFrm.clear();
	//ImgList.clear();//������Ҫ�õ����ģ��
	
	delete b;
}


void MainWindow::ExudationSlot()
{
	Base* b = new ExudationSeg();
	if (ImgList.size() == 0)
	{
		QMessageBox::warning(this, QStringLiteral("������Ϣ"), QStringLiteral("��δ��������ģ��ͼ��"));
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
		//	QStringLiteral("�ư߷ָ�")
		//	);
		display(
			b->getOutputImage().at(cnt - 1),
			QStringLiteral("����")
			);
		/*display(
			b->getOutputImage().at(cnt - 2),
			QStringLiteral("����ģ��")
			);*/

	}
	ImgList.clear();
	delete b;
}
void MainWindow::SaveImgSlot()
{
	if (m_img.format() != QImage::Format_Indexed8)
	{
		QMessageBox::warning(this, QStringLiteral("������Ϣ"), QStringLiteral("�洢ͼ���ʽ����"));
		return;
	}
	ImgList.push_back(&m_img);
	
	QMessageBox::about(NULL, QStringLiteral("�洢ͼ��"), QStringLiteral("�洢ͼ�����") );
}

void MainWindow::ClearImgListSlot()
{
	ImgList.clear();
	QMessageBox::about(NULL, QStringLiteral("���"), QStringLiteral("��̨ͼ�������"));
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
			QStringLiteral("���ʱ任")
			);
	}
	delete b;
	
}

void MainWindow::histEqSlot()  //û�в������룬����ֱ��run��
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
			QStringLiteral("ֱ��ͼ����")
			);
	}
	delete b;
	
}




void MainWindow::aboutSlot()
{
	QDialog *w = new QDialog;
	QLabel * label1 = new QLabel(QStringLiteral("�������۵�ҽѧͼ����"));
	QLabel * label2 = new QLabel(QStringLiteral("       ���ӿƼ���ѧ.����̽�������ܸ�֪ʵ���ң�IDIP��"));
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label1);
	layout->addWidget(label2);
	w->setLayout(layout);
	w->setWindowTitle(QStringLiteral("����"));
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
	if (pw.exec() == QDialog::Accepted) //ֱ���з����źţ������ȡ����
	{
		for (int i = 0; i < base->getNums(); i++)
		{
			para = pw.getPara(i); //getPara������������Ĳ�����������

			switch (base->ListOfParam.at(i).value.varType)
			{
			case T_INT:
				base->ListOfParam.at(i).value.nVal = para->toInt();  //���ԭ����������������int�Ͱ��ַ�תΪint
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
		long code = b->run();  //���иò���
		switch (code){
		case ERROR_INVALID_ARG:
			QMessageBox::warning(this, QStringLiteral("������Ϣ"), QStringLiteral("�����������"));
			break;
		case ERROR_INVALID_IMGTYPE:
			QMessageBox::warning(this, QStringLiteral("������Ϣ"), QStringLiteral("����ͼ�����ʹ���(�ǲ�ɫͼ��)�������ô��㷨"));
			break;
		case ALL_OK:
			return true;
		default:
			return false;
		}
	}
}
