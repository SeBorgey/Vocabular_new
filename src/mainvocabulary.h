#ifndef MAIN_VOCABULARY_H
#define MAIN_VOCABULARY_H
#include <deque>
#include <QDateTime>
#include <unordered_map>

struct Word;
class MainVocabulary
{
public:
    MainVocabulary();
    void getAllWords();
    void chooseWordsForLearning();
    void addWord(QString russian, QString english);
    void saveWords();
    void deleteWord(int index);
    void updateWord(int index, const QString& english, const QString& russian, int times, const QDateTime& lastLearning);
    std::deque<Word*> words;
    std::unordered_map<QString, Word*> englishWords;
    std::unordered_map<QString, Word*> russianWords;
};

struct Word{
    QString english;
    QString russian;
    int learningTimes;
    QDateTime lastLearning;
    Word(QString a, QString b, int t, QDateTime time):
        english(a), russian(b), learningTimes(t), lastLearning(time){

    }
};
#endif // MAIN_VOCABULARY_H
