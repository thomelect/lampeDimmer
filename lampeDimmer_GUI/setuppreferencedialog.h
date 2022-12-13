#ifndef SETUPPREFERENCEDIALOG_H
#define SETUPPREFERENCEDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QSettings>

namespace Ui
{
    class SetupPreferenceDialog;
}

class SetupPreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    SetupPreferenceDialog(QDialog *parent = nullptr);
    ~SetupPreferenceDialog();

private slots:
    void on_btAnnuler_clicked();

    void on_btEnregistre_clicked();

    void on_cbOption_4_clicked();

private:

    /*Déclarations objets : */
    QSettings *settingsPref;
    Ui::SetupPreferenceDialog *ui;
    void boutonManage(void);
};

#endif // SETUPPREFERENCEDIALOG_H
