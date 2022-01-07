#ifndef SETUPINTERFACE_H
#define SETUPINTERFACE_H

#include <QDialog>

namespace Ui {
class setupinterface;
}

class setupinterface : public QDialog
{
    Q_OBJECT

public:
    explicit setupinterface(QWidget *parent = nullptr);
    ~setupinterface();

private:
    Ui::setupinterface *ui;
};

#endif // SETUPINTERFACE_H
