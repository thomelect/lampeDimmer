/*
 * Fichier: main.cpp
 * Auteur: Thomas Desrosiers
 * Date: 2022 01 22
 * Desc.: Projet personnel de contrôleur de lampe.
*/

#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QDir>

#define MAX_DAY_LOG 0
#define DEBUG_LOG 0

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
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
    QString formattedDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); //Date et heure actuelle dans le format: AAAA-MM-JJ HH:MM:SS.
    QString fileName = "log";

    QFileInfo fileInfo(fileName); //Récupération des infos du fichier.
    QFile file(fileName);

    if (fileInfo.birthTime().date().daysTo(QDate::currentDate()) > MAX_DAY_LOG) //Si la différence entre la date de création et la date actuelle est suppérieure à MAX_DAY_LOG...
        file.remove();                                                        //Le fichier est supprimé.
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append); //Le fichier est ouvert. Si le fichier n'existe pas, il est créé.
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << txt << " -> " << formattedDateTime << endl; //Affichage de la donnée suivi de la date et de l'heure.
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
#if DEBUG_LOG //Si DEBUG_LOG est égal à 0, les débug se font dans la console.
    qInstallMessageHandler(myMessageOutput);
#endif
    return a.exec();
}
