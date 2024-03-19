#ifndef BACKEND_H
#define BACKEND_H
#include <cstring>
#include <iostream>
#include <QObject>
#include "srtparser.h"
using namespace std;
class BackEnd : public QObject
{
    Q_OBJECT
private:
    QString filename;
public:
    explicit BackEnd(QString filename, QObject *parent = nullptr);
    bool readed = false;
    void readSubtitleFile(QString directory);
    QString getSubtitleText(double playTime);
    bool isFileExist(const string &temp);
    std::vector<SubtitleItem*> sub;
signals:
};

#endif // BACKEND_H
