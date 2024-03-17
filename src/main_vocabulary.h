#ifndef MAIN_VOCABULARY_H
#define MAIN_VOCABULARY_H
#include <deque>
#include <QDateTime>
struct Word;
class Main_vocabulary
{
public:
    Main_vocabulary();
    void get_all_words();
    void choose_words_for_learning();
    void add_word(QString russian, QString english);
    void save_words();
    std::deque<Word*> words;
    std::unordered_map<QString, Word*> english_words;
    std::unordered_map<QString, Word*> russian_words;
};
struct Word{
    QString english;
    QString russian;
    int learning_times;
    QDateTime last_learning;
    Word(QString a, QString b, int t, QDateTime time):
    english(a), russian(b), learning_times(t), last_learning(time){

    }
};
#endif // MAIN_VOCABULARY_H
