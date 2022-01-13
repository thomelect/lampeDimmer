/**
 * @file    savereadfile.cpp
 * @author  Thomas Desrosiers
 * @date    2022/01/12
 * @brief   Classe qui est utilisée pour la lecture et l'écriture dans un fichier texte.
 */

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
    for (uint8_t index = 0; index < size; index++) //Tant que le nombre du données écrites est inférieur à size...
    {
        out << data[index] << endl;
    }
    out << QDateTime::currentDateTime().toString(); //Affichage de la date et de l'heure lors de la dernière écriture.
    out.flush();
    file.close();
}

void SaveReadFile::readFromfile(QString *dest, uint8_t size)
{
    QFile file(_fileName);
    file.setFileName(_fileName);
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) //Lecture du fichier.
        {
            qDebug() << "Fichier existant!";
            for (uint8_t index = 0; index < size; index++)
            {
                dest[index] = file.readLine();
                dest[index].remove(QRegExp("\\n"));
            }
        }
        file.close();
    }
    else //Sinon (si le fichier n'existe pas)...
    {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Fichier " + _fileName + " créé!";
        }
        file.close();
    }
}
