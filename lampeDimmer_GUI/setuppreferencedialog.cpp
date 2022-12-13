#include "setuppreferencedialog.h"
#include "ui_setuppreferencedialog.h"
#include <QDebug>

SetupPreferenceDialog::SetupPreferenceDialog(QDialog *parent) : QDialog(parent), ui(new Ui::SetupPreferenceDialog)
{
    ui->setupUi(this);

    ui->cbList_1->addItem("A");
    ui->cbList_1->addItem("B");
    ui->cbList_1->addItem("C");
    ui->cbList_1->addItem("D");
    ui->cbList_1->addItem("E");
    ui->cbList_1->addItem("F");
    ui->cbList_1->addItem("G");

    ui->cbList_2->addItem("A");
    ui->cbList_2->addItem("B");
    ui->cbList_2->addItem("C");
    ui->cbList_2->addItem("D");
    ui->cbList_2->addItem("E");
    ui->cbList_2->addItem("F");
    ui->cbList_2->addItem("G");
    ui->btAnnuler->setText("Annuler");
    ui->btEnregistre->setText("Enregistrer");
    /*-----------------------------------------------*/
    settingsPref = new QSettings("./preferences.ini", QSettings::IniFormat);
    settingsPref->beginGroup("Options");
    ui->cbOption_1->setChecked(settingsPref->value("option_1").toBool());
    ui->cbOption_2->setChecked(settingsPref->value("option_2").toBool());
    ui->cbOption_3->setChecked(settingsPref->value("option_3").toBool());
    ui->cbOption_4->setChecked(settingsPref->value("option_4").toBool());
    ui->cbList_1->setCurrentIndex(settingsPref->value("optionMulti_1").toInt());
    ui->cbList_2->setCurrentIndex(settingsPref->value("optionMulti_2").toInt());
    settingsPref->endGroup();
}

SetupPreferenceDialog::~SetupPreferenceDialog()
{
    delete ui;
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
        settingsPref->setValue("optionMulti_2", ui->cbList_2->currentIndex());
    settingsPref->endGroup();

    this->close();
}
