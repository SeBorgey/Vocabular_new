#ifndef MAIN_VOCABULARY_H
#define MAIN_VOCABULARY_H
#include <deque>
#include <QDateTime>
struct Word;
class MainVocabulary
{
public:
    MainVocabulary();
    void getAllWords();
    void chooseWordsForLearning();
    void addWord(QString russian, QString english);
    void saveWords();
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
