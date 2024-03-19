#include "current_learning.h"

Current_learning::Current_learning(Main_vocabulary main_vocabulary):
    main_vocabulary(main_vocabulary)
{

}

void Current_learning::accept()
{
    current_index++;
}

void Current_learning::next()
{
    current_words.push_back(current_words[current_index]);
    current_index++;
}

void Current_learning::check_pack()
{
    for (int i=0; i<20; i++){
        main_vocabulary.words[i]->learning_times++;
        main_vocabulary.words[i]->last_learning = QDateTime::currentDateTime();
    }
}

void Current_learning::choose_words_for_learning()
{
    current_words.clear();
    struct {
        bool operator()(const Word* word1, const Word* word2) const {
            return word1->last_learning < word2->last_learning;
        }
    } comparator;
    sort(main_vocabulary.words.begin(),main_vocabulary.words.end(),comparator);
    for (int i=0; i<20; i++){
        current_words.push_back({main_vocabulary.words[i]->english, main_vocabulary.words[i]->russian});
    }
    for (int i=0; i<20; i++){
        current_words.push_back({main_vocabulary.words[i]->russian, main_vocabulary.words[i]->english});
    }
    current_index=0;
}
