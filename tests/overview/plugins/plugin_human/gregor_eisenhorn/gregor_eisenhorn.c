#include "plugins/plugin_human.h"

#include <string.h>
#include <stdint.h>

const void* get_implementation_id () {
    static uint8_t result [] = {
        0x24, 0x1b, 0xee, 0x6f,
        0x5a, 0x6d,
        0x47, 0xd9,
        0xa9, 0x8e,
        0x95, 0x2e, 0x09, 0x54, 0xc0, 0x37
    };
    return result;
}

const char* get_full_name () {
    return "Gregor Eisenhorn";
}

const char* get_first_name () {
    return "Gregor";
}

const char* get_last_name () {
    return " Eisenhorn";
}

uint8_t get_age () {
    return 42;
}

void work_with_int (int *value) {
    *value = (*value) * 10;
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
