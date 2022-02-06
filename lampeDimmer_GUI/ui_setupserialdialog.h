/********************************************************************************
** Form generated from reading UI file 'setupserialdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPSERIALDIALOG_H
#define UI_SETUPSERIALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SetupSerialDialog
{
public:
    QComboBox *cbListBaudRate;
    QComboBox *cbListPortSerie;
    QPushButton *btActualiser;
    QPushButton *btConnexion;
    QLabel *lbBaudRate;
    QPushButton *btAnnuler;
    QPushButton *btOk;

    void setupUi(QDialog *SetupSerialDialog)
    {
        if (SetupSerialDialog->objectName().isEmpty())
            SetupSerialDialog->setObjectName(QString::fromUtf8("SetupSerialDialog"));
        SetupSerialDialog->resize(280, 135);
        cbListBaudRate = new QComboBox(SetupSerialDialog);
        cbListBaudRate->setObjectName(QString::fromUtf8("cbListBaudRate"));
        cbListBaudRate->setGeometry(QRect(79, 70, 191, 22));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        cbListBaudRate->setPalette(palette);
        cbListPortSerie = new QComboBox(SetupSerialDialog);
        cbListPortSerie->setObjectName(QString::fromUtf8("cbListPortSerie"));
        cbListPortSerie->setGeometry(QRect(9, 40, 261, 22));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        cbListPortSerie->setPalette(palette1);
        btActualiser = new QPushButton(SetupSerialDialog);
        btActualiser->setObjectName(QString::fromUtf8("btActualiser"));
        btActualiser->setGeometry(QRect(9, 10, 121, 23));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        btActualiser->setPalette(palette2);
        btConnexion = new QPushButton(SetupSerialDialog);
        btConnexion->setObjectName(QString::fromUtf8("btConnexion"));
        btConnexion->setGeometry(QRect(139, 10, 131, 23));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        btConnexion->setPalette(palette3);
        lbBaudRate = new QLabel(SetupSerialDialog);
        lbBaudRate->setObjectName(QString::fromUtf8("lbBaudRate"));
        lbBaudRate->setGeometry(QRect(9, 73, 71, 16));
        QFont font;
        font.setPointSize(10);
        lbBaudRate->setFont(font);
        btAnnuler = new QPushButton(SetupSerialDialog);
        btAnnuler->setObjectName(QString::fromUtf8("btAnnuler"));
        btAnnuler->setGeometry(QRect(100, 100, 80, 25));
        btOk = new QPushButton(SetupSerialDialog);
        btOk->setObjectName(QString::fromUtf8("btOk"));
        btOk->setGeometry(QRect(190, 100, 80, 25));

        retranslateUi(SetupSerialDialog);

        QMetaObject::connectSlotsByName(SetupSerialDialog);
    } // setupUi

    void retranslateUi(QDialog *SetupSerialDialog)
    {
        SetupSerialDialog->setWindowTitle(QApplication::translate("SetupSerialDialog", "Dialog", nullptr));
        btActualiser->setText(QApplication::translate("SetupSerialDialog", "Actualiser", nullptr));
        btConnexion->setText(QApplication::translate("SetupSerialDialog", "Connecter", nullptr));
        lbBaudRate->setText(QApplication::translate("SetupSerialDialog", "Baud rate :", nullptr));
        btAnnuler->setText(QApplication::translate("SetupSerialDialog", "Annuler", nullptr));
        btOk->setText(QApplication::translate("SetupSerialDialog", "Confirmer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SetupSerialDialog: public Ui_SetupSerialDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPSERIALDIALOG_H
