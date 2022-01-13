/**
 *@file 	savereadfile.h
 *@brief 	Classe qui est utilisée pour la lecture et l'écriture dans un fichier texte.
 *@author 	Thomas Desrosiers
 *@version 	1.0
 *@date 	2022/01/12
*/

#ifndef SAVEREADFILE_H
#define SAVEREADFILE_H

#include <QString>

class SaveReadFile
{
public:
    /**
     * @brief           Constructeur de la classe SaveReadFile
     * @param fileName  Nom et emplacement du fichier de configuration.
     */
    SaveReadFile(QString fileName);
    ~SaveReadFile();

    /**
     * @brief       Fonction utilisée pour sauvegarder des données dans un fichier de configuration.
     * @param data  Donnée à écrire dans le fichier de configuration.
     * @param size  Nombre de donnée à écrire (doit être plus petit ou égal au nombre de données présente dans le tableau data).
     */
    void saveToFile(QString *data, uint8_t size);

    /**
     * @brief       Fonction utilisée pour lire des données d'un fichier de configuration.
     * @param dest  Pointeur vesr le tableau dans lequel les données lut seront écrites.
     * @param size  Nombre de données à lire.
     */
    void readFromfile(QString *dest, uint8_t size);

private:
    /* Déclarations variables: */
    QString _fileName;
    QString *readReturn;
    int sizeSend;
};

#endif // SAVEREADFILE_H
