#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include "big_bool.h"
#include <stdlib.h>


int bb_errno = ERR_OK;

void BB_free(big_bool *bb){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return;
    }
    free(bb->parts);
    bb->parts = NULL;
    free(bb);
    bb = NULL;
}

big_bool *copy_vector(big_bool *bb){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }
    big_bool *twin_vector = (big_bool *)calloc(1, sizeof(big_bool));

    if(twin_vector == NULL){
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    twin_vector->parts = (uint8_t *)calloc(bb->last_byte + 1, sizeof(uint8_t));

    if(twin_vector->parts == NULL){
        bb_errno = ERR_MEM_NOT_ALLOC;
        free(twin_vector);
        return NULL;
    }
    twin_vector->last_bit = bb->last_bit;
    twin_vector->last_byte = bb->last_byte;
    for(int i = 0; i <= bb->last_byte; i++){
        twin_vector->parts[i] = bb->parts[i];
    }
    return twin_vector;
}

void BB_output(big_bool *bb){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return;
    }
    int i = 0, j = 0;
    int bit;
    while((i != bb->last_byte) || (j != bb->last_bit)){
        bit = (((bb->parts[i] & (1 << (8 - j - 1)))) > 0)? 1 : 0;
        j++;
        j = j % 8;
        if(j == 0){
            i++;
        }
        printf("%d", bit);
    }
    printf("\n");
}

int BB_lenght(big_bool *vector){
    if(vector == NULL){
        bb_errno = ERR_NULL_INPUT;
        return -1;
    }
    if(vector == NULL)return -1;
    return vector->last_byte*8 + vector->last_bit;
}

big_bool *BB_right_shift(big_bool *bb, int number){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    if(number <  0){
        bb_errno = ERR_SHIFT_EQUAL_OR_LESS_ZERO;
        return NULL;
    }
    int i = number/8;
    int j = 0;
    int ost = number%8;
    int mask;
    big_bool *vector = copy_vector(bb);
    while((bb->last_byte - j >= 0) && (bb->last_byte - i >= 0)){
        if(bb->last_byte - i -1 < 0){
            mask = 0;
        } else {
        mask = (((1 << ost) - 1) & bb->parts[bb->last_byte - i - 1]);}
        mask = (mask << (8 - (ost)));
        mask |= ((((2<<8) - 1) & bb->parts[bb->last_byte - i]) >> ost);
        i++;
        vector->parts[bb->last_byte - j] = mask;
        j++;
    }
    for(i = 0; i < number/8; i++){
        vector->parts[i] = 0;
    }
    return vector;
}

big_bool *BB_left_shift(big_bool *bb, int number){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    if(number <  0){
        bb_errno = ERR_SHIFT_EQUAL_OR_LESS_ZERO;
        return NULL;
    }
    int ost = number % 8;
    int i;
    big_bool *vector = copy_vector(bb);
    for(i = 0; i <= bb->last_byte - number/8; i++){                                 // making two parts of mask from two bytes
        int bytes = number/8 + i;                                                   // because in general we have number % 8 != 0
        if(number/8 +i +1 <= bb->last_byte){                                        // This is hard to take it in mind
            bytes = number/8 + i;                                                   // I lost 6 hours making this function and next
            vector->parts[i] =((((1 << (8-ost))-1) & bb->parts[bytes])<< ost);      // day found a bug. I don't want to touch it
            vector->parts[i] |= (((((1 << 8) -1) << (8-ost)) &                      // while it works.
                    bb->parts[bytes + 1]) >>(8-ost));
        }else{vector->parts[i] =(((1<<(8-ost))-1)& bb->parts[bytes])<< ost;}
    }
    vector->parts[i - 1] &= (((1<<8) - 1) - ((1 << (8 - bb->last_bit)) - 1));
    while(i <= bb->last_byte){
        vector->parts[i]=0;
        i++;
    }
    
    return vector;
}

big_bool *BB_cyclic_right_shift(big_bool *bb, int number){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }
    if(number <  0){
        bb_errno = ERR_SHIFT_EQUAL_OR_LESS_ZERO;
        return NULL;
    }
    number %= BB_lenght(bb);
    big_bool *bb3 = BB_right_shift(bb, number);
    big_bool *bb4 = BB_left_shift(bb, BB_lenght(bb) - number);
    for(int i = 0; i <= bb->last_byte; i++){
        bb3->parts[i] |= bb4->parts[i];
    }
    BB_free(bb4);
    return bb3;
}
big_bool *BB_cyclic_left_shift(big_bool *bb, int number){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    if(number <  0){
        bb_errno = ERR_SHIFT_EQUAL_OR_LESS_ZERO;
        return NULL;
    }
    number %= BB_lenght(bb);
    big_bool *bb3 = BB_left_shift(bb, number);
    big_bool *bb4 = BB_right_shift(bb, BB_lenght(bb) - number);
    for(int i = 0; i <= bb->last_byte; i++){
        bb3->parts[i] += bb4->parts[i];
    }
    BB_free(bb4);
    return(bb3);
}
big_bool *BB_make_equal_size(big_bool *big_vec, big_bool *less_vec){
    if(big_vec == NULL || less_vec == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    big_bool *vector = copy_vector(less_vec);
    vector->parts = realloc(vector->parts, (big_vec->last_byte + 1)*sizeof(uint8_t));
    if(vector->parts == NULL){
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    vector->last_byte = big_vec->last_byte;
    vector->last_bit = big_vec->last_bit;
    big_bool *bb = BB_right_shift(vector, BB_lenght(big_vec) - BB_lenght(less_vec));
    BB_free(vector);
    return bb;
}

big_bool *BB_conjunction(big_bool *bb1, big_bool *bb2){
    if(bb1 == NULL || bb2 == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }


    if(BB_lenght(bb1) < BB_lenght(bb2)){
        big_bool *tmp = bb1;
        bb1 = bb2;
        bb2 = tmp;
    }
    big_bool *bb = BB_make_equal_size(bb1, bb2);
    for(int i = 0; i <= bb->last_byte; i++){
        bb->parts[i] = bb->parts[i] & bb1->parts[i];
    }
    return bb;
}

big_bool *BB_disjunction(big_bool *bb1, big_bool *bb2){
    if(bb1 == NULL || bb2 == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    if(BB_lenght(bb1) < BB_lenght(bb2)){
        big_bool *tmp = bb1;
        bb1 = bb2;
        bb2 = tmp;
    }
    big_bool *bb = BB_make_equal_size(bb1, bb2);
    for(int i = 0; i <= bb->last_byte; i++){
        bb->parts[i] = bb->parts[i] | bb1->parts[i];
    }
    return bb;
}
big_bool *BB_xor(big_bool *bb1, big_bool *bb2){
    if(bb1 == NULL || bb2 == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    if(BB_lenght(bb1) < BB_lenght(bb2)){
        big_bool *tmp = bb1;
        bb1 = bb2;
        bb2 = tmp;
    }
    big_bool *bb = BB_make_equal_size(bb1, bb2);
    for(int i = 0; i <= bb->last_byte; i++){
        bb->parts[i] = bb->parts[i] ^ bb1->parts[i];
    }
    return bb;
}

char *BB_string_input(){
    char symbol = 1;
    char *string = (char *)malloc(1 * sizeof(char));
    if(string == NULL){
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    int i = 0;
    while(symbol != '\n' && symbol != '\0'){
        symbol = getc(stdin);
        string[i] = symbol;
        string = (char *)realloc(string, i * sizeof(char) + 2);
        if(string == NULL){
            bb_errno = ERR_MEM_NOT_ALLOC;
            return NULL;
        }
        i++;
    }
    string[i - 1] = '\0';
    return string;
}

big_bool *BB_from_string(char *string){
    if(string == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    if(strlen(string) < 1)return NULL;
    big_bool vector = {(uint8_t *)calloc(strlen(string), sizeof(uint8_t)), 0, 0};
    if(vector.parts == NULL){
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    char symbol;
    int i = 0;
    size_t new_size;
    while(string[i] != '\0'){
        symbol = string[i];
        if((symbol != '1') && (symbol != '0')){
            bb_errno = ERR_WRONG_CHARS;
            free(vector.parts);
            return NULL;
        }
        vector.parts[vector.last_byte] |= (((symbol == '0')? 0 : 1) << (8 - vector.last_bit -1));
        vector.last_bit++;
        vector.last_bit = vector.last_bit % 8;
        if(vector.last_bit == 0){
            vector.last_byte++;
        }
        i++;
    }

    big_bool *bb = copy_vector(&vector);
    free(vector.parts);
    return bb;
}

char *BB_to_string(big_bool *bb){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    int i = 0, j = 0, k = 0;
    char *string = (char *)malloc(BB_lenght(bb) + 1);
    if(string == NULL){
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    while((i != bb->last_byte) || (j != bb->last_bit)){
        string[k] = '0' + ((((bb->parts[i] & (1 << (8 - j - 1)))) > 0)? 1 : 0);
        j++;
        k++;
        j = j % 8;
        if(j == 0){
            i++;
        }
    }
    string[k] = '\0';
    return string;
}

big_bool *BB_from_uint64_t(uint64_t number){
    big_bool vector = {(uint8_t *)calloc(9, sizeof(uint8_t)), 0, 8};
    if(vector.parts == NULL){
        bb_errno = ERR_MEM_NOT_ALLOC;
        return NULL;
    }
    uint64_t mask = (2 << 8) - 1;
    int i = 1;
    while(number){
        vector.parts[vector.last_byte - i] = number & mask;
        number = number >> 8;
        i++;
    }
    big_bool *bb = copy_vector(&vector);
    free(vector.parts);
    return bb;
}

big_bool *BB_inverting(big_bool *bb){
    if(bb == NULL){
        bb_errno = ERR_NULL_INPUT;
        return NULL;
    }

    big_bool *vector = copy_vector(bb);
    for(int i = 0; i <= bb->last_byte; i++){
        vector->parts[i] = ~(vector->parts[i]);
    }
    return vector;
}