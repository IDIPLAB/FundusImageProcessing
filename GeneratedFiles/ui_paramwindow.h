/********************************************************************************
** Form generated from reading UI file 'paramwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMWINDOW_H
#define UI_PARAMWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ParamWindow
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *paramWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *ParamWindow)
    {
        if (ParamWindow->objectName().isEmpty())
            ParamWindow->setObjectName(QStringLiteral("ParamWindow"));
        ParamWindow->resize(482, 333);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(ParamWindow->sizePolicy().hasHeightForWidth());
        ParamWindow->setSizePolicy(sizePolicy);
        ParamWindow->setSizeGripEnabled(false);
        verticalLayout = new QVBoxLayout(ParamWindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        paramWidget = new QTableWidget(ParamWindow);
        if (paramWidget->columnCount() < 3)
            paramWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        paramWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        paramWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        paramWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        paramWidget->setObjectName(QStringLiteral("paramWidget"));
        paramWidget->horizontalHeader()->setDefaultSectionSize(147);

        verticalLayout->addWidget(paramWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(ParamWindow);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(ParamWindow);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ParamWindow);

        QMetaObject::connectSlotsByName(ParamWindow);
    } // setupUi

    void retranslateUi(QDialog *ParamWindow)
    {
        ParamWindow->setWindowTitle(QApplication::translate("ParamWindow", "ParamWindow", 0));
        QTableWidgetItem *___qtablewidgetitem = paramWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ParamWindow", "\345\217\202\346\225\260\345\220\215", 0));
        QTableWidgetItem *___qtablewidgetitem1 = paramWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ParamWindow", "\345\217\202\346\225\260\345\200\274", 0));
        QTableWidgetItem *___qtablewidgetitem2 = paramWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ParamWindow", "\345\217\202\346\225\260\350\257\264\346\230\216", 0));
        pushButton_2->setText(QApplication::translate("ParamWindow", "OK", 0));
        pushButton->setText(QApplication::translate("ParamWindow", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ParamWindow: public Ui_ParamWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMWINDOW_H
