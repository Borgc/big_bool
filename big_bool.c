#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include "big_bool.h"
#include <stdlib.h>

typedef struct big_bool big_bool;

void BB_free(big_bool *bb){
    if(bb != NULL) {
        free(bb->parts);
        bb->parts = NULL;
        free(bb);
        bb = NULL;
    }
}

big_bool *copy_vector(big_bool *bb){
    big_bool *twin_vector = (big_bool *)calloc(1, sizeof(big_bool));
    twin_vector->parts = (uint8_t *)calloc(bb->last_byte + 1, sizeof(uint8_t));
    twin_vector->last_bit = bb->last_bit;
    twin_vector->last_byte = bb->last_byte;
    for(int i = 0; i <= bb->last_byte; i++){
        twin_vector->parts[i] = bb->parts[i];
    }
    return twin_vector;
}

// big_bool *BB_from_file(FILE *input_file){
//     if(!input_file)return NULL;

//     big_bool vector = {(uint8_t *)calloc(1, sizeof(uint8_t)), 0, 0};
//     char symbol;
//     size_t new_size;
//     while(!feof(input_file)){
//         symbol = getc(input_file);
//         if(symbol == '\n' || symbol == EOF)break;
//         vector.parts[vector.last_byte] |= 
//         (((symbol == '0')? 0 : 1) << (8 - vector.last_bit -1));
//         vector.last_bit++;
//         vector.last_bit = vector.last_bit % 8;
//         if(vector.last_bit == 0){
//             new_size = (size_t)((vector.last_byte + 1) * sizeof(uint8_t)) +2;
//             vector.parts = (uint8_t *)realloc(vector.parts, new_size);
//             vector.last_byte++;
//         }
//     }
//     big_bool *bb = copy_vector(&vector);
//     free(vector.parts);
//     return bb;
// }


void BB_output(big_bool *bb){
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
    if(vector == NULL)return -1;
    return vector->last_byte*8 + vector->last_bit;
}

int power(int a, int b){
    int c = 1;
    if(b == 0 && a == 0)return 1;
    if(b == 0)return 1;
    if(a == 0)return 0;
    for(int i = 0; i< b; i++){
        c *=a;
    }
    return c;
}
big_bool *BB_right_shift(big_bool *bb, int number){
    if(!bb)return NULL;
    if(number <  0)return NULL;
    int i = number/8;
    int j = 0;
    int ost = number%8;
    int mask;
    big_bool *vector = copy_vector(bb);
    while((bb->last_byte - j >= 0) && (bb->last_byte - i >= 0)){
        if(bb->last_byte - i -1 < 0){
            mask = 0;
        } else {
        mask = ((power(2, ost) - 1) & bb->parts[bb->last_byte - i - 1]);}
        mask = (mask << (8 - (ost)));
        mask += (((power(2, 8) - 1) & bb->parts[bb->last_byte - i]) >> ost);
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
    if(!bb)return NULL;
    if(number <  0)return NULL;
    int ost = number % 8;
    int i;
    big_bool *vector = copy_vector(bb);
    for(i = 0; i <= bb->last_byte - number/8; i++){
        int bytes = number/8 + i;
        if(number/8 +i +1 <= bb->last_byte){
            bytes = number/8 + i;
            vector->parts[i] =((((power(2,8-ost))-1) & bb->parts[bytes])<< ost) +
            ((((power(2,8) -1) << (8-ost)) & bb->parts[bytes +1]) >>(8-ost));
        }else{vector->parts[i] =((power(2,8-ost)-1)& bb->parts[bytes])<< ost;}
    }
    vector->parts[i - 1] &= ((power(2, 8) - 1) - (power(2, 8 - bb->last_bit) - 1));
    while(i <= bb->last_byte){
        vector->parts[i]=0;
        i++;
    }
    
    return vector;
}

big_bool *BB_cyclic_right_shift(big_bool *bb, int number){
    if(!bb)return NULL;
    if(number <  0)return NULL;
    number %= BB_lenght(bb);
    big_bool *bb1 = copy_vector(bb);
    big_bool *bb2 = copy_vector(bb); 
    big_bool *bb3 = BB_right_shift(bb1, number);
    BB_free(bb1);
    big_bool *bb4 = BB_left_shift(bb2, BB_lenght(bb) - number);
    BB_free(bb2);
    for(int i = 0; i <= bb->last_byte; i++){
        bb3->parts[i] += bb4->parts[i];
    }
    BB_free(bb4);
    return(bb3);
}
big_bool *BB_cyclic_left_shift(big_bool *bb, int number){
    if(!bb)return NULL;
    if(number <  0)return NULL;
    number %= BB_lenght(bb);
    big_bool *bb1 = copy_vector(bb);
    big_bool *bb2 = copy_vector(bb); 
    big_bool *bb3 = BB_left_shift(bb1, number);
    BB_free(bb1);
    big_bool *bb4 = BB_right_shift(bb2, BB_lenght(bb) - number);
    BB_free(bb2);
    for(int i = 0; i <= bb->last_byte; i++){
        bb3->parts[i] += bb4->parts[i];
    }
    BB_free(bb4);
    return(bb3);
}
big_bool *BB_make_equal_size(big_bool *big_vec, big_bool *less_vec){
    if(!big_vec)return NULL;
    if(!less_vec)return NULL;
    big_bool *vector = copy_vector(less_vec);
    vector->parts = realloc(vector->parts, (big_vec->last_byte + 1)*sizeof(uint8_t));
    vector->last_byte = big_vec->last_byte;
    vector->last_bit = big_vec->last_bit;
    big_bool *bb = BB_right_shift(vector, BB_lenght(big_vec) - BB_lenght(less_vec));
    BB_free(vector);
    return bb;
}

big_bool *BB_conjunction(big_bool *bb1, big_bool *bb2){
    if(!bb1)return NULL;
    if(!bb2)return NULL;
    if(BB_lenght(bb1) > BB_lenght(bb2)){
        big_bool *bb = BB_make_equal_size(bb1, bb2);
        for(int i = 0; i <= bb->last_byte; i++){
            bb->parts[i] = bb->parts[i] & bb1->parts[i];
        }   
        return bb;
    } else {
        big_bool *bb = BB_make_equal_size(bb2, bb1);
        for(int i = 0; i <= bb->last_byte; i++){
            bb->parts[i] = bb->parts[i] & bb2->parts[i];
        }
        return bb;
    }
}

big_bool *BB_disjunction(big_bool *bb1, big_bool *bb2){
    if(!bb1)return NULL;
    if(!bb2)return NULL;
    if(BB_lenght(bb1) > BB_lenght(bb2)){
        big_bool *bb = BB_make_equal_size(bb1, bb2);
        for(int i = 0; i <= bb->last_byte; i++){
            bb->parts[i] = bb->parts[i] | bb1->parts[i];
        }   
        return bb;
    } else {
        big_bool *bb = BB_make_equal_size(bb2, bb1);
        for(int i = 0; i <= bb->last_byte; i++){
            bb->parts[i] = bb->parts[i] | bb2->parts[i];
        }
        return bb;
    }
}
big_bool *BB_xor(big_bool *bb1, big_bool *bb2){
    if(!bb1)return NULL;
    if(!bb2)return NULL;
    if(BB_lenght(bb1) > BB_lenght(bb2)){
        big_bool *bb = BB_make_equal_size(bb1, bb2);
        for(int i = 0; i <= bb->last_byte; i++){
            bb->parts[i] = bb->parts[i] ^ bb1->parts[i];
        }   
        return bb;
    } else {
        big_bool *bb = BB_make_equal_size(bb2, bb1);
        for(int i = 0; i <= bb->last_byte; i++){
            bb->parts[i] = bb->parts[i] ^ bb2->parts[i];
        }
        return bb;
    }
}

char *string_input(){
    char symbol = 1;
    char *string = (char *)malloc(1 * sizeof(char));
    int i = 0;
    while(symbol != '\n' && symbol != '\0'){
        symbol = getc(stdin);
        string[i] = symbol;
        string = (char *)realloc(string, i * sizeof(char) + 2);
        i++;
    }
    string[i - 1] = '\0';
    return string;
}

big_bool *BB_from_string(char *string){
    if(!string)return NULL;
    if(strlen(string) < 1)return NULL;
    big_bool vector = {(uint8_t *)calloc(1, sizeof(uint8_t)), 0, 0};
    char symbol;
    int i = 0;
    size_t new_size;
    while(string[i] != '\0'){
        symbol = string[i];
        if((symbol != '1') && (symbol != '0')){
            free(vector.parts);
            return NULL;
        }
        vector.parts[vector.last_byte] |= (((symbol == '0')? 0 : 1) << (8 - vector.last_bit -1));
        vector.last_bit++;
        vector.last_bit = vector.last_bit % 8;
        if(vector.last_bit == 0){
            new_size = (size_t)((vector.last_byte + 2) * sizeof(uint8_t)); //+2;
            vector.parts = (uint8_t *)realloc(vector.parts, new_size);
            vector.last_byte++;
        }
        i++;
    }

    big_bool *bb = copy_vector(&vector);
    free(vector.parts);
    return bb;
}

char *string_from_BB(big_bool *bb){
    if(!bb)return NULL;
    int i = 0, j = 0, k = 0;
    char *string = (char *)malloc(BB_lenght(bb) + 1);
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
    uint64_t mask = power(2, 8) - 1;
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
    if(!bb)return NULL;
    big_bool *vector = copy_vector(bb);
    for(int i = 0; i <= bb->last_byte; i++){
        vector->parts[i] = ~(vector->parts[i]);
    }
    return vector;
}