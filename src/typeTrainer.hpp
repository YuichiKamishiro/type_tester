#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>

#include "curses.h"

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

    typingTrainer(int);
    void find_helper_func(std::vector<int> &);
    void input();
    void isEnd();
    void accMenu();
};
