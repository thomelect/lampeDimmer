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
#include <QSettings>
#include <QDir>

#define MAX_DAY_LOG 0
#define DEBUG_LOG 0
#define WINDOW_NORMAL 0

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
    QString fileName = "log.txt";

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
    int currentExitCode = 0;
    do{
    QApplication a(argc, argv);
    QSettings settingsPref("./preferences.ini", QSettings::IniFormat);
    if (settingsPref.value("Options/option_2").toBool()) //Rester ouvert en arrière plan n'est pas coché.
        a.setQuitOnLastWindowClosed(false); //L'application n'est pas "arrêtée" si la fenêtre est fermée.
    else
        a.setQuitOnLastWindowClosed(true); //L'application n'est pas "arrêtée" si la fenêtre est fermée.

    MainWindow w;
    if (!settingsPref.value("Options/option_1").toBool()) //Ouvrir en arrière plan n'est pas coché.
        w.show(); //Si non utilisé, la fenêtre n'est pas affiché au démarrage de l'application

#if DEBUG_LOG //Si DEBUG_LOG est égal à 0, les débug se font dans la console.
    qInstallMessageHandler(myMessageOutput);
#endif
        currentExitCode = a.exec();
    } while (currentExitCode == MainWindow::EXIT_CODE_REBOOT);
    return currentExitCode;
}
