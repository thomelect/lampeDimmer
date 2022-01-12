#include "savereadfile.h"
#include <QDebug>
#include <QFile>
#include <iostream>
#include <fstream>

SaveReadFile::SaveReadFile(QString fileName)
{
    _fileName = fileName;
}

SaveReadFile::~SaveReadFile()
{

}

void SaveReadFile::saveToFile(QString data)
{

}

QString SaveReadFile::readFromfile(QString *dest)
{
    QString readReturn[2];
    QFile file(_fileName);
    file.setFileName(_fileName);
    if (file.exists())
    {
        if(file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            qDebug() << "Fichier existant!";
            for (uint8_t index = 0; !file.atEnd(); index++) {
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
    return *readReturn;
}
