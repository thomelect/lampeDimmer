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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QDial *dialIntensite;
    QPushButton *pushBottonOnOff;
    QLabel *lbIntensiteValue;
    QSlider *horizontalSliderIntensite;
    QComboBox *comboBoxSleep;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(242, 312);
        MainWindow->setMaximumSize(QSize(300, 320));
        MainWindow->setDocumentMode(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(40, 30, 160, 201));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        dialIntensite = new QDial(verticalLayoutWidget);
        dialIntensite->setObjectName(QString::fromUtf8("dialIntensite"));
        dialIntensite->setMaximum(255);
        dialIntensite->setWrapping(false);
        dialIntensite->setNotchTarget(64.000000000000000);
        dialIntensite->setNotchesVisible(true);

        horizontalLayout_2->addWidget(dialIntensite);

        pushBottonOnOff = new QPushButton(verticalLayoutWidget);
        pushBottonOnOff->setObjectName(QString::fromUtf8("pushBottonOnOff"));

        horizontalLayout_2->addWidget(pushBottonOnOff);


        verticalLayout_2->addLayout(horizontalLayout_2);

        lbIntensiteValue = new QLabel(verticalLayoutWidget);
        lbIntensiteValue->setObjectName(QString::fromUtf8("lbIntensiteValue"));
        lbIntensiteValue->setEnabled(true);
        lbIntensiteValue->setMaximumSize(QSize(16777215, 25));
        QFont font;
        font.setFamily(QString::fromUtf8("Calibri"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        lbIntensiteValue->setFont(font);
        lbIntensiteValue->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lbIntensiteValue);

        horizontalSliderIntensite = new QSlider(verticalLayoutWidget);
        horizontalSliderIntensite->setObjectName(QString::fromUtf8("horizontalSliderIntensite"));
        horizontalSliderIntensite->setMaximum(255);
        horizontalSliderIntensite->setOrientation(Qt::Horizontal);
        horizontalSliderIntensite->setInvertedAppearance(false);
        horizontalSliderIntensite->setTickPosition(QSlider::TicksAbove);
        horizontalSliderIntensite->setTickInterval(64);

        verticalLayout_2->addWidget(horizontalSliderIntensite);

        comboBoxSleep = new QComboBox(verticalLayoutWidget);
        comboBoxSleep->addItem(QString());
        comboBoxSleep->addItem(QString());
        comboBoxSleep->addItem(QString());
        comboBoxSleep->addItem(QString());
        comboBoxSleep->addItem(QString());
        comboBoxSleep->setObjectName(QString::fromUtf8("comboBoxSleep"));
        comboBoxSleep->setStyleSheet(QString::fromUtf8(""));
        comboBoxSleep->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        comboBoxSleep->setDuplicatesEnabled(false);
        comboBoxSleep->setFrame(true);

        verticalLayout_2->addWidget(comboBoxSleep);

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
        pushBottonOnOff->setText(QString());
        lbIntensiteValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        comboBoxSleep->setItemText(0, QApplication::translate("MainWindow", "NONE", nullptr));
        comboBoxSleep->setItemText(1, QApplication::translate("MainWindow", "OFF", nullptr));
        comboBoxSleep->setItemText(2, QApplication::translate("MainWindow", "ON", nullptr));
        comboBoxSleep->setItemText(3, QApplication::translate("MainWindow", "BREATHING", nullptr));
        comboBoxSleep->setItemText(4, QApplication::translate("MainWindow", "VEILLEUSE", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
