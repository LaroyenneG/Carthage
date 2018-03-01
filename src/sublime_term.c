//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "sublime_term.h"


#define noir "\e[0;30m"
#define gris "\e[1;30m"
#define rougefonce "\e[0;31m"
#define rose "\e[1;31m"
#define vertfonce "\e[0;32m"
#define vertclair "\e[1;32m"
#define orange "\e[0;33m"
#define jaune "\e[1;33m"
#define bleufonce "\e[0;34m"
#define bleuclair "\e[1;34m"
#define violetfonce "\e[0;35m"
#define violetclair "\e[1;35m"
#define cyanfonce "\e[0;36m"
#define cyanclair "\e[1;36m"
#define grisclair "\e[0;37m"
#define blanc "\e[1;37m"
#define neutre "\e[0;m"

void color_term(enum color c) {

    switch (c) {

        case RED:
            printf(rougefonce);
            break;

        case BLUE:
            printf(bleufonce);
            break;

        case BLACK:
            printf(noir);
            break;

        case GREY:
            printf(gris);
            break;

        case YELLOW:
            printf(jaune);
            break;

        case WHITE:
            printf(blanc);
            break;

        case GREEN:
            printf(vertclair);
            break;

        case ORANGE:
            printf(orange);
            break;

        default:
            printf(neutre);
            break;
    }
}


void clear_term() {

    printf("\033[H\033[2J");
}