#include "setuppreferencedialog.h"
#include "ui_setuppreferencedialog.h"

SetupPreferenceDialog::SetupPreferenceDialog(QDialog *parent) :  QDialog(parent), ui(new Ui::SetupPreferenceDialog)
{
    ui->setupUi(this);
    ui->btAnnuler->setText("Annuler");
    ui->btEnregistre->setText("Enregistrer");
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
    this->close();
}

