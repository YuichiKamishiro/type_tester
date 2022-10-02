#include "app.hpp"
#include <curses.h>

app::app(int amount_of_words) : typeTr(amount_of_words) {
    
}

void app::run() {
    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    

    while(typeTr.end == false){
        typeTr.input();
        typeTr.isEnd();

        for(int i = 0; i < typeTr.str_buffer.size(); ++i) {
            if(typeTr.str_status[i] == '1') {
                attron(COLOR_PAIR(2));
                mvaddch(5, 5 + i, std::as_const(typeTr.str_buffer[i]));
                attroff(COLOR_PAIR(2));
            } else if(typeTr.str_status[i] == '0'){
                attron(COLOR_PAIR(1));
                mvaddch(5, 5 + i, std::as_const(typeTr.str_buffer[i]));
                attroff(COLOR_PAIR(1));
            } else {
                mvaddch(5, 5 + i, std::as_const(typeTr.str_buffer[i]));
            }
        }
        refresh();
    }

    endwin();
}
