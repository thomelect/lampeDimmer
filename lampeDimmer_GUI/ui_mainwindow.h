/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lbIntensiteValue;
    QDial *dialIntensite;
    QSlider *horizontalSliderIntensite;
    QPushButton *pushBottonOnOff;
    QLabel *lb_0_slider;
    QLabel *lb_255_slider;
    QLabel *lb_255_dial;
    QLabel *lb_0_dial;
    QComboBox *comboBoxSleep;
    QPushButton *pbReboot;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(295, 173);
        MainWindow->setMaximumSize(QSize(300, 320));
        MainWindow->setDocumentMode(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        lbIntensiteValue = new QLabel(centralwidget);
        lbIntensiteValue->setObjectName(QString::fromUtf8("lbIntensiteValue"));
        lbIntensiteValue->setEnabled(true);
        lbIntensiteValue->setGeometry(QRect(217, 110, 47, 22));
        QFont font;
        font.setFamily(QString::fromUtf8("Calibri"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        lbIntensiteValue->setFont(font);
        lbIntensiteValue->setAlignment(Qt::AlignCenter);
        dialIntensite = new QDial(centralwidget);
        dialIntensite->setObjectName(QString::fromUtf8("dialIntensite"));
        dialIntensite->setGeometry(QRect(220, 10, 50, 64));
        dialIntensite->setMaximum(255);
        dialIntensite->setWrapping(false);
        dialIntensite->setNotchTarget(64.000000000000000);
        dialIntensite->setNotchesVisible(true);
        horizontalSliderIntensite = new QSlider(centralwidget);
        horizontalSliderIntensite->setObjectName(QString::fromUtf8("horizontalSliderIntensite"));
        horizontalSliderIntensite->setGeometry(QRect(37, 110, 160, 22));
        horizontalSliderIntensite->setMaximum(255);
        horizontalSliderIntensite->setOrientation(Qt::Horizontal);
        horizontalSliderIntensite->setInvertedAppearance(false);
        horizontalSliderIntensite->setTickPosition(QSlider::TicksAbove);
        horizontalSliderIntensite->setTickInterval(64);
        pushBottonOnOff = new QPushButton(centralwidget);
        pushBottonOnOff->setObjectName(QString::fromUtf8("pushBottonOnOff"));
        pushBottonOnOff->setGeometry(QRect(130, 20, 65, 65));
        lb_0_slider = new QLabel(centralwidget);
        lb_0_slider->setObjectName(QString::fromUtf8("lb_0_slider"));
        lb_0_slider->setEnabled(true);
        lb_0_slider->setGeometry(QRect(31, 90, 21, 20));
        lb_0_slider->setAlignment(Qt::AlignCenter);
        lb_255_slider = new QLabel(centralwidget);
        lb_255_slider->setObjectName(QString::fromUtf8("lb_255_slider"));
        lb_255_slider->setEnabled(true);
        lb_255_slider->setGeometry(QRect(180, 90, 21, 20));
        lb_255_slider->setAlignment(Qt::AlignCenter);
        lb_255_dial = new QLabel(centralwidget);
        lb_255_dial->setObjectName(QString::fromUtf8("lb_255_dial"));
        lb_255_dial->setEnabled(true);
        lb_255_dial->setGeometry(QRect(250, 60, 21, 20));
        lb_255_dial->setAlignment(Qt::AlignCenter);
        lb_0_dial = new QLabel(centralwidget);
        lb_0_dial->setObjectName(QString::fromUtf8("lb_0_dial"));
        lb_0_dial->setEnabled(true);
        lb_0_dial->setGeometry(QRect(220, 60, 21, 20));
        lb_0_dial->setAlignment(Qt::AlignCenter);
        comboBoxSleep = new QComboBox(centralwidget);
        comboBoxSleep->addItem(QString());
        comboBoxSleep->addItem(QString());
        comboBoxSleep->addItem(QString());
        comboBoxSleep->addItem(QString());
        comboBoxSleep->addItem(QString());
        comboBoxSleep->setObjectName(QString::fromUtf8("comboBoxSleep"));
        comboBoxSleep->setGeometry(QRect(20, 50, 98, 18));
        comboBoxSleep->setStyleSheet(QString::fromUtf8(""));
        comboBoxSleep->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        comboBoxSleep->setDuplicatesEnabled(false);
        comboBoxSleep->setFrame(true);
        pbReboot = new QPushButton(centralwidget);
        pbReboot->setObjectName(QString::fromUtf8("pbReboot"));
        pbReboot->setGeometry(QRect(30, 10, 80, 25));
        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Lampe contr\303\264leur", nullptr));
        lbIntensiteValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        pushBottonOnOff->setText(QString());
        lb_0_slider->setText(QApplication::translate("MainWindow", "0", nullptr));
        lb_255_slider->setText(QApplication::translate("MainWindow", "255", nullptr));
        lb_255_dial->setText(QApplication::translate("MainWindow", "255", nullptr));
        lb_0_dial->setText(QApplication::translate("MainWindow", "0", nullptr));
        comboBoxSleep->setItemText(0, QApplication::translate("MainWindow", "NONE", nullptr));
        comboBoxSleep->setItemText(1, QApplication::translate("MainWindow", "OFF", nullptr));
        comboBoxSleep->setItemText(2, QApplication::translate("MainWindow", "ON", nullptr));
        comboBoxSleep->setItemText(3, QApplication::translate("MainWindow", "BREATHING", nullptr));
        comboBoxSleep->setItemText(4, QApplication::translate("MainWindow", "VEILLEUSE", nullptr));

        pbReboot->setText(QApplication::translate("MainWindow", "Red\303\251marrer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
