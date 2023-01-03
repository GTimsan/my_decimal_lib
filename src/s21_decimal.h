#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <string.h>

#define RESULT_ERROR 1

#define RESULT_SUCCESS 0
#define MAXLIMIT_ERROR 1  // число слишком велико или равно бесконечности
#define MINLIMIT_ERROR \
  2  // число слишком мало или равно отрицательной бесконечности
#define DIVBYZERO_ERROR 3  // деление на 0

#define CONVERTING_ERROR 1

#define is_nan(x) __builtin_isnan(x)
#define AFTER_DOT 8

typedef struct {
    int bits[4];
} s21_decimal;

typedef struct {
    unsigned bits[8];
    unsigned opt;
} big_decimal;

// Функции проекта
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_from_float_to_decimal(float src, s21_decimal* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);

int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst);

int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_floor(s21_decimal value, s21_decimal* dst);
int s21_round(s21_decimal value, s21_decimal* dst);
int s21_truncate(s21_decimal value, s21_decimal* dst);
int s21_negate(s21_decimal value, s21_decimal* dst);

// Для обработки s21_decimal
int mult(s21_decimal a, s21_decimal b, s21_decimal* result);
int plus(s21_decimal a, s21_decimal b, s21_decimal* result);
int cleanBit(s21_decimal* d, int i);
int setBit(s21_decimal* d, int i);
int getBit(s21_decimal d, int i);
int shift(s21_decimal* d, int flag);  // 1 - сдвигаем влево, -1 - сдвиг вправо
int getCount(s21_decimal a);
int getExp(float f);
int setScale(s21_decimal* dst, int scale);
int getScale(s21_decimal src);
int setSign(s21_decimal* dst);
int getSign(s21_decimal dst);
int not_null(s21_decimal d);
void cleanSign(s21_decimal* value);

unsigned char* p_bite(s21_decimal* p_value, int* index);
char read_bit(s21_decimal* p_value, int index);
void change_bit(s21_decimal* p_value, int index, unsigned char bit);
int get_sign(s21_decimal* p_value);
int get_exp(s21_decimal* p_value);
int clean_Scale(s21_decimal* dc);
int s_down(s21_decimal* bd, int val, s21_decimal* result);
int to_z(s21_decimal* d);
int div(s21_decimal n, s21_decimal d, s21_decimal* result);
int big_or(s21_decimal a, s21_decimal b);
int minus(s21_decimal a, s21_decimal b, s21_decimal* result);
int to_dop_code(s21_decimal b, s21_decimal* d);
int getCount(s21_decimal a);

// Для обработки big_decimal
int big_big_small(big_decimal a, big_decimal b);
int set_bigSign(big_decimal* dst);
int get_bigSign(big_decimal dst);
int to_one_scale(s21_decimal* first, s21_decimal* second, big_decimal* bd_first,
                 big_decimal* bd_second);
int big_getScale(big_decimal src);
int big_to_dec(big_decimal big_d, s21_decimal* d);
int scale_down(big_decimal* bd, int val, big_decimal* result);
int scale_up(big_decimal* bd, int val, big_decimal* result);
int clean_bigScale(big_decimal* dc);
int big_setScale(big_decimal* dst, int scale);
int to_zero(big_decimal* d);
int big_div(big_decimal n, big_decimal d, big_decimal* result);
int big_mult(big_decimal a, big_decimal b, big_decimal* result);
int big_big_or(big_decimal a, big_decimal b);
int big_shift(big_decimal* d, int flag);
int big_minus(big_decimal a, big_decimal b, big_decimal* result);
int big_plus(big_decimal a, big_decimal b, big_decimal* result);
int clean_bigBit(big_decimal* d, int i);
int get_bigCount(big_decimal a);
int set_bigBit(big_decimal* d, int i);
int get_bigBit(big_decimal d, int i);
int big_not_null(big_decimal d);
int dec_to_big(big_decimal* big_d, s21_decimal d);
int big_to_dop_code(big_decimal b, big_decimal* d);

#endif  // SRC_S21_DECIMAL_H_
