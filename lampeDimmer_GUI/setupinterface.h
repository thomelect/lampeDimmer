#ifndef SETUPINTERFACE_H
#define SETUPINTERFACE_H

#include <QDialog>

namespace Ui {
class SetupInterface;
}

class SetupInterface : public QDialog
{
    Q_OBJECT
private:

public:
    struct settingInterface_s
    {
        char displayFormat; //0 -> % | 1 -> 0-255
        double displayFormatMath;
    };

    struct partageSettingInterface_s
    {
        settingInterface_s setting;
    };

public:
    explicit SetupInterface(QWidget *parent = nullptr);
    ~SetupInterface();

    void getStruct(settingInterface_s *setting);

private slots:
    void on_btOk_clicked();

    void on_btAnnuler_clicked();

private:

    partageSettingInterface_s partageSettingInterface;

    Ui::SetupInterface *ui;
};

#endif // SETUPINTERFACE_H
