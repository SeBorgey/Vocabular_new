#ifndef CURRENT_LEARNING_H
#define CURRENT_LEARNING_H
#include <string>
#include "mainvocabulary.h"
struct Word;

class CurrentLearning
{
public:
    MainVocabulary* mainVocabulary;
    CurrentLearning(MainVocabulary* mainVocab);
    void accept();
    void next();
    void checkPack();
    size_t currentIndex=0;
    void chooseWordsForLearning();
    std::vector<std::pair<QString,QString>> currentWords;
    std::pair<QString, QString> getCurrentWord();
};

#endif // CURRENT_LEARNING_H
