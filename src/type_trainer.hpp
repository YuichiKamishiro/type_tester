#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>

#include "curses.h"
#include "locale.h"

class typingTrainer{
public:
    std::ifstream ifstr;
    std::string str_buffer, str_status;
    std::vector<int> all_cpms;

    std::chrono::steady_clock::time_point timer;
    bool end = false, typed = false;
    int pointer = 0;
    int correct_chars = 0;
    int uncorrect_chars = 0;
    int amount_of_words = 5;

    typingTrainer();
    void init();
    void input();
    void isEnd();
    void accMenu();
    void modeMenu();
};
