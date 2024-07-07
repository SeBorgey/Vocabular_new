#include "mainvocabulary.h"
#include <QFile>
#include <QStringList>
#include <QDebug>

MainVocabulary::MainVocabulary()
{
    getAllWords();
}

void MainVocabulary::getAllWords()
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
        englishWords[english]=word;
        russianWords[russian]=word;
    }
    file.close();
}


void MainVocabulary::addWord(QString russian, QString english)
{
    if (englishWords.contains(english)){
        if(englishWords[english]->russian.contains(russian)){
            return;
        }else{
            englishWords[english]->russian+=", "+russian;
            englishWords[english]->lastLearning=QDateTime::fromString("97", "yy");
            russianWords.erase(russian);
            russianWords[englishWords[english]->russian]=englishWords[english];
            return;
        }
    }
    if (russianWords.contains(russian)){
        if(russianWords[russian]->english.contains(english)){
            return;
        }else{
            russianWords[russian]->english+=", "+english;
            russianWords[russian]->lastLearning=QDateTime::fromString("97", "yy");
            englishWords.erase(english);
            englishWords[russianWords[russian]->english]=russianWords[russian];
            return;
        }
    }
    Word* word = new Word(english, russian, 0, QDateTime::fromString("97", "yy"));
    words.push_back(word);
    englishWords[english]=word;
    russianWords[russian]=word;
    saveWords();
}

void MainVocabulary::saveWords()
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
        s+=QByteArray::number(el->learningTimes)+dot;
        s+=el->lastLearning.toString("yyyy.MM.dd HH:mm:ss").toUtf8();
        file.write(s);
        file.write("\n");
    }
    file.close();
}
void MainVocabulary::deleteWord(int index)
{
    if (index < 0 || index >= words.size()) return;

    Word* word = words[index];
    englishWords.erase(word->english);
    russianWords.erase(word->russian);
    words.erase(words.begin() + index);
    delete word;
    saveWords();
}

void MainVocabulary::updateWord(int index, const QString& english, const QString& russian, int times, const QDateTime& lastLearning)
{
    if (index < 0 || index >= words.size()) return;

    Word* word = words[index];
    englishWords.erase(word->english);
    russianWords.erase(word->russian);

    word->english = english;
    word->russian = russian;
    word->learningTimes = times;
    word->lastLearning = lastLearning;

    englishWords[english] = word;
    russianWords[russian] = word;

    saveWords();
}
void MainVocabulary::importWord(const QString& english, const QString& russian, int times, const QDateTime& lastLearning)
{
    if (english.isEmpty() || russian.isEmpty()) {
        return;
    }

    if (englishWords.contains(english)) {
        Word* existingWord = englishWords[english];
        if (!existingWord->russian.contains(russian)) {
            existingWord->russian += ", " + russian;
            russianWords.erase(existingWord->russian);
            russianWords[existingWord->russian] = existingWord;
        }
        existingWord->learningTimes = times;
        existingWord->lastLearning = lastLearning;
    }
    else if (russianWords.contains(russian)) {
        Word* existingWord = russianWords[russian];
        if (!existingWord->english.contains(english)) {
            existingWord->english += ", " + english;
            englishWords.erase(existingWord->english);
            englishWords[existingWord->english] = existingWord;
        }
        existingWord->learningTimes = times;
        existingWord->lastLearning = lastLearning;
    }
    else {
        Word* word = new Word(english, russian, times, lastLearning);
        words.push_back(word);
        englishWords[english] = word;
        russianWords[russian] = word;
    }
}
