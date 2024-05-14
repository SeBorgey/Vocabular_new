#include "main_vocabulary.h"
#include <QFile>
#include <QStringList>
#include <QDebug>

Main_vocabulary::Main_vocabulary()
{
    get_all_words();
}

void Main_vocabulary::get_all_words()
{
    QFile file("MyWords.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    int i = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        auto s = line.split('|');
        QString english = QString(s[0]);
        QString russian = QString(s[1]);
        int times = s[2].toInt();
        QDateTime last = QDateTime::fromString(s[3].trimmed(),"yyyy.MM.dd HH:mm:ss");
        Word* word = new Word(english, russian, times, last);
        words.push_back(word);
        english_words[english]=word;
        russian_words[russian]=word;
    }
    file.close();
}


void Main_vocabulary::add_word(QString russian, QString english)
{
    if (english_words.contains(english)){
        if(english_words[english]->russian == russian){
            return;
        }else{
            english_words[english]->russian+=", "+russian;
            english_words[english]->last_learning=QDateTime::fromString("97", "yy");
            russian_words.erase(russian);
            russian_words[english_words[english]->russian]=english_words[english];
            return;
        }
    }
    if (english_words.contains(russian)){
        if(english_words[russian]->english == english){
            return;
        }else{
            english_words[russian]->english+=", "+english;
            english_words[russian]->last_learning=QDateTime::fromString("97", "yy");
            english_words.erase(english);
            english_words[russian_words[russian]->english]=russian_words[russian];
            return;
        }
    }
    Word* word = new Word(english, russian, 0, QDateTime::fromString("97", "yy"));
    words.push_back(word);
    english_words[english]=word;
    russian_words[russian]=word;
    save_words();
}

void Main_vocabulary::save_words()
{
    QFile file("MyWords.txt");
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    for(auto& el: words){
        QByteArray s;
        QByteArray dot = QString("|").toUtf8();
        s+=el->english.toUtf8()+dot;
        s+=el->russian.toUtf8()+dot;
        s+=QByteArray::number(el->learning_times)+dot;
        s+=el->last_learning.toString("yyyy.MM.dd HH:mm:ss").toUtf8();
        file.write(s);
        file.write("\n");
    }
    file.close();


}
