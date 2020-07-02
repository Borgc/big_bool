#ifndef BIG_BOOL_H

#define BIG_BOOL_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

struct big_bool{
    uint8_t *parts;
    int last_bit;
    int last_byte;
};

typedef struct big_bool big_bool;

/* -------------------------------------------------------------------------
 * I/O Functions
 * ------------------------------------------------------------------------- 
 */

// /*Fuction: BB_from_file
//  *returns pointer to structure type of big_bool from FILE *input_file
//  *where the vector is written as a string
//  *returns NULL in case of FILE * == NULL
//  *after using the vector you should to free allocated memory
//  *look at BB_free(big_bool *bb)
//  */
// big_bool *BB_from_file(FILE *input_file);

/*Function: BB_output
*using to output the vector to the terminal
*/
void BB_output(big_bool *bb);

/*Function: string_input
 *input string from stdin
 */
char *string_input();

/* -------------------------------------------------------------------------
 * Useful functions
 * ------------------------------------------------------------------------- 
 */

/*Fuction: BB_lenght
 *returns lenght of bool vector: lenght(1001) --> 4
 */
int BB_lenght(big_bool *bool_vector);

/*Function: BB_make_equal_size
 *returns pointer to structure type big_bool less lenght with increased lenght.
 *This is a very useful function, because in some cases you should to
 *use other functions carefully.
 *
 *For example: inv(x&y) = inv(x) | inv(y) identity
 *x&y will bring the lengths of vectors to one size
 *if you invert x&y you'll get in the beginning "111...11"
 *but if you try to do right part you'll get another answer
 *without making their size equal.
 */
big_bool *BB_make_equal_size(big_bool *big_vec, big_bool *less_vec);
/* -------------------------------------------------------------------------
 * Shift fuctions
 * ------------------------------------------------------------------------- 
 * As for all shift functions if number = 0 then functions returns the same 
 * structure type of bug_bool.
 * If number < 0 then functions returns NULL.
 * 
 * If number >= BB_lenght(bb) then not cyclic shifts returns pointer to
 * structure type of big_bool with zero in each component.
 * This happens because of size of structure, it will not grow with shift!
 * For gaining "growing" effect 
 * use BB_make_equal_size(big_bool *big_vec, less_vec) and then shift your 
 * vector type of big_bool.
 */

/*Function: BB_right_shift
 *returns pointer to structure type of big_bool
 *where the vector shifted right on number of bits
 */
big_bool *BB_right_shift(big_bool *bb, int number);

/*Function: BB_left_shift
 *returns pointer to structure type of big_bool
 *where the vector shifted left on number of bits
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 *
 * Behavior with number >= BB_lenght(bb) such a zero vector the same lenght 
 */
big_bool *BB_left_shift(big_bool *bb, int number);

/*Function: BB_cyclic_right_shift
 *returns pointer to structure type of big_bool
 *where the vector shifted cyclic right on number of bits
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 *
 * Behavior with number >= BB_lenght(bb) such a zero vector the same lenght 
 */
big_bool *BB_cyclic_right_shift(big_bool *bb, int number);

/*Function: BB_cyclic_left_shift
 *returns pointer to structure type of big_bool
 *where the vector shifted cyclic left on number of bits
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 */
big_bool *BB_cyclic_left_shift(big_bool *bb, int number);

/*Fuction: BB_free
 *free your bb->parts and bb because they are allocated
 *use it after each function where you no longer need to use your vector
 */
void BB_free(big_bool *bb);

/* -------------------------------------------------------------------------
 * Logic Functions
 * ------------------------------------------------------------------------- 
 */

/*Fuction BB_conjunction
 *returns pointer to structure big_bool representing
 *conjunction of bb1 and bb2 vectors
 *
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 */
big_bool *BB_conjunction(big_bool *bb1, big_bool *bb2);

/*Fuction BB_xor
 *returns pointer to structure big_bool representing
 *xor of bb1 and bb2 vectors
 *
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 */
big_bool *BB_xor(big_bool *bb1, big_bool *bb2);

/*Fuction BB_disjunction
 *returns pointer to structure big_bool representing
 *disjunction of bb1 and bb2 vectors
 *
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 */
big_bool *BB_disjunction(big_bool *bb1, big_bool *bb2);

/*Fuction BB_inverting
 *returns pointer to structure big_bool representing
 *inverted bb vector
 *
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 */
big_bool *BB_inverting(big_bool *bb);

/* -------------------------------------------------------------------------
 * Conversion Functions
 * ------------------------------------------------------------------------- 
 */

/*Function: BB_from_srting
 *Returns pointer to structure type of big_bool
 *converted from string
 *
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 */
big_bool *BB_from_string(char *string);

/*Function: string_from_BB
 *Returns pointer to string converted from structure type of big_bool
 *Make free(string); after using this function
 */
char *string_from_BB(big_bool *bb);

/*Function: BB_from_uint64_t
 *Returns pointer to structure type of big_bool
 *converted from uint64_t
 *
 *after using the vector you should to free allocated memory
 *look at BB_free(big_bool *bb)
 */
big_bool *BB_from_uint64_t(uint64_t number);

#endif /*BIG_BOOL_H*/