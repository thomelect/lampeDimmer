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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QHeaderView>
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
    QPushButton *pushButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(295, 173);
        MainWindow->setMaximumSize(QSize(300, 320));
        MainWindow->setDocumentMode(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        lbIntensiteValue = new QLabel(centralwidget);
        lbIntensiteValue->setObjectName(QStringLiteral("lbIntensiteValue"));
        lbIntensiteValue->setEnabled(true);
        lbIntensiteValue->setGeometry(QRect(217, 110, 47, 22));
        QFont font;
        font.setFamily(QStringLiteral("Calibri"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        lbIntensiteValue->setFont(font);
        lbIntensiteValue->setAlignment(Qt::AlignCenter);
        dialIntensite = new QDial(centralwidget);
        dialIntensite->setObjectName(QStringLiteral("dialIntensite"));
        dialIntensite->setGeometry(QRect(220, 10, 50, 64));
        dialIntensite->setMaximum(255);
        dialIntensite->setWrapping(false);
        dialIntensite->setNotchTarget(64);
        dialIntensite->setNotchesVisible(true);
        horizontalSliderIntensite = new QSlider(centralwidget);
        horizontalSliderIntensite->setObjectName(QStringLiteral("horizontalSliderIntensite"));
        horizontalSliderIntensite->setGeometry(QRect(37, 110, 160, 22));
        horizontalSliderIntensite->setMaximum(255);
        horizontalSliderIntensite->setOrientation(Qt::Horizontal);
        horizontalSliderIntensite->setInvertedAppearance(false);
        horizontalSliderIntensite->setTickPosition(QSlider::TicksAbove);
        horizontalSliderIntensite->setTickInterval(64);
        pushBottonOnOff = new QPushButton(centralwidget);
        pushBottonOnOff->setObjectName(QStringLiteral("pushBottonOnOff"));
        pushBottonOnOff->setGeometry(QRect(130, 20, 65, 65));
        lb_0_slider = new QLabel(centralwidget);
        lb_0_slider->setObjectName(QStringLiteral("lb_0_slider"));
        lb_0_slider->setEnabled(true);
        lb_0_slider->setGeometry(QRect(31, 90, 21, 20));
        lb_0_slider->setAlignment(Qt::AlignCenter);
        lb_255_slider = new QLabel(centralwidget);
        lb_255_slider->setObjectName(QStringLiteral("lb_255_slider"));
        lb_255_slider->setEnabled(true);
        lb_255_slider->setGeometry(QRect(180, 90, 21, 20));
        lb_255_slider->setAlignment(Qt::AlignCenter);
        lb_255_dial = new QLabel(centralwidget);
        lb_255_dial->setObjectName(QStringLiteral("lb_255_dial"));
        lb_255_dial->setEnabled(true);
        lb_255_dial->setGeometry(QRect(250, 60, 21, 20));
        lb_255_dial->setAlignment(Qt::AlignCenter);
        lb_0_dial = new QLabel(centralwidget);
        lb_0_dial->setObjectName(QStringLiteral("lb_0_dial"));
        lb_0_dial->setEnabled(true);
        lb_0_dial->setGeometry(QRect(220, 60, 21, 20));
        lb_0_dial->setAlignment(Qt::AlignCenter);
        comboBoxSleep = new QComboBox(centralwidget);
        comboBoxSleep->setObjectName(QStringLiteral("comboBoxSleep"));
        comboBoxSleep->setGeometry(QRect(20, 50, 98, 18));
        comboBoxSleep->setStyleSheet(QStringLiteral(""));
        comboBoxSleep->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        comboBoxSleep->setDuplicatesEnabled(false);
        comboBoxSleep->setFrame(true);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(50, 20, 31, 21));
        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Lampe contr\303\264leur", Q_NULLPTR));
        lbIntensiteValue->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        pushBottonOnOff->setText(QString());
        lb_0_slider->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lb_255_slider->setText(QApplication::translate("MainWindow", "255", Q_NULLPTR));
        lb_255_dial->setText(QApplication::translate("MainWindow", "255", Q_NULLPTR));
        lb_0_dial->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        comboBoxSleep->clear();
        comboBoxSleep->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "OFF", Q_NULLPTR)
         << QApplication::translate("MainWindow", "ON", Q_NULLPTR)
         << QApplication::translate("MainWindow", "BREATHING", Q_NULLPTR)
         << QApplication::translate("MainWindow", "VEILLEUSE", Q_NULLPTR)
        );
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
