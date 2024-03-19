#include "backend.h"
using namespace std;
BackEnd::BackEnd(QString filename, QObject *parent) : QObject(parent), filename(filename)
{
    readSubtitleFile(filename);
}

void BackEnd::readSubtitleFile(QString directory)
{
    cout<< "readSubtitleFile:"<< directory.toStdString()<<endl;
    if(!isFileExist(directory.toStdString())) {
        cout<< "file does not exist"<<endl;
        readed=false;
        return;
    }
    SubtitleParserFactory *subParserFactory = new SubtitleParserFactory(directory.toStdString());
    SubtitleParser *parser = subParserFactory->getParser();
    readed = true;
    sub = parser->getSubtitles();
}

QString BackEnd::getSubtitleText(double playTime)
{
    for(SubtitleItem * element : sub) {
        double startTime = element->getStartTime();
        double endTime = element->getEndTime();
        if( (startTime <= playTime) && (playTime <= endTime)) {
            cout<< "getSubtitleText: founded"<< element->getText()<<endl;
            return QString::fromStdString(element->getText());
        }
    }
    cout<< "getSubtitleText: not founded"<< endl;
    return "";
}

bool BackEnd::isFileExist(const string &temp)
{
    if (FILE *file = fopen(temp.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}
