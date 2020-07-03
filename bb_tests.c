#include"big_bool.h"
#include<stdio.h>
#include<string.h>
#include <stdint.h>

#define RESET "\033[0m"
#define RED   "\033[31m"		/* Red */
#define GREEN "\033[32m"		/* Green */

#define LOG_ERR(msg) \
	printf(RED"%s:%d %s [ERR] %s\n"RESET,\
		__FILE__, __LINE__, __func__, msg);

int test_BB_from_uint64_t(){
    uint64_t big_number = 
    0b0000000000000001000000100000001100000100000001010000011000000111;
    big_bool *vector = BB_from_uint64_t(big_number);
    char *string = string_from_BB(vector);
    if(strcmp(string, 
    "0000000000000001000000100000001100000100000001010000011000000111") != 0){
        LOG_ERR("Wrong output from BB_from_uint64_t");
        BB_free(vector);
        free(string);
        return 1;
    }
    BB_free(vector);
    free(string);
    return 0;
}

int test_shifts(){
    //test simple shift n < 8;
    big_bool *vec1 = BB_from_string("111000111000");
    
    int num_error = 0;

    big_bool *rsh = BB_right_shift(vec1, 3);
    char *strrsh = string_from_BB(rsh);
    if(strcmp(strrsh, "000111000111")){
        num_error++;
        LOG_ERR("Wrong output from BB_right_shift (111000111000 >> 3 != 000111000111)");
    }
    BB_free(rsh);
    free(strrsh);

    big_bool *lsh = BB_left_shift(vec1, 5);
    char *strlsh = string_from_BB(lsh);
    if(strcmp(strlsh, "011100000000")){
        num_error++;
        LOG_ERR("Wrong output from BB_left_shift (111000111000 << 5 != 011100000000)");
    }
    BB_free(lsh);
    free(strlsh);

    big_bool *rcsh = BB_cyclic_right_shift(vec1, 8);
    char *strrcsh = string_from_BB(rcsh);
    if(strcmp(strrcsh, "001110001110")){
        num_error++;
        LOG_ERR("Wrong output from BB_cyclic_right_shift (111000111000 >> 8 != 001110001110)");
    }
    BB_free(rcsh);
    free(strrcsh);

    big_bool *lcsh = BB_cyclic_left_shift(vec1, 11);
    char *strlcsh = string_from_BB(lcsh);
    if(strcmp(strlcsh, "011100011100")){
        num_error++;
        LOG_ERR("Wrong output from BB_cyclic_left_shift (111000111000 << 11 != 011100011100)");
    }
    BB_free(lcsh);
    free(strlcsh);

    BB_free(vec1);

    return num_error;
}

int test_logic_functions(){
    big_bool *vec1 = BB_from_string("111000111000");
    big_bool *vec2 = BB_from_string    ("00110011");

    big_bool *con = BB_conjunction(vec1, vec2);
    big_bool *dis = BB_disjunction(vec1, vec2);
    big_bool *xor = BB_xor(vec1, vec2);
    big_bool *inv = BB_inverting(vec1);
    BB_free(vec1);
    BB_free(vec2);
    int num_error = 0;

    char *strcon = string_from_BB(con);
    BB_free(con);
    char *strdis = string_from_BB(dis);
    BB_free(dis);
    char *strxor = string_from_BB(xor);
    BB_free(xor);
    char *strinv = string_from_BB(inv);
    BB_free(inv);

    if(strcmp(strcon, "000000110000")){
        num_error++;
        LOG_ERR("Wrong output from BB_conjunction (con != 000000110000)");
    }
    if(strcmp(strdis, "111000111011")){
        num_error++;
        LOG_ERR("Wrong output from BB_disjunction (dis != 111000111011)");
    }
    if(strcmp(strxor, "111000001011")){
        num_error++;
        LOG_ERR("Wrong output from BB_xor (xor != 111000001011)");
    }
    if(strcmp(strinv, "000111000111")){
        num_error++;
        LOG_ERR("Wrong output from BB_inverting (inv != 000111000111)");
    }

    free(strcon);
    free(strdis);
    free(strxor);
    free(strinv);

    return num_error;
}

int test_conversion(){
    int num_error = 0;

    big_bool *vector = BB_from_string("10101010");
    
    char *str = string_from_BB(vector);
    if(strcmp(str, "10101010")){
        num_error++;
        LOG_ERR("Wrong output from BB_from_string (vector != 10101010)");
    }
    BB_free(vector);
    free(str);

    return num_error;
}

int test_special_cases(){
    int num_error = 0;
    
    srand(time(NULL));

    big_bool *vec1 = BB_from_uint64_t(rand()*rand());
    big_bool *vec2 = BB_from_uint64_t(rand()*rand());

    int num = 64;

    big_bool *lsh = BB_left_shift(vec1, num);
    big_bool *rsh = BB_right_shift(vec2, num);

    char *strlsh = string_from_BB(lsh);
    char *strrsh = string_from_BB(rsh);

    BB_free(lsh);
    BB_free(rsh);

    if(strcmp(strlsh, "0000000000000000000000000000000000000000000000000000000000000000")){
        num_error++;
        LOG_ERR("Wrong output from BB_left_shift if num = LenOfVec");
    }
    if(strcmp(strrsh, "0000000000000000000000000000000000000000000000000000000000000000")){
        num_error++;
        LOG_ERR("Wrong output from BB_right_shift if num = LenOfVec");
    }

    free(strlsh);
    free(strrsh);

    big_bool *lcsh = BB_cyclic_left_shift(vec1, num);

    char *strlcsh = string_from_BB(lcsh);
    char *strvec1 = string_from_BB(vec1);
    if(strcmp(strlcsh, strvec1)){
        num_error++;
        LOG_ERR("Wrong output from BB_cyclic_left_shift ((uint64_t)vector cyclic shift 64 != vector)")
    }
    big_bool *rcsh = BB_cyclic_right_shift(vec2, num);

    char *strrcsh = string_from_BB(rcsh);
    char *strvec2 = string_from_BB(vec2);
    if(strcmp(strrcsh, strvec2)){
        num_error++;
        LOG_ERR("Wrong output from BB_cyclic_right_shift ((uint64_t)vector cyclic shift 64 != vector)")
    }

    BB_free(vec1);
    BB_free(vec2);
    BB_free(rcsh);
    BB_free(lcsh);

    free(strlcsh);
    free(strvec1);
    free(strrcsh);
    free(strvec2);

    //NULL cases
    
    if(-1 != BB_lenght(NULL))num_error++;
    if(NULL != BB_right_shift(NULL, 3))num_error++;
    if(NULL != BB_left_shift(NULL, 3))num_error++;
    if(NULL != BB_cyclic_left_shift(NULL, 0))num_error++;
    if(NULL != BB_cyclic_right_shift(NULL, 0))num_error++;
    if(NULL != BB_conjunction(NULL, NULL))num_error++;
    if(NULL != BB_disjunction(NULL, NULL))num_error++;
    if(NULL != BB_xor(NULL, NULL))num_error++;
    if(NULL != BB_inverting(NULL))num_error++;
    if(NULL != BB_from_string(NULL))num_error++;
    if(NULL != string_from_BB(NULL))num_error++;
    if(NULL != BB_from_string("abcd"))num_error++;
    return num_error;
}

big_bool *make_random_vec(){
    int lenght = rand() % 256 +1;
    char string[lenght+1];
    for(int i = 0; i < lenght; i++){
        string[i] = rand()%2 + '0';
    }
    string[lenght] = '\x00';
    return BB_from_string(string);
}

int test_edentities(){
    int num_error = 0;

    srand(time(NULL));

    //double invertion inv(inv(vec))=vec
    big_bool *inv1 = make_random_vec();
    big_bool *inv2 = BB_inverting(inv1);
    big_bool *inv3 = BB_inverting(inv2);

    char *strdinv1 = string_from_BB(inv1);
    char *strdinv3 = string_from_BB(inv3);

    if(strcmp(strdinv1, strdinv3)){
        num_error++;
        LOG_ERR("Double invertion doesn't work");
    }

    BB_free(inv1);
    BB_free(inv2);
    BB_free(inv3);

    free(strdinv1);
    free(strdinv3);

    //vec&inv(vec) = 0
    big_bool *inv4 = make_random_vec();
    big_bool *inv5 = BB_inverting(inv4);
    big_bool *coninv = BB_conjunction(inv4, inv5);
    char *strconinv = string_from_BB(coninv);

    char string[BB_lenght(inv4)+1];
    for(int i = 0; i < BB_lenght(inv4); i++){
        string[i] = '0';
    }
    string[BB_lenght(inv4)] = '\x00';

    if(strcmp(strconinv, string)){
        num_error++;
        LOG_ERR("Negation doesn't work");
    }

    BB_free(inv4);
    BB_free(inv5);
    BB_free(coninv);
    free(strconinv);

    //vec + inv(vec) = 1
    big_bool *a = make_random_vec();
    big_bool *not_a = BB_inverting(a);
    big_bool *a_dis_not_a = BB_disjunction(a, not_a);
    char *str_a_dis_not_a = string_from_BB(a_dis_not_a);

    char one_string[BB_lenght(a)];
    for(int i = 0; i < BB_lenght(a); i++){
        one_string[i] = '1';
    }
    one_string[BB_lenght(a)] = '\x00';
    if(strcmp(str_a_dis_not_a, one_string)){
        num_error++;
        LOG_ERR("Third wheel doesn't work");
    }
    BB_free(a);
    BB_free(not_a);
    BB_free(a_dis_not_a);
    free(str_a_dis_not_a);

    //De Morgan
    big_bool *c = make_random_vec();
    big_bool *d = make_random_vec();

    if(BB_lenght(c) > BB_lenght(d)){
        big_bool *bb = BB_make_equal_size(c, d);
        BB_free(d);
        d = bb;
    }
    if(BB_lenght(c) < BB_lenght(d)){
        big_bool *bb = BB_make_equal_size(d, c);
        BB_free(c);
        c = bb;
    }

    big_bool *c_con_d = BB_conjunction(c, d);
    big_bool *inv_c_con_d = BB_inverting(c_con_d);

    big_bool *not_c = BB_inverting(c);
    big_bool *not_d = BB_inverting(d);
    big_bool *not_c_dis_not_d = BB_disjunction(not_c, not_d);

    BB_free(c);
    BB_free(d);
    BB_free(c_con_d);
    BB_free(not_c);
    BB_free(not_d);

    char *str_left = string_from_BB(inv_c_con_d);
    char *str_right = string_from_BB(not_c_dis_not_d);

    if(strcmp(str_left, str_right)){
        num_error++;
        LOG_ERR("De Morgan doesn't work");
    }

    BB_free(inv_c_con_d);
    BB_free(not_c_dis_not_d);
    free(str_left);
    free(str_right);

    big_bool *x = make_random_vec();
    big_bool *y = make_random_vec();

    if(BB_lenght(x) > BB_lenght(y)){
        big_bool *bb = BB_make_equal_size(x, y);
        BB_free(y);
        y = bb;
    }
    if(BB_lenght(x) < BB_lenght(y)){
        big_bool *bb = BB_make_equal_size(y, x);
        BB_free(x);
        x = bb;
    }

    big_bool *x_dis_y = BB_disjunction(x, y);
    big_bool *not_x_dis_y = BB_inverting(x_dis_y);
    big_bool *not_x = BB_inverting(x);
    big_bool *not_y = BB_inverting(y);
    big_bool *not_x_con_not_y = BB_conjunction(not_x, not_y);
    char *str_thirst = string_from_BB(not_x_dis_y);
    char *str_second = string_from_BB(not_x_con_not_y);

    if(strcmp(str_thirst, str_second)){
        num_error++;
        LOG_ERR("De Morgan doesn't work");
    }

    BB_free(x);
    BB_free(y);
    BB_free(x_dis_y);
    BB_free(not_x_dis_y);
    BB_free(not_x);
    BB_free(not_y);
    BB_free(not_x_con_not_y);
    free(str_thirst);
    free(str_second);

    //f xor g = f&inv(g) | g&inv(f)
    big_bool *f = make_random_vec();
    big_bool *g = make_random_vec();

    if(BB_lenght(f) > BB_lenght(g)){
        big_bool *bb = BB_make_equal_size(f, g);
        BB_free(g);
        g = bb;
    }
    if(BB_lenght(f) < BB_lenght(g)){
        big_bool *bb = BB_make_equal_size(g, f);
        BB_free(f);
        f = bb;
    }

    big_bool *f_xor_g = BB_xor(f, g);
    big_bool *not_f = BB_inverting(f);
    big_bool *not_g = BB_inverting(g);
    big_bool *f_con_not_g = BB_conjunction(f, not_g);
    big_bool *g_con_not_f = BB_conjunction(not_f, g);
    big_bool *con_dis_con = BB_disjunction(f_con_not_g, g_con_not_f);

    BB_free(f);
    BB_free(g);
    BB_free(not_f);
    BB_free(not_g);
    BB_free(f_con_not_g);
    BB_free(g_con_not_f);

    char *str_f_xor_g = string_from_BB(f_xor_g);
    char *str_con_dis_con = string_from_BB(con_dis_con);

    BB_free(f_xor_g);
    BB_free(con_dis_con);
    if(strcmp(str_f_xor_g, str_con_dis_con)){
        num_error++;
        LOG_ERR("XOR doesn't work");
    }
    free(str_f_xor_g);
    free(str_con_dis_con);

    return num_error;
}
int main(){
    int num_error = 0;

    num_error += test_BB_from_uint64_t();
    num_error += test_shifts();
    num_error += test_logic_functions();
    num_error += test_conversion();
    num_error += test_special_cases();

    printf("Do you want to begin test with random cases? 1 - yes, 0 - no\n");
    char symbol = getc(stdin);
    if(symbol == '1'){
        printf("Choose number of iterations\n");
        int number;
        scanf("%d", &number);
        if(number <= 0){
            printf("Wrong input, try to type number > 0 next time \n");
        } else {
            for (int i = 0; i < number; i++) {
                num_error += test_edentities();
            }
        }
    }
    if (num_error>0){
		printf ("Number of failed tests: %d\n", num_error);
		return 0;
	} else {
	puts (GREEN"[OK] All is ok!"RESET);
    }

	return 0;
}

