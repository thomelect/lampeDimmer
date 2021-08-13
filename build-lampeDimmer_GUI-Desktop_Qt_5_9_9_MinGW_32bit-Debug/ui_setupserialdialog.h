/********************************************************************************
** Form generated from reading UI file 'setupserialdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPSERIALDIALOG_H
#define UI_SETUPSERIALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SetupSerialDialog
{
public:
    QComboBox *cbListBaudRate;
    QComboBox *cbListPortSerie;
    QPushButton *btActualiser;
    QPushButton *btConnection;
    QLabel *lbbaudRate;
    QPushButton *pbCANCEL;
    QPushButton *pbOK;

    void setupUi(QDialog *SetupSerialDialog)
    {
        if (SetupSerialDialog->objectName().isEmpty())
            SetupSerialDialog->setObjectName(QStringLiteral("SetupSerialDialog"));
        SetupSerialDialog->resize(280, 135);
        cbListBaudRate = new QComboBox(SetupSerialDialog);
        cbListBaudRate->setObjectName(QStringLiteral("cbListBaudRate"));
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
        cbListPortSerie->setObjectName(QStringLiteral("cbListPortSerie"));
        cbListPortSerie->setGeometry(QRect(9, 40, 261, 22));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        cbListPortSerie->setPalette(palette1);
        btActualiser = new QPushButton(SetupSerialDialog);
        btActualiser->setObjectName(QStringLiteral("btActualiser"));
        btActualiser->setGeometry(QRect(9, 10, 121, 23));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        btActualiser->setPalette(palette2);
        btConnection = new QPushButton(SetupSerialDialog);
        btConnection->setObjectName(QStringLiteral("btConnection"));
        btConnection->setGeometry(QRect(139, 10, 131, 23));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        btConnection->setPalette(palette3);
        lbbaudRate = new QLabel(SetupSerialDialog);
        lbbaudRate->setObjectName(QStringLiteral("lbbaudRate"));
        lbbaudRate->setGeometry(QRect(9, 73, 71, 16));
        QFont font;
        font.setPointSize(10);
        lbbaudRate->setFont(font);
        pbCANCEL = new QPushButton(SetupSerialDialog);
        pbCANCEL->setObjectName(QStringLiteral("pbCANCEL"));
        pbCANCEL->setGeometry(QRect(100, 100, 80, 25));
        pbOK = new QPushButton(SetupSerialDialog);
        pbOK->setObjectName(QStringLiteral("pbOK"));
        pbOK->setGeometry(QRect(190, 100, 80, 25));

        retranslateUi(SetupSerialDialog);

        QMetaObject::connectSlotsByName(SetupSerialDialog);
    } // setupUi

    void retranslateUi(QDialog *SetupSerialDialog)
    {
        SetupSerialDialog->setWindowTitle(QApplication::translate("SetupSerialDialog", "Dialog", Q_NULLPTR));
        btActualiser->setText(QApplication::translate("SetupSerialDialog", "Actualiser", Q_NULLPTR));
        btConnection->setText(QApplication::translate("SetupSerialDialog", "Connecter", Q_NULLPTR));
        lbbaudRate->setText(QApplication::translate("SetupSerialDialog", "Baud rate :", Q_NULLPTR));
        pbCANCEL->setText(QApplication::translate("SetupSerialDialog", "Annuler", Q_NULLPTR));
        pbOK->setText(QApplication::translate("SetupSerialDialog", "Confirmer", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SetupSerialDialog: public Ui_SetupSerialDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPSERIALDIALOG_H
