#include "plugins/plugin_human.h"

#include <string.h>
#include <stdint.h>

const void* get_implementation_id () {
    static uint8_t result [] = {
        0x0c, 0xf7, 0x58, 0x98,
        0x0a, 0x81,
        0x47, 0xfb,
        0xaf, 0x0e,
        0x93, 0xcc, 0xb9, 0xc8, 0xdc, 0x8c
    };
    return result;
}

const char* get_full_name () {
    return "Ivan Susanin";
}

const char* get_first_name () {
    return "Ivan";
}

const char* get_last_name () {
    return "Susanin";
}

uint8_t get_age () {
    return 45;
}

void work_with_int (int *value) {
    *value = 0;
}


//int sum (int a1, int a2) {
//    return a1 + a2;
//}
//
//char to_upper (char *source) {
//    char *s = source;
//    while (*s) {
//        *s = to_upper(*s);
//    }
//    return '\0';
//}
//
//double div2 (double source) {
//    return source / 2;
//}
