#include "setuppreferencedialog.h"
#include "ui_setuppreferencedialog.h"
#include "mainwindow.h"
#include <QDebug>

SetupPreferenceDialog::SetupPreferenceDialog(QDialog *parent) : QDialog(parent), ui(new Ui::SetupPreferenceDialog)
{
    ui->setupUi(this);

    ui->cbList_1->addItem("OUI");
    ui->cbList_1->addItem("NON");
    ui->cbList_1->addItem("PEUT-ÊTRE");

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    ui->verticalLayout_4->addWidget(line);

    ui->btAnnuler->setText("Annuler");
    ui->btEnregistre->setText("Enregistrer");
    /*-----------------------------------------------*/
    settingsPref = new QSettings("./preferences.ini", QSettings::IniFormat);

    settingsPref->beginGroup("Options");
    ui->cbOption_1->setChecked(settingsPref->value("option_1").toBool());
    ui->cbOption_2->setChecked(settingsPref->value("option_2").toBool());
    ui->cbOption_3->setChecked(settingsPref->value("option_3").toBool());
    ui->cbOption_4->setChecked(settingsPref->value("option_4").toBool());
    settingsPref->endGroup();

    boutonManage();
}

SetupPreferenceDialog::~SetupPreferenceDialog()
{
    delete ui;
}

void SetupPreferenceDialog::boutonManage(void)
{

    if (ui->cbOption_4->isChecked())
    {
        ui->cbList_1->setEnabled(1);
        ui->cbList_1->setCurrentIndex(settingsPref->value("Options/optionMulti_2").toInt());
    }
    else
    {
        ui->cbList_1->setEnabled(0);
        ui->cbList_1->setCurrentIndex(ui->cbList_1->findText("NON", Qt::MatchExactly));
    }

}

void SetupPreferenceDialog::on_btAnnuler_clicked()
{
    this->close();
}

void SetupPreferenceDialog::on_btEnregistre_clicked()
{
    settingsPref->beginGroup("Options");
        settingsPref->setValue("option_1", ui->cbOption_1->isChecked());
        settingsPref->setValue("option_2", ui->cbOption_2->isChecked());
        settingsPref->setValue("option_3", ui->cbOption_3->isChecked());
        settingsPref->setValue("option_4", ui->cbOption_4->isChecked());
        settingsPref->setValue("optionMulti_1", ui->cbList_1->currentIndex());
    settingsPref->endGroup();

    this->close();
}

void SetupPreferenceDialog::on_cbOption_4_clicked()
{
    boutonManage();
}


void SetupPreferenceDialog::on_pbReboot_clicked()
{
    MainWindow::reboot();
}

