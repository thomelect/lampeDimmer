/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lbIntensiteValue;
    QDial *dialIntensite;
    QSlider *horizontalSliderIntensite;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(273, 272);
        MainWindow->setMaximumSize(QSize(300, 320));
        MainWindow->setDocumentMode(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        lbIntensiteValue = new QLabel(centralwidget);
        lbIntensiteValue->setObjectName(QStringLiteral("lbIntensiteValue"));
        lbIntensiteValue->setGeometry(QRect(220, 180, 47, 22));
        QFont font;
        font.setFamily(QStringLiteral("Calibri"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        lbIntensiteValue->setFont(font);
        lbIntensiteValue->setAlignment(Qt::AlignCenter);
        dialIntensite = new QDial(centralwidget);
        dialIntensite->setObjectName(QStringLiteral("dialIntensite"));
        dialIntensite->setGeometry(QRect(210, 90, 50, 64));
        horizontalSliderIntensite = new QSlider(centralwidget);
        horizontalSliderIntensite->setObjectName(QStringLiteral("horizontalSliderIntensite"));
        horizontalSliderIntensite->setGeometry(QRect(40, 180, 160, 22));
        horizontalSliderIntensite->setOrientation(Qt::Horizontal);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Laboratoire 5", Q_NULLPTR));
        lbIntensiteValue->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
