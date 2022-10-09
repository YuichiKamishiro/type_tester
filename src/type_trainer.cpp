#include "type_trainer.hpp"
#include <chrono>
#include <curses.h>

using namespace std::chrono;

typingTrainer::typingTrainer() {
}

void typingTrainer::init() {
    ifstr.open("words.txt");
    // handling errors
    if(!ifstr.is_open()) {std::cout << "Can't open file!\n";}
    
    std::vector<std::string> all_words;
    std::string buffer;
    // registrating all words
    while(getline(ifstr, buffer)) {
        all_words.push_back(buffer);
    }

    // random device for random
    std::random_device rd;
    std::mt19937 g(rd());
 
    // shuffling all words
    std::shuffle(all_words.begin(), all_words.end(), g);
 
    // adding spaces to words
    for(int i = 0; i < amount_of_words; ++i) {
        str_buffer += all_words[i];
        if(i + 1 < amount_of_words) str_buffer += ' ';
    }

    // filling status buffer as unentered 
    for(int i = 0; i < str_buffer.size(); ++i) {
        str_status += '2';
    }

}

void typingTrainer::input() {
    // handling user input
    int c = getch();

    // checking first input
    if(c != -1 && !typed) {
        typed = true;
        timer = steady_clock::now();
    }
    // handling backspace
    if(c == KEY_BACKSPACE) {
        if(str_status[pointer] == '2') {
            if(str_status[pointer - 1] == '1') --correct_chars;
            else if(str_status[pointer - 1] == '0') --uncorrect_chars;
            str_status[pointer - 1] = '2';
            --pointer;
        } else{
            if(str_status[pointer - 1] == '1') --correct_chars;
            else if(str_status[pointer - 1] == '0') --uncorrect_chars;
            str_status[pointer] = '2';
            --pointer;
        }
    } else if(str_buffer[pointer] == c && c != -1){
        str_status[pointer] = '1';
        ++pointer;
        ++correct_chars;
    } else if(str_buffer[pointer] != c && c != -1){
        str_status[pointer] = '0';
        ++pointer;
        ++uncorrect_chars;
    }
    // checking pointer
    if(pointer == str_buffer.size()) pointer -= 1;

    // registrating cpm
    if(c != -1) {
        steady_clock::time_point time_end = steady_clock::now();
        float millis = duration_cast<milliseconds>(time_end - timer).count();

        int cpm = (correct_chars - uncorrect_chars) * (60000 / millis);
        
        if(cpm > 0) all_cpms.push_back(cpm);
    }
}

void typingTrainer::accMenu() {
    clear();
    
    // final cpm
    steady_clock::time_point time_end = steady_clock::now();
    int millis = duration_cast<milliseconds>(time_end - timer).count();

    int cpm = (correct_chars - uncorrect_chars) * (60000 / millis);
    if(cpm < 0) cpm = 0;

    int wpm = cpm / 5;
    
    // generate graph
    std::vector<int> cpms_for_graph;
    
    std::vector<int> unique_all_cpms;

    for(int i = 0; i < all_cpms.size(); ++i) if(std::find(unique_all_cpms.begin(), unique_all_cpms.end(), all_cpms[i]) == unique_all_cpms.end()) unique_all_cpms.push_back(all_cpms[i]);
    
    double cf = 10.f / (unique_all_cpms.size());
    double isStep = cf;

    if(unique_all_cpms.size() < 10) mvprintw(17, 0, "error: too little information");
    
    for(int i = 0; i < unique_all_cpms.size(); ++i) {
        isStep += cf;
        if(isStep >= 1.) {
            isStep -= 1.;
            cpms_for_graph.push_back(unique_all_cpms[i]);
        }
    }

    std::vector<int> sorted_cpms_for_graph = cpms_for_graph;
    std::sort(sorted_cpms_for_graph.begin(), sorted_cpms_for_graph.end());
    std::vector<std::pair<int, int>> graph_vector;

    for(int i = 0; i < cpms_for_graph.size(); ++i) {
    for(int j = 0; j < sorted_cpms_for_graph.size(); ++j) {
        if(sorted_cpms_for_graph[j] == cpms_for_graph[i]) graph_vector.push_back({cpms_for_graph[i], j + 1});    
    }}

    // draw cpm wpm
    mvprintw(0, 0, "cpm = %d wpm = %d", cpm, wpm);
    
    std::reverse(sorted_cpms_for_graph.begin(), sorted_cpms_for_graph.end());
    for(int i = 0;i < sorted_cpms_for_graph.size(); ++i) {
        mvprintw(2 + i + 10 - sorted_cpms_for_graph.size(), 0, "%d", sorted_cpms_for_graph[i]);
    }

    init_pair(3, COLOR_BLACK, COLOR_GREEN);

    // draw graph
    for(int i = 0; i < graph_vector.size(); ++i) {
    for(int j = 0; j < graph_vector[i].second; ++j) {
        attron(COLOR_PAIR(3));
        mvaddwstr(11 - j, i * 2 + 4, L"\u3164");
        attroff(COLOR_PAIR(3));
    }}
    
    mvprintw(12, 4, "1 2 3 4 5 6 7 8 9 10");
    
    refresh();
    timeout(-1);
    mvprintw(13, 0, "Please press any key to quit");
    refresh();
    getch();
    end = true;
}

void typingTrainer::modeMenu() {
    WINDOW *win = newwin(5, 50, 0, 0);
    keypad(win, TRUE);

    const int SIZE = 3;
    std::string choices[SIZE] {"5", "10", "15"};

    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    
    bool entered = false;

    int highlight_ptr = 0;

    while(!entered) {
        // clearing screen without refreshing
        werase(win);
        // box
        box(win, 0, 0);
        // text 
        mvwaddwstr(win, 0, 1, L"Mode selection[words]");
    
        // drawing our menu
        for(int i = 0; i < SIZE; ++i) {
            if(highlight_ptr == i) {
                wattron(win, COLOR_PAIR(4));
                mvwprintw(win, 1 + i, 1, choices[i].c_str());
                wattroff(win, COLOR_PAIR(4));
            } else {
                mvwprintw(win, 1 + i, 1, choices[i].c_str());
            }
        }
        
        if(highlight_ptr > SIZE - 1) highlight_ptr = SIZE - 1;
        else if(highlight_ptr < 0) highlight_ptr = 0;

        int c = getch();

        switch(c) {
            case KEY_UP: --highlight_ptr; break;
            case KEY_DOWN: ++highlight_ptr; break;
            // KEY_ENTER doen't work so we use 10
            case 10: amount_of_words = stoi(choices[highlight_ptr]); init(); entered = true; break;
        }

        wrefresh(win);
    }
    clear();
}

void typingTrainer::isEnd() {
    // checking last symbol
    if(pointer == str_buffer.size() - 1 && 
       str_status[str_status.size() - 1] == '1') {accMenu();};
}
