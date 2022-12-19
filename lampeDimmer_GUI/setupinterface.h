#ifndef SETUPINTERFACE_H
#define SETUPINTERFACE_H

#include <QDialog>
#include "singleton.hpp"

namespace Ui
{
    class SetupInterface;
}

class SetupInterface : public QDialog, public TSingleton<SetupInterface>
{
    friend class TSingleton<SetupInterface>;
    Q_OBJECT

private:

public:

    struct settingInterface_s
    {
        int displayFormat; //0 -> % | 1 -> 0-255
        int displayFormatMath;

        bool mode; // Manuel/Auto
        bool pompe;
        bool eauChaude;
        bool eauFroide;
        bool debordementGrosBassin;
        bool debordementPetitBassin;

        double valveGrosBassin;
        double valvePetitBassin;
        double valveEauChaude;
        double valveEauFroide;
        double niveauGrosBassin;
        double niveauPetitBassin;
        double temperatureGrosBassin;
        double temperaturePetitBassin;

        double consigneNiveauGrosBassin;
        double consigneNiveauPetitBassin;
        double consigneTemperaturePetitBassin;
    };

    struct partageSettingInterface_s
    {
        settingInterface_s setting;
    };

private:

    partageSettingInterface_s partageSettingInterface;

    Ui::SetupInterface *ui;

public:
    explicit SetupInterface(QWidget *parent = nullptr);
    ~SetupInterface();

    // Partage
    partageSettingInterface_s *getPartageSettingInterface(void);
    void setSettingInterface(settingInterface_s *setting);
    void getSettingInterface(settingInterface_s *setting);

private slots:
    void on_btOk_clicked();

    void on_btAnnuler_clicked();

};

#endif // SETUPINTERFACE_H
