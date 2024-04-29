#include "current_learning.h"

Current_learning::Current_learning(Main_vocabulary* main_vocab):
    main_vocabulary(main_vocab)
{

}

void Current_learning::accept()
{
    current_index++;
    if (current_index>=current_words.size()){
        check_pack();
        choose_words_for_learning();
    }
}

void Current_learning::next()
{
    current_words.push_back(current_words[current_index]);
    current_index++;
}

void Current_learning::check_pack()
{
    for (int i=0; i<std::min(20,static_cast<int>(main_vocabulary->words.size())); i++){
        main_vocabulary->words[i]->learning_times++;
        main_vocabulary->words[i]->last_learning = QDateTime::currentDateTime();
    }
}

std::pair<QString, QString> Current_learning::get_current_word(){
    return current_words[current_index];
}

void Current_learning::choose_words_for_learning()
{
    current_words.clear();
    struct {
        bool operator()(const Word* word1, const Word* word2) const {
            return word1->last_learning < word2->last_learning;
        }
    } comparator;
    sort(main_vocabulary->words.begin(),main_vocabulary->words.end(),comparator);
    for (int i=0; i<std::min(20,static_cast<int>(main_vocabulary->words.size())); i++){
        current_words.push_back({main_vocabulary->words[i]->english, main_vocabulary->words[i]->russian});
    }
    for (int i=0; i<std::min(20,static_cast<int>(main_vocabulary->words.size())); i++){
        current_words.push_back({main_vocabulary->words[i]->russian, main_vocabulary->words[i]->english});
    }
    current_index=0;
}
