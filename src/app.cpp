#include "app.hpp"
#include <curses.h>

app::app(int amount_of_words) : typing_trainer(amount_of_words) {
    
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
    

    while(typing_trainer.end == false){
        typing_trainer.input();
        typing_trainer.isEnd();

        for(int i = 0; i < typing_trainer.str_buffer.size(); ++i) {
            if(typing_trainer.str_status[i] == '1') {
                attron(COLOR_PAIR(2));
                mvaddch(5, 5 + i, std::as_const(typing_trainer.str_buffer[i]));
                attroff(COLOR_PAIR(2));
            } else if(typing_trainer.str_status[i] == '0'){
                attron(COLOR_PAIR(1));
                mvaddch(5, 5 + i, std::as_const(typing_trainer.str_buffer[i]));
                attroff(COLOR_PAIR(1));
            } else {
                mvaddch(5, 5 + i, std::as_const(typing_trainer.str_buffer[i]));
            }
        }
        refresh();
    }
    endwin();
}
