#include "savereadfile.h"
#include <QDebug>
#include <QFile>

SaveReadFile::SaveReadFile()
{

}

void SaveReadFile::saveToFile(QString fileName, QString data)
{

}

QString SaveReadFile::readFromfile(QString fileName)
{
    QString readReturn[2];
    QString filePath = fileName;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug() << "Fichier existant!";
        for (uint8_t index = 0; !file.atEnd(); index++) {
            readReturn[index] = file.readLine();
        }
        qDebug() << "Line #1 " + readReturn[0];
        qDebug() << "Line #2 " + readReturn[1];
    }
    else
    {
        qDebug() << "Fichier "+filePath+" créé!";
    }
    file.close();
    return *readReturn;
}
