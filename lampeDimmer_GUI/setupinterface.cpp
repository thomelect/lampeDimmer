#include "setupinterface.h"
#include "ui_setupinterface.h"
#include <QDebug>

SetupInterface::SetupInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupInterface)
{
    ui->setupUi(this);
}

SetupInterface::~SetupInterface()
{
    delete ui;
}

void SetupInterface::on_btOk_clicked()
{
    if (ui->rbBrut->isChecked())
    {
        partageSettingInterface.setting.displayFormat = ' ';
        partageSettingInterface.setting.displayFormat = 1.00;
    }
    else
    {
        partageSettingInterface.setting.displayFormat = '%';
        partageSettingInterface.setting.displayFormatMath = 2.55;
    }
    qDebug() << partageSettingInterface.setting.displayFormat;
    qDebug() << partageSettingInterface.setting.displayFormatMath;
    this->close();
}

void SetupInterface::on_btAnnuler_clicked()
{
    this->close();
}

void SetupInterface::getStruct(settingInterface_s *setting)
{
    *setting = partageSettingInterface.setting;
}

