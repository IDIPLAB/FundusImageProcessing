#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QDialog>
#include <QList>
#include "base.h"
namespace Ui { class ParamWindow; };

class ParamWindow : public QDialog
{
	Q_OBJECT

public:
	ParamWindow(QWidget *parent = 0);
	//ParamWindow(QStringList strList, QWidget *parent = 0);
	ParamWindow(vector<ParamValue>, QWidget* parent = 0);
	~ParamWindow();

	QString* getPara(int i);

	public slots :
	void okSlot();
	void cancelSlot();


private:
	Ui::ParamWindow *ui;
};

#endif // PARAMWINDOW_H
