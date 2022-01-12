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

void SaveReadFile::saveToFile(QString *data)
{
    QFile file(_fileName);
    file.setFileName(_fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << data[0] << endl;
    out << data[1];
    out.flush();
    file.close();
//    out << tr("%1").arg(QDateTime::currentDateTime().toString());
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
