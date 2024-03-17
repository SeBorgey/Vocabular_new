#ifndef CURRENT_LEARNING_H
#define CURRENT_LEARNING_H
#include <string>
#include "main_vocabulary.h"
struct Word;

class Current_learning
{
public:
    Main_vocabulary main_vocabulary;
    Current_learning(Main_vocabulary main_vocabulary);
    void accept();
    void next();
    void check_pack();
    size_t current_index=0;
    void choose_words_for_learning();
    std::vector<std::pair<QString,QString>> current_words;
};

#endif // CURRENT_LEARNING_H
