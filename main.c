#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include "big_bool.h"


int main(){
    //test uint64_t input
    uint64_t big_number = 0b0000000000000001000000100000001100000100000001010000011000000111;
    big_bool *ptr = BB_from_uint64_t(big_number);
    BB_output(ptr);
    BB_free(ptr);

    //test logic functions

    big_bool *vec1 = BB_from_string("101");
    big_bool *vec2 = BB_from_string("1010101");
    BB_output(vec1);
    BB_output(vec2);

    big_bool *con = BB_conjunction(vec1, vec2);
    big_bool *dis = BB_disjunction(vec1, vec2);
    big_bool *xor = BB_xor(vec1, vec2);
    big_bool *inv = BB_inverting(vec1);

    printf("Conjunction:\n");
    BB_output(con);
    BB_free(con);
    printf("Disjunction:\n");
    BB_output(dis);
    BB_free(dis);
    printf("XOR:\n");
    BB_output(xor);
    BB_free(xor);
    printf("Inverting:\n");
    BB_output(inv);
    BB_free(inv);

    BB_free(vec2);

    //test shifts
    int num = 16;
    big_bool *rsh = BB_right_shift(vec1, num);
    big_bool *lsh = BB_left_shift(vec1, num);
    big_bool *rcsh = BB_cyclic_right_shift(vec1, num);
    big_bool *lcsh = BB_cyclic_left_shift(vec1, num);
    printf("Right shift:\n");
    BB_output(rsh);
    BB_free(rsh);
    printf("Left shift:\n");
    BB_output(lsh);
    BB_free(lsh);
    printf("Right cyclic shift:\n");
    BB_output(rcsh);
    BB_free(rcsh);
    printf("Left cyclic shift:\n");
    BB_output(lcsh);
    BB_free(lcsh);

    //test string from big bool
    printf("string:\n");
    char *str = string_from_BB(vec1);
    puts(str);
    printf("\n");
    free(str);
    str = NULL;
     BB_free(vec1);

    //test BB_from_string
    char *string = "0001010111";
    big_bool *vec = BB_from_string(string);
    printf("BB_from_string:\n");
    BB_output(vec);
    BB_free(vec);

    //test string input
    char *str2 = string_input();
    puts(str2);
    big_bool *vec3 = BB_from_string(str2);
    free(str2);
    str2 = NULL;
    printf("BB_from_string:\n");
    BB_output(vec3);
    BB_free(vec3);
    return 0;
}