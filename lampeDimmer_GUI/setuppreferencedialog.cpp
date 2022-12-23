#include "setuppreferencedialog.h"
#include "ui_setuppreferencedialog.h"
#include "mainwindow.h"
#include <QDebug>

SetupPreferenceDialog::SetupPreferenceDialog(QDialog *parent) : QDialog(parent), ui(new Ui::SetupPreferenceDialog)
{
    needReboot = false;
    ui->setupUi(this);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    ui->verticalLayout_4->addWidget(line);

    ui->pbReboot->setVisible(needReboot);
    ui->btAnnuler->setText("Annuler");
    ui->btEnregistre->setText("Enregistrer");
    /*-----------------------------------------------*/
    settingsPref = new QSettings("Thomas Desrosiers", "Lampe Dimmer\\Preferences\\Options");
    ui->cbOption_1->setChecked(settingsPref->value("option_1").toBool());
    ui->cbOption_2->setChecked(settingsPref->value("option_2").toBool());
    ui->cbOption_3->setChecked(settingsPref->value("option_3").toBool());
}

SetupPreferenceDialog::~SetupPreferenceDialog()
{
    delete ui;
}

void SetupPreferenceDialog::setDemarrageAuto(bool demarrageAuto)
{
    QString key = "Lampe Dimmer";
    QSettings registrySettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    registrySettings.remove(key);

    if (demarrageAuto)
    {
        registrySettings.setValue(key, QString("\"" + windowsAppPath() + "\""));
    }
    else
        registrySettings.remove(key);

    registrySettings.sync();
}

QString SetupPreferenceDialog::windowsAppPath()
{
    return QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
}

void SetupPreferenceDialog::on_btAnnuler_clicked()
{
    this->close();
}

void SetupPreferenceDialog::on_btEnregistre_clicked()
{
    settingsPref->setValue("option_1", ui->cbOption_1->isChecked());
    settingsPref->setValue("option_2", ui->cbOption_2->isChecked());
    settingsPref->setValue("option_3", ui->cbOption_3->isChecked());

    if (!needReboot)
    {
        this->close();
    }
    else
    {
        ui->pbReboot->setVisible(needReboot);
    }
    setDemarrageAuto(ui->cbOption_3->isChecked());
}

void SetupPreferenceDialog::on_pbReboot_clicked()
{
    MainWindow::reboot();
}

void SetupPreferenceDialog::on_cbOption_2_clicked()
{
    if (settingsPref->value("option_2").toBool() != ui->cbOption_2->isChecked())
        needReboot = true;
    else
        needReboot = false;
}
