#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>

#include "curses.h"

class typeTrainer{
public:
    std::ifstream ifstr;
    std::string str_buffer, str_status;
    
    std::chrono::steady_clock::time_point timer;
    bool end = false, typed = false;
    int pointer = 0;

    typeTrainer(int);
    void input();
    void isEnd();
    void accMenu();
};
