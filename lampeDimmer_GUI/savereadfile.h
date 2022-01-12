#ifndef SAVEREADFILE_H
#define SAVEREADFILE_H

#include <QString>

class SaveReadFile
{
public:
    SaveReadFile(QString fileName);
    ~SaveReadFile();

    void saveToFile(QString data);
    QString readFromfile(QString *dest);
private:
    QString _fileName;
};

#endif // SAVEREADFILE_H
