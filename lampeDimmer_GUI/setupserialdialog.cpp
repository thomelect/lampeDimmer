/**
 * @file       setupserialdialog.h
 * @brief      Classe qui affiche une boite de dialogue pour configurer le port série
 * @author     Base : Marc Juneau
 * @author     Adaptation : Thomas Desrosiers
 * @version    0.01
 * @date       8 février 2019
 */

#include "setupserialdialog.h"
#include "ui_setupserialdialog.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

SetupSerialDialog::SetupSerialDialog(QSerialPort *s, void *shared) : QDialog(0),
                                                       ui(new Ui::SetupSerialDialog)
{
    connectInfo = new QString[3];
    saveRead = (SaveReadFile *)shared;
    saveRead->readFromfile(connectInfo, 3);

    serial = s;
    ui->setupUi(this);
    ui->cbListBaudRate->addItem("9600");
    ui->cbListBaudRate->addItem("19200");
    ui->cbListBaudRate->addItem("115200");
    ui->cbListBaudRate->addItem("250000");
    ui->cbListBaudRate->addItem("921600");
    ui->cbListBaudRate->addItem("1000000");
    ui->cbListBaudRate->addItem("2000000");
    if (ui->cbListBaudRate->findText(connectInfo[0], Qt::MatchExactly) != -1)
    {
        ui->cbListBaudRate->setCurrentIndex(ui->cbListBaudRate->findText(connectInfo[0], Qt::MatchExactly));
    }
    else
    {
        ui->cbListBaudRate->setCurrentIndex(0);
        qDebug() << ui->cbListPortSerie->currentText();
    }
    if (serial->isOpen())
    {
        ui->btActualiser->setText("Déconnecter");
        ui->cbListPortSerie->addItem(serial->portName());
        ui->cbListPortSerie->setEnabled(false);
        ui->btConnexion->setEnabled(false);
    }
    else
        ui->btOk->setEnabled(false);
}

SetupSerialDialog::~SetupSerialDialog()
{
    delete ui;
}

void SetupSerialDialog::on_btActualiser_clicked()
{
    ui->cbListPortSerie->clear();
    ui->btActualiser->setText("Actualiser");
    ui->btOk->setEnabled(false);
    serial->close();
    ui->cbListPortSerie->setEnabled(true);
    ui->btConnexion->setEnabled(true);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->cbListPortSerie->addItem(info.portName() + " " + info.description());
        if (info.serialNumber() == connectInfo[2])
        {
            connectInfoCom = info.portName();
        }
    }
    if (ui->cbListPortSerie->findText(connectInfoCom, Qt::MatchExactly) != -1)
    {
        ui->cbListPortSerie->setCurrentIndex(ui->cbListPortSerie->findText(connectInfoCom, Qt::MatchExactly));
    }
    else
    {
        ui->cbListPortSerie->setCurrentIndex(0);
        qDebug() << ui->cbListPortSerie->currentText();
    }
}

void SetupSerialDialog::on_btAnnuler_clicked()
{
    this->close();
}

void SetupSerialDialog::on_btConnexion_clicked()
{
    ui->btOk->setEnabled(true);
    if (ui->cbListPortSerie->count() > 0)
    {
        QString portName = ui->cbListPortSerie->itemText(ui->cbListPortSerie->currentIndex());
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            QString infoTag = info.portName() + " " + info.description();
            if (infoTag == portName)
            {
                serial->setPort(info);
                qDebug() << "try  " << portName;
                if (serial->open(QIODevice::ReadWrite))
                {
                    qDebug() << "open " << portName;
                    if (serial->setBaudRate(BAUD_RATE[ui->cbListBaudRate->currentIndex()]) && serial->setDataBits(QSerialPort::Data8) && serial->setParity(QSerialPort::NoParity) && serial->setStopBits(QSerialPort::OneStop) && serial->setFlowControl(QSerialPort::NoFlowControl))
                    {
                        ui->btConnexion->setEnabled(false);
                        ui->btActualiser->setText("Déconnecter");
                        connectInfo[0] = QString::number(BAUD_RATE[ui->cbListBaudRate->currentIndex()]);
                        connectInfo[1] = info.description();
                        connectInfo[2] = info.serialNumber();
                        saveRead->saveToFile(connectInfo, 3);
                    }
                }
            }
        }
    }
}

void SetupSerialDialog::on_btOk_clicked()
{
    this->close();
}
