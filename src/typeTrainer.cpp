#include "typeTrainer.hpp"
#include <algorithm>
#include <chrono>

typeTrainer::typeTrainer(int amount_of_words) {
    ifstr.open("../words.txt");
    if(!ifstr.is_open()) {std::cout << "Can't open file!\n";}
    
    std::vector<std::string> all_words;
    std::string buffer;
    while(getline(ifstr, buffer)) {
        all_words.push_back(buffer);
    }

    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(all_words.begin(), all_words.end(), g);
 
    for(int i = 0; i < amount_of_words; ++i) {
        str_buffer += all_words[i];
        if(i + 1 < amount_of_words) str_buffer += ' ';
    }

    for(int i = 0; i < str_buffer.size(); ++i) {
        str_status += '2';
    }
}

void typeTrainer::input() {
    int c = getch();
    
    if(c != -1 && !typed) {
        typed = true;
        timer = std::chrono::steady_clock::now();
    }

    if(c == KEY_BACKSPACE) {
        if(str_status[pointer] == '2') {
            str_status[pointer - 1] = '2';
            --pointer;
        } else{
            str_status[pointer] = '2';
            --pointer;
        }
    } else if(str_buffer[pointer] == c && c != -1){
        str_status[pointer] = '1';
        ++pointer;
    } else if(str_buffer[pointer] != c && c != -1){
        str_status[pointer] = '0';
        ++pointer;
    }
    if(pointer == str_buffer.size()) pointer -= 1;
}

void typeTrainer::accMenu() {
    using namespace std::chrono;

    clear();
    
    steady_clock::time_point time_end = steady_clock::now();
    int sec = duration_cast<seconds>(time_end - timer).count();

    int correct_char = 0;
    for(char c : str_status) {
        if(c == '1') ++correct_char;
    }

    int cpm = correct_char * (60 / sec);
    int wpm = cpm / 5;
    

    mvprintw(5, 5, "cpm = %d wpm = %d", cpm, wpm);
    
    refresh();
    timeout(-1);
    getch();
    end = true;
}

void typeTrainer::isEnd() {
    if(pointer == str_buffer.size() - 1 && 
       str_status[str_status.size() - 1] == '1') {accMenu();};
}
