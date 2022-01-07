/********************************************************************************
** Form generated from reading UI file 'setupinterface.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPINTERFACE_H
#define UI_SETUPINTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupInterface
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_6;
    QRadioButton *radioButton_7;
    QRadioButton *rbBrut;
    QRadioButton *rbPourcentage;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *btAnnuler;
    QPushButton *btOk;

    void setupUi(QDialog *SetupInterface)
    {
        if (SetupInterface->objectName().isEmpty())
            SetupInterface->setObjectName(QStringLiteral("SetupInterface"));
        SetupInterface->resize(629, 300);
        layoutWidget = new QWidget(SetupInterface);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 20, 541, 221));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        radioButton_3 = new QRadioButton(layoutWidget);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));

        gridLayout->addWidget(radioButton_3, 1, 1, 1, 1);

        radioButton_6 = new QRadioButton(layoutWidget);
        radioButton_6->setObjectName(QStringLiteral("radioButton_6"));

        gridLayout->addWidget(radioButton_6, 2, 2, 1, 1);

        radioButton_7 = new QRadioButton(layoutWidget);
        radioButton_7->setObjectName(QStringLiteral("radioButton_7"));

        gridLayout->addWidget(radioButton_7, 3, 1, 1, 1);

        rbBrut = new QRadioButton(layoutWidget);
        rbBrut->setObjectName(QStringLiteral("rbBrut"));

        gridLayout->addWidget(rbBrut, 0, 2, 1, 1);

        rbPourcentage = new QRadioButton(layoutWidget);
        rbPourcentage->setObjectName(QStringLiteral("rbPourcentage"));

        gridLayout->addWidget(rbPourcentage, 0, 1, 1, 1);

        radioButton_4 = new QRadioButton(layoutWidget);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));

        gridLayout->addWidget(radioButton_4, 1, 2, 1, 1);

        radioButton_5 = new QRadioButton(layoutWidget);
        radioButton_5->setObjectName(QStringLiteral("radioButton_5"));
        radioButton_5->setMinimumSize(QSize(89, 0));

        gridLayout->addWidget(radioButton_5, 2, 1, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        btAnnuler = new QPushButton(SetupInterface);
        btAnnuler->setObjectName(QStringLiteral("btAnnuler"));
        btAnnuler->setGeometry(QRect(440, 260, 80, 25));
        btOk = new QPushButton(SetupInterface);
        btOk->setObjectName(QStringLiteral("btOk"));
        btOk->setGeometry(QRect(530, 260, 80, 25));

        retranslateUi(SetupInterface);

        QMetaObject::connectSlotsByName(SetupInterface);
    } // setupUi

    void retranslateUi(QDialog *SetupInterface)
    {
        SetupInterface->setWindowTitle(QApplication::translate("SetupInterface", "Dialog", Q_NULLPTR));
        radioButton_3->setText(QApplication::translate("SetupInterface", "RadioButton", Q_NULLPTR));
        radioButton_6->setText(QApplication::translate("SetupInterface", "RadioButton", Q_NULLPTR));
        radioButton_7->setText(QApplication::translate("SetupInterface", "RadioButton", Q_NULLPTR));
        rbBrut->setText(QApplication::translate("SetupInterface", "Valeur brut (0 - 255)", Q_NULLPTR));
        rbPourcentage->setText(QApplication::translate("SetupInterface", "Pourcentage (0 - 100%)", Q_NULLPTR));
        radioButton_4->setText(QApplication::translate("SetupInterface", "RadioButton", Q_NULLPTR));
        radioButton_5->setText(QApplication::translate("SetupInterface", "RadioButton", Q_NULLPTR));
        label->setText(QApplication::translate("SetupInterface", "Format d'affichage", Q_NULLPTR));
        label_2->setText(QApplication::translate("SetupInterface", "TextLabel", Q_NULLPTR));
        label_3->setText(QApplication::translate("SetupInterface", "TextLabel", Q_NULLPTR));
        label_4->setText(QApplication::translate("SetupInterface", "TextLabel", Q_NULLPTR));
        btAnnuler->setText(QApplication::translate("SetupInterface", "Annuler", Q_NULLPTR));
        btOk->setText(QApplication::translate("SetupInterface", "Confirmer", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SetupInterface: public Ui_SetupInterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPINTERFACE_H
