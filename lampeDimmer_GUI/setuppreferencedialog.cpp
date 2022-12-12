#include "setuppreferencedialog.h"
#include "ui_setuppreferencedialog.h"

SetupPreferenceDialog::SetupPreferenceDialog(QDialog *parent) :  QDialog(parent), ui(new Ui::SetupPreferenceDialog)
{
    ui->setupUi(this);
    ui->btOk->setText("Annuler");
    ui->btAnnuler->setText("Annuler");
}

SetupPreferenceDialog::~SetupPreferenceDialog()
{
    delete ui;
}

void SetupPreferenceDialog::on_btAnnuler_clicked()
{

}


void SetupPreferenceDialog::on_btOk_clicked()
{
    this->close();
}

