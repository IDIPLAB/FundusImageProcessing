/********************************************************************************
** Form generated from reading UI file 'displaypicturewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYPICTUREWINDOW_H
#define UI_DISPLAYPICTUREWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplayPictureWindow
{
public:

    void setupUi(QWidget *DisplayPictureWindow)
    {
        if (DisplayPictureWindow->objectName().isEmpty())
            DisplayPictureWindow->setObjectName(QStringLiteral("DisplayPictureWindow"));
        DisplayPictureWindow->resize(400, 300);

        retranslateUi(DisplayPictureWindow);

        QMetaObject::connectSlotsByName(DisplayPictureWindow);
    } // setupUi

    void retranslateUi(QWidget *DisplayPictureWindow)
    {
        DisplayPictureWindow->setWindowTitle(QApplication::translate("DisplayPictureWindow", "DisplayPictureWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class DisplayPictureWindow: public Ui_DisplayPictureWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYPICTUREWINDOW_H
