#include "savereadfile.h"
#include <QDebug>
#include <QFile>
#include <iostream>
#include <fstream>
#include <QDateTime>

SaveReadFile::SaveReadFile(QString fileName)
{
    _fileName = fileName;
}

SaveReadFile::~SaveReadFile()
{

}

void SaveReadFile::saveToFile(QString *data, uint8_t size)
{
    QFile file(_fileName);
    file.setFileName(_fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (uint8_t index = 0; index < size; index++) {
        out << data[index] << endl;
    }
    out << QDateTime::currentDateTime().toString();
    out.flush();
    file.close();
}

void SaveReadFile::readFromfile(QString *dest, uint8_t size)
{
    QFile file(_fileName);
    file.setFileName(_fileName);
    if (file.exists())
    {
        if(file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            qDebug() << "Fichier existant!";
            for (uint8_t index = 0; index < size; index++) {
                dest[index] = file.readLine();
                dest[index].remove(QRegExp("\\n"));
            }
        }
        file.close();
    }
    else {
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            qDebug() << "Fichier " + _fileName + " créé!";
        }
        file.close();
    }
//    return readReturn;
}
