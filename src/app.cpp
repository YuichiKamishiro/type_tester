#include "app.hpp"
#include <curses.h>

app::app(){
}

void app::run() {
    // starting colors
    start_color();
    // initialize screen
    initscr();
    // turning off echo
    noecho();
    // no delay for input
    nodelay(stdscr, TRUE);
    // invisible cursor
    curs_set(0);
    // for KEY_UP,DOWN
    keypad(stdscr, TRUE);
    // for unicode
    setlocale(LC_ALL, ""); 
    // 8 it's gray
    init_pair(1, COLOR_RED, 8);
    // color pair for correct char
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
    // menu for chosing mode
    typing_trainer.modeMenu();

    while(typing_trainer.end == false){
        // input
        typing_trainer.input();
        // checking last char
        typing_trainer.isEnd();
        
        // render
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
        // refreshing current screen
        refresh();
    }
    // close current screen
    endwin();
}
