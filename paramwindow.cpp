#include "paramwindow.h"
#include "ui_paramwindow.h"

ParamWindow::ParamWindow(QWidget *parent)
: QDialog(parent)
{
	ui = new Ui::ParamWindow();
	ui->setupUi(this);
}

ParamWindow::ParamWindow(vector<ParamValue> params, QWidget* parent /* = 0 */)
: QDialog(parent)
{
	ui = new Ui::ParamWindow();
	ui->setupUi(this);
	int strCount = params.size();
	int paraRowCount = ui->paramWidget->rowCount();
	ui->paramWidget->setRowCount(paraRowCount + strCount);
	ui->paramWidget->setColumnCount(3);//设置列数为3
	//设置字体大小
	QFont font;
	font.setPixelSize(14);
	for (int i = 0; i < strCount; i++)
	{
		QTableWidgetItem *item_name = new QTableWidgetItem(params.at(i).name);
		item_name->setFlags(QFlag(32));//设置文字为不可编辑
		item_name->setFont(font);
		//item_name->setForeground(QBrush(QColor(255, 0, 0)));//设置字体颜色
		ui->paramWidget->setItem(i, 0, item_name);	
		QString strVal;
		switch (params.at(i).value.varType)
		{
		case T_INT:
			strVal = QString::asprintf("%d", params.at(i).value.nVal);
			break;
		case T_DOUBLE:
			if (fabs(params.at(i).value.dbVal) < 1e-6 && fabs(params.at(i).value.dbVal) > 0)
				strVal = QString::asprintf("%e", params.at(i).value.dbVal);
			else
				strVal = QString::asprintf("%4.6f", params.at(i).value.dbVal);
			break;
		default:
			break;
		}
		QTableWidgetItem *item_val = new QTableWidgetItem(strVal);
		item_val->setFont(font);
		ui->paramWidget->setItem(i, 1, item_val);	
		//ui->paramWidget->item(i, 1)->setText(strVal);
		if (!params.at(i).explain.isEmpty()){
			QTableWidgetItem *item_exp = new QTableWidgetItem(params.at(i).explain);
			item_exp->setFlags(QFlag(32));
			item_exp->setFont(font);
			ui->paramWidget->setItem(i, 2, item_exp);
			this->setWindowTitle(QStringLiteral("输入"));
			this->setFixedWidth(482);//设置窗口固定宽度
		}else{
			ui->paramWidget->setColumnHidden(2, true);
			//ui->paramWidget->resizeColumnToContents(2);
			this->setWindowTitle(QStringLiteral("输出"));
			this->setFixedWidth(333);
		}
		//this->adjustSize();
		ui->paramWidget->resizeRowsToContents();
	}

	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(okSlot()));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));
}

ParamWindow::~ParamWindow()
{
	delete ui;
}

void ParamWindow::okSlot(){
	this->accept();
}

void ParamWindow::cancelSlot(){
	this->reject();
}

QString* ParamWindow::getPara(int i) {
	QString *params = new QString();
	params->append(ui->paramWidget->item(i, 1)->text());
	return params;
}