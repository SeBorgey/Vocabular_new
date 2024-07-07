#include "currentlearning.h"

CurrentLearning::CurrentLearning(MainVocabulary* mainVocab)

{
    mainVocabulary = mainVocab;
}

void CurrentLearning::accept()
{
    currentIndex++;
    if (currentIndex >= currentWords.size()){
        checkPack();
        chooseWordsForLearning();
    }
}

void CurrentLearning::next()
{
    currentWords.push_back(currentWords[currentIndex]);
    currentIndex++;
}

void CurrentLearning::checkPack()
{
    for (int i=0; i<std::min(20,static_cast<int>(mainVocabulary->words.size())); i++){
        mainVocabulary->words[i]->learningTimes++;
        mainVocabulary->words[i]->lastLearning = QDateTime::currentDateTime();
    }
}

std::pair<QString, QString> CurrentLearning::getCurrentWord(){
    return currentWords[currentIndex];
}

void CurrentLearning::chooseWordsForLearning()
{
    currentWords.clear();
    struct {
        bool operator()(const Word* word1, const Word* word2) const {
            return word1->lastLearning < word2->lastLearning;
        }
    } comparator;
    sort(mainVocabulary->words.begin(),mainVocabulary->words.end(),comparator);
    for (int i=0; i<std::min(20,static_cast<int>(mainVocabulary->words.size())); i++){
        currentWords.push_back({mainVocabulary->words[i]->english, mainVocabulary->words[i]->russian});
    }
    for (int i=0; i<std::min(20,static_cast<int>(mainVocabulary->words.size())); i++){
        currentWords.push_back({mainVocabulary->words[i]->russian, mainVocabulary->words[i]->english});
    }
    currentIndex=0;
}
