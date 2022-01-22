/*
 * Fichier: main.cpp
 * Auteur: Thomas Desrosiers
 * Date: 2021 03 23
 * Desc.: Laboratoire #5 du cours d'intégration de systèmes.
*/

#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    switch (type)
    {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    case QtInfoMsg:
        txt = QString("Info: %1").arg(msg);
        break;
    }
    QString fileName = "log";
    QString formattedTime = QTime::currentTime().toString("hh:mm:ss");
    QString formattedDate = QDate::currentDate().toString("yyyy-mm-dd");
    QFile file(fileName);
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << txt << " -> " << formattedDate << formattedTime << endl; //Affichage de la date et de l'heure lors de la dernière écriture.
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //qInstallMessageHandler(myMessageOutput);
    MainWindow w;
    w.show();
    return a.exec();
}
