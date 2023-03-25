/********************************************************************************
** Form generated from reading UI file 'sandbox.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SANDBOX_H
#define UI_SANDBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SandBox
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *canvasWidget_1;
    QFrame *canvasWidget_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SandBox)
    {
        if (SandBox->objectName().isEmpty())
            SandBox->setObjectName("SandBox");
        SandBox->resize(800, 600);
        centralwidget = new QWidget(SandBox);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        canvasWidget_1 = new QFrame(centralwidget);
        canvasWidget_1->setObjectName("canvasWidget_1");
        canvasWidget_1->setFrameShape(QFrame::Box);
        canvasWidget_1->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(canvasWidget_1, 0, 0, 1, 1);

        canvasWidget_2 = new QFrame(centralwidget);
        canvasWidget_2->setObjectName("canvasWidget_2");
        canvasWidget_2->setFrameShape(QFrame::Box);
        canvasWidget_2->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(canvasWidget_2, 0, 1, 1, 1);

        SandBox->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SandBox);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        SandBox->setMenuBar(menubar);
        statusbar = new QStatusBar(SandBox);
        statusbar->setObjectName("statusbar");
        SandBox->setStatusBar(statusbar);

        retranslateUi(SandBox);

        QMetaObject::connectSlotsByName(SandBox);
    } // setupUi

    void retranslateUi(QMainWindow *SandBox)
    {
        SandBox->setWindowTitle(QCoreApplication::translate("SandBox", "SandBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SandBox: public Ui_SandBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SANDBOX_H
