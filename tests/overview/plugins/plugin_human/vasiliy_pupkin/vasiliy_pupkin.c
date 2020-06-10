#include "plugins/plugin_human.h"

#include <string.h>
#include <stdint.h>

const void* get_implementation_id () {
    static uint8_t result [] = {
        0x70, 0x45, 0x99, 0xc7,
        0x1e, 0x21,
        0x4d, 0x1c,
        0x94, 0x91,
        0x0a, 0x89, 0xb5, 0x98, 0xa0, 0x97
    };
    return result;
}

const char* get_full_name () {
    return "Vasiliy Pupkin";
}

const char* get_first_name () {
    return "Vasiliy";
}

const char* get_last_name () {
    return "Pupkin";
}

uint8_t get_age () {
    return 30;
}

void work_with_int (int *value) {
    *value = (*value) * 2;
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
