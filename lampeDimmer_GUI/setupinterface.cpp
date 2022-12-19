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
    partageSettingInterface.setting.displayFormat = ui->rbBrut->isChecked();
    if (ui->rbBrut->isChecked())
        partageSettingInterface.setting.displayFormatMath = 1.00;
    else
        partageSettingInterface.setting.displayFormatMath = 2.55;



    this->close();
}

void SetupInterface::on_btAnnuler_clicked()
{
    this->close();
}


SetupInterface::partageSettingInterface_s *SetupInterface::getPartageSettingInterface()
{
    partageSettingInterface_s *result = NULL;
    result = &partageSettingInterface;
    return result;
}

void SetupInterface::setSettingInterface(settingInterface_s *setting)
{
    partageSettingInterface.setting = *setting;
}

void SetupInterface::getSettingInterface(settingInterface_s *setting)
{
    *setting = partageSettingInterface.setting;
}

