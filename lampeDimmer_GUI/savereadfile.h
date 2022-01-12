#ifndef SAVEREADFILE_H
#define SAVEREADFILE_H

#include <QString>

class SaveReadFile
{
public:
    SaveReadFile(QString fileName);
    ~SaveReadFile();

    void saveToFile(QString *data, uint8_t size);
    void readFromfile(QString *dest, uint8_t size);
private:
    QString _fileName;
    QString *readReturn;
    int sizeSend;
};

#endif // SAVEREADFILE_H
