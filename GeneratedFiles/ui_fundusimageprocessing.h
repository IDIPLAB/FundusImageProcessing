/********************************************************************************
** Form generated from reading UI file 'fundusimageprocessing.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FUNDUSIMAGEPROCESSING_H
#define UI_FUNDUSIMAGEPROCESSING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FundusImageProcessingClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FundusImageProcessingClass)
    {
        if (FundusImageProcessingClass->objectName().isEmpty())
            FundusImageProcessingClass->setObjectName(QStringLiteral("FundusImageProcessingClass"));
        FundusImageProcessingClass->resize(600, 400);
        menuBar = new QMenuBar(FundusImageProcessingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FundusImageProcessingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FundusImageProcessingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FundusImageProcessingClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FundusImageProcessingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FundusImageProcessingClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FundusImageProcessingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FundusImageProcessingClass->setStatusBar(statusBar);

        retranslateUi(FundusImageProcessingClass);

        QMetaObject::connectSlotsByName(FundusImageProcessingClass);
    } // setupUi

    void retranslateUi(QMainWindow *FundusImageProcessingClass)
    {
        FundusImageProcessingClass->setWindowTitle(QApplication::translate("FundusImageProcessingClass", "FundusImageProcessing", 0));
    } // retranslateUi

};

namespace Ui {
    class FundusImageProcessingClass: public Ui_FundusImageProcessingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FUNDUSIMAGEPROCESSING_H
