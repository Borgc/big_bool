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


big_bool *make_random_vec(){
    int lenght = rand() % 256 +1;
    char string[lenght+1];
    for(int i = 0; i < lenght; i++){
        string[i] = rand()%2 + '0';
    }
    string[lenght] = '\x00';
    return BB_from_string(string);
}

int test_BB_from_uint64_t(){
    uint64_t big_number = 
    0b0000000000000001000000100000001100000100000001010000011000000111;
    big_bool *vector = BB_from_uint64_t(big_number);
    char *string = BB_to_string(vector);
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
    //test simple right shift
    big_bool *vec1 = BB_from_string("111000111000");
    big_bool *new_vec = BB_from_string("10101100111000");
    int num_error = 0;

    big_bool *rsh = BB_right_shift(vec1, 3);
    big_bool *new_rsh = BB_right_shift(new_vec, 9);
    big_bool *new_byte_rsh = BB_right_shift(new_vec, 8);

    char *strrsh = BB_to_string(rsh);
    char *str_new_rsh = BB_to_string(new_rsh);
    char *str_new_byte_rsh = BB_to_string(new_byte_rsh);

    if(strcmp(strrsh, "000111000111")){
        num_error++;
        LOG_ERR("Wrong output from BB_right_shift (111000111000 >> 3 != 000111000111)");
    }

    if(strcmp(str_new_rsh, "00000000010101")){
        num_error++;
        LOG_ERR("Wrong output from BB_right_shift (111000111000 >> 9 != 00000000010101)");
    }

    if(strcmp(str_new_byte_rsh, "00000000101011")){
        num_error++;
        LOG_ERR("Wrong output from BB_right_shift (111000111000 >> 8 != 00000000101011)");
    }

    BB_free(rsh);
    free(strrsh);
    BB_free(new_rsh);
    free(str_new_rsh);
    BB_free(new_byte_rsh);
    free(str_new_byte_rsh);

    //test simple left shift
    big_bool *lsh = BB_left_shift(vec1, 5);
    big_bool *new_lsh = BB_left_shift(new_vec, 12);
    big_bool *new_byte_lsh = BB_left_shift(new_vec, 8);

    char *strlsh = BB_to_string(lsh);
    char *str_new_lsh = BB_to_string(new_lsh);
    char *str_new_byte_lsh = BB_to_string(new_byte_lsh);

    if(strcmp(strlsh, "011100000000")){
        num_error++;
        LOG_ERR("Wrong output from BB_left_shift (111000111000 << 5 != 011100000000)");
    }
    if(strcmp(str_new_lsh, "00000000000000")){
        num_error++;
        LOG_ERR("Wrong output from BB_left_shift (10101100111000 << 12 != 00000000000000)");
    }
    if(strcmp(str_new_byte_lsh, "11100000000000")){
        num_error++;
        LOG_ERR("Wrong output from BB_left_shift (10101100111000 << 8 != 11100000000000)");
    }

    BB_free(lsh);
    free(strlsh);
    BB_free(new_lsh);
    free(str_new_lsh);
    BB_free(new_byte_lsh);
    free(str_new_byte_lsh);
    BB_free(new_vec);

    //test cyclic shifts
    big_bool *rcsh = BB_cyclic_right_shift(vec1, 8);
    char *strrcsh = BB_to_string(rcsh);
    if(strcmp(strrcsh, "001110001110")){
        num_error++;
        LOG_ERR("Wrong output from BB_cyclic_right_shift (111000111000 >> 8 != 001110001110)");
    }
    BB_free(rcsh);
    free(strrcsh);

    big_bool *lcsh = BB_cyclic_left_shift(vec1, 11);
    char *strlcsh = BB_to_string(lcsh);
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
    int num_error = 0;

    big_bool *vec1 = BB_from_string            ("111000111000");
    big_bool *vec2 = BB_from_string                ("00110011");
    big_bool *vec3 = BB_from_string                     ("101");
    big_bool *vec4 = BB_from_string        ("1010110011001111");
    big_bool *vec5 = BB_from_string("111100001111000010101100");
    big_bool *vec6 = BB_from_string                ("01010101");

    big_bool *con  = BB_conjunction(vec1, vec2); //different length
    big_bool *con2 = BB_conjunction(vec2, vec6);//both 1 byte length
    big_bool *con3 = BB_conjunction(vec4, vec5);//both more than 1 byte length
    big_bool *con4 = BB_conjunction(vec1, vec3);//both not int byte length

    char *strcon = BB_to_string(con);
    char *strcon2 = BB_to_string(con2);
    char *strcon3 = BB_to_string(con3);
    char *strcon4 = BB_to_string(con4);

    BB_free(con);
    BB_free(con2);
    BB_free(con3);
    BB_free(con4);

    if(strcmp(strcon, "000000110000")){
        num_error++;
        LOG_ERR("Wrong output from BB_conjunction (con != 000000110000)");
    }
    if(strcmp(strcon2, "00010001")){
        num_error++;
        LOG_ERR("Wrong output from BB_conjunction (con2 != 00010001)");
    }
    if(strcmp(strcon3, "000000001010000010001100")){
        num_error++;
        LOG_ERR("Wrong output from BB_conjunction (con3 != 000000001010000010001100)");
    }
    if(strcmp(strcon4, "000000000000")){
        num_error++;
        LOG_ERR("Wrong output from BB_conjunction (con4 != 000000000000)");
    }

    free(strcon);
    free(strcon2);
    free(strcon3);
    free(strcon4);

    big_bool *dis  = BB_disjunction(vec1, vec2); //different length
    big_bool *dis2 = BB_disjunction(vec2, vec6);//both 1 byte length
    big_bool *dis3 = BB_disjunction(vec4, vec5);//both more than 1 byte length
    big_bool *dis4 = BB_disjunction(vec1, vec3);//both not int byte length

    char *strdis = BB_to_string(dis);
    char *strdis2 = BB_to_string(dis2);
    char *strdis3 = BB_to_string(dis3);
    char *strdis4 = BB_to_string(dis4);

    BB_free(dis);
    BB_free(dis2);
    BB_free(dis3);
    BB_free(dis4);

    if(strcmp(strdis, "111000111011")){
        num_error++;
        LOG_ERR("Wrong output from BB_disjunction (dis != 111000111011)");
    }
    if(strcmp(strdis2, "01110111")){
        num_error++;
        LOG_ERR("Wrong output from BB_disjunction (dis2 != 01110111)");
    }
    if(strcmp(strdis3, "111100001111110011101111")){
        num_error++;
        LOG_ERR("Wrong output from BB_disjunction (dis3 != 111100001111110011101111)");
    }
    if(strcmp(strdis4, "111000111101")){
        num_error++;
        LOG_ERR("Wrong output from BB_disjunction (dis4 != 111000111101)");
    }
    free(strdis);
    free(strdis2);
    free(strdis3);
    free(strdis4);

    big_bool *xor  = BB_xor(vec1, vec2); //different length
    big_bool *xor2 = BB_xor(vec2, vec6);//both 1 byte length
    big_bool *xor3 = BB_xor(vec4, vec5);//both more than 1 byte length
    big_bool *xor4 = BB_xor(vec1, vec3);//both not int byte length

    char *strxor = BB_to_string(xor);
    char *strxor2 = BB_to_string(xor2);
    char *strxor3 = BB_to_string(xor3);
    char *strxor4 = BB_to_string(xor4);

    BB_free(xor);
    BB_free(xor2);
    BB_free(xor3);
    BB_free(xor4);

    if(strcmp(strxor, "111000001011")){
        num_error++;
        LOG_ERR("Wrong output from BB_xor (xor != 111000001011)");
    }
    if(strcmp(strxor2, "01100110")){
        num_error++;
        LOG_ERR("Wrong output from BB_xor (xor2 != 01100110)");
    }
    if(strcmp(strxor3, "111100000101110001100011")){
        num_error++;
        LOG_ERR("Wrong output from BB_xor (xor3 != 111100000101110001100011)");
    }
    if(strcmp(strxor4, "111000111101")){
        num_error++;
        LOG_ERR("Wrong output from BB_xor (xor4 != 111000111101)");
    }

    free(strxor);
    free(strxor2);
    free(strxor3);
    free(strxor4);

    big_bool *inv  = BB_inverting(vec1);
    big_bool *inv2 = BB_inverting(vec2);
    big_bool *inv3 = BB_inverting(vec3);
    big_bool *inv4 = BB_inverting(vec4);
    big_bool *inv5 = BB_inverting(vec5);
    big_bool *inv6 = BB_inverting(vec6);

    char *strinv  = BB_to_string(inv);
    char *strinv2 = BB_to_string(inv2);
    char *strinv3 = BB_to_string(inv3);
    char *strinv4 = BB_to_string(inv4);
    char *strinv5 = BB_to_string(inv5);
    char *strinv6 = BB_to_string(inv6);

    BB_free(inv);
    BB_free(inv2);
    BB_free(inv3);
    BB_free(inv4);
    BB_free(inv5);
    BB_free(inv6);

    if(strcmp(strinv, "000111000111")){
        num_error++;
        LOG_ERR("Wrong output from BB_inverting (inv != 000111000111)");
    }
    if(strcmp(strinv2, "11001100")){
        num_error++;
        LOG_ERR("Wrong output from BB_inverting (inv2 != 11001100)");
    }
    if(strcmp(strinv3, "010")){
        num_error++;
        LOG_ERR("Wrong output from BB_inverting (inv3 != 010)");
    }
    if(strcmp(strinv4, "0101001100110000")){
        num_error++;
        LOG_ERR("Wrong output from BB_inverting (inv4 != 0101001100110000)");
    }
    if(strcmp(strinv5, "000011110000111101010011")){
        num_error++;
        LOG_ERR("Wrong output from BB_inverting (inv5 != 000011110000111101010011)");
    }
    if(strcmp(strinv6, "10101010")){
        num_error++;
        LOG_ERR("Wrong output from BB_inverting (inv6 != 10101010)");
    }

    free(strinv);
    free(strinv2);
    free(strinv3);
    free(strinv4);
    free(strinv5);
    free(strinv6);

    BB_free(vec1);
    BB_free(vec2);
    BB_free(vec3);
    BB_free(vec4);
    BB_free(vec5);
    BB_free(vec6);

    return num_error;
}

int test_conversion(){
    int num_error = 0;

    big_bool *vector = make_random_vec();
    
    char *str = BB_to_string(vector);
    big_bool *vector2 = BB_from_string(str);
    char *str2 = BB_to_string(vector2);

    if(strcmp(str, str2)){
        num_error++;
        LOG_ERR("Wrong output from BB_from_string\n");
    }
    BB_free(vector);
    BB_free(vector2);
    free(str2);
    free(str);

    return num_error;
}

int test_special_cases(){
    int num_error = 0;
    
    srand(time(NULL));

    big_bool *vec1 = BB_from_uint64_t(rand()*rand());
    big_bool *vec2 = BB_from_uint64_t(rand()*rand());
    //this test is about shift >= length that is zero
    int num = 64;

    big_bool *lsh = BB_left_shift(vec1, num);
    big_bool *rsh = BB_right_shift(vec2, num);

    char *strlsh = BB_to_string(lsh);
    char *strrsh = BB_to_string(rsh);

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

    char *strlcsh = BB_to_string(lcsh);
    char *strvec1 = BB_to_string(vec1);
    if(strcmp(strlcsh, strvec1)){
        num_error++;
        LOG_ERR("Wrong output from BB_cyclic_left_shift ((uint64_t)vector cyclic shift 64 != vector)")
    }
    big_bool *rcsh = BB_cyclic_right_shift(vec2, num);

    char *strrcsh = BB_to_string(rcsh);
    char *strvec2 = BB_to_string(vec2);
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

    //NULL cases and ERROR outputs
    
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
    if(NULL != BB_to_string(NULL))num_error++;
    if(NULL != BB_from_string("abcd"))num_error++;
    return num_error;
}



int test_double_invertion() {
    int num_error = 0;

    srand(time(NULL));

    //double invertion inv(inv(vec))=vec
    big_bool *inv1 = make_random_vec();
    big_bool *inv2 = BB_inverting(inv1);
    big_bool *inv3 = BB_inverting(inv2);

    char *strdinv1 = BB_to_string(inv1);
    char *strdinv3 = BB_to_string(inv3);

    if (strcmp(strdinv1, strdinv3)) {
        num_error++;
        LOG_ERR("Double invertion doesn't work");
    }

    BB_free(inv1);
    BB_free(inv2);
    BB_free(inv3);

    free(strdinv1);
    free(strdinv3);
}
int test_x_con_not_x(){
    int num_error = 0;
    //vec&inv(vec) = 0
    big_bool *inv4 = make_random_vec();
    big_bool *inv5 = BB_inverting(inv4);
    big_bool *coninv = BB_conjunction(inv4, inv5);
    char *strconinv = BB_to_string(coninv);

    char *string = calloc(BB_lenght(inv4) + 1, sizeof(char));
    for (int i = 0; i < BB_lenght(inv4); i++) {
        string[i] = '0';
    }


    if (strcmp(strconinv, string)) {
        num_error++;
        LOG_ERR("Negation doesn't work");
    }
    free(string);

    BB_free(inv4);
    BB_free(inv5);
    BB_free(coninv);
    free(strconinv);

    return num_error;
}
int test_x_dis_not_x() {
    int num_error = 0;
    //vec + inv(vec) = 1
    big_bool *a = make_random_vec();
    big_bool *not_a = BB_inverting(a);
    big_bool *a_dis_not_a = BB_disjunction(a, not_a);
    char *str_a_dis_not_a = BB_to_string(a_dis_not_a);

    char one_string[BB_lenght(a)];
    for (int i = 0; i < BB_lenght(a); i++) {
        one_string[i] = '1';
    }
    one_string[BB_lenght(a)] = '\x00';
    if (strcmp(str_a_dis_not_a, one_string)) {
        num_error++;
        LOG_ERR("Third wheel doesn't work");
    }
    BB_free(a);
    BB_free(not_a);
    BB_free(a_dis_not_a);
    free(str_a_dis_not_a);
    return num_error;
}
int test_De_Morgan(){
    int num_error = 0;
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

    char *str_left = BB_to_string(inv_c_con_d);
    char *str_right = BB_to_string(not_c_dis_not_d);

    if(strcmp(str_left, str_right)){
        num_error++;
        LOG_ERR("De Morgan doesn't work");
    }

    BB_free(inv_c_con_d);
    BB_free(not_c_dis_not_d);
    free(str_left);
    free(str_right);
    //making vectors equal size
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
    char *str_thirst = BB_to_string(not_x_dis_y);
    char *str_second = BB_to_string(not_x_con_not_y);

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

    char *str_f_xor_g = BB_to_string(f_xor_g);
    char *str_con_dis_con = BB_to_string(con_dis_con);

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
                num_error += test_x_con_not_x();
                num_error += test_x_dis_not_x();
                num_error += test_De_Morgan();
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

