//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#ifndef LIBRARY_SUBLIME_TERM_H
#define LIBRARY_SUBLIME_TERM_H


enum color {
    RED,
    BLUE,
    BLACK,
    GREY,
    YELLOW,
    WHITE,
    GREEN,
    ORANGE,
    DEFAULT
};


extern void color_term(enum color c);

extern void clear_term();

#endif //LIBRARY_SUBLIME_TERM_H
