#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error = 0;
    s21_decimal zero = {0};
    *result = zero;
    if (!not_null(value_2)) {
        error = 3;
    } else if (not_null(value_1)) {
        big_decimal bd_first = {0};
        big_decimal bd_second = {0};
        big_decimal bd_result = {0};
        big_decimal bd_result_tmp = {0};
        to_one_scale(&value_1, &value_2, &bd_first, &bd_second);
        int scale_result = 0;
        while (big_not_null(bd_first) && scale_result < 30) {
            if (big_big_or(bd_first, bd_second)) {
                int count_shift = get_bigCount(bd_first) - get_bigCount(bd_second);
                for (int i = 0; i < count_shift; i++) {
                    big_shift(&bd_second, 1);
                }
                for (int j = 0; j <= count_shift; j++) {
                    if (big_big_or(bd_first, bd_second)) {
                        big_minus(bd_first, bd_second, &bd_first);
                        set_bigBit(&bd_result_tmp, 0);
                    }
                    if (j < count_shift) {
                        big_shift(&bd_result_tmp, 1);
                        big_shift(&bd_second, -1);
                    }
                }

                big_plus(bd_result, bd_result_tmp, &bd_result);
                big_decimal big_zero = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
                bd_result_tmp = big_zero;

            } else {
                scale_result++;
                big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}, 0};
                big_mult(bd_first, ten, &bd_first);
                big_mult(bd_result, ten, &bd_result);
            }
        }
        big_setScale(&bd_result, scale_result);
        if (get_sign(&value_1) != get_sign(&value_2)) set_bigSign(&bd_result);
        error = big_to_dec(bd_result, result);
    }
    return error;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error = 0;
    s21_decimal zero = {0};
    *result = zero;
    if (!not_null(value_2)) {
        error = 3;
    } else if (!not_null(value_1)) {
    } else {
        big_decimal bd_first = {0};
        big_decimal bd_second = {0};
        big_decimal bd_result = {0};
        big_decimal tmp_div = {0};
        big_decimal tmp_mul = {0};

        to_one_scale(&value_1, &value_2, &bd_first, &bd_second);

        if (big_big_or(bd_first, bd_second)) {
            big_div(bd_first, bd_second, &tmp_div);
            big_mult(tmp_div, bd_second, &tmp_mul);
            big_minus(bd_first, tmp_mul, &bd_result);
            if (get_bigSign(bd_first) == 1) {
                set_bigSign(&bd_result);
            }

        } else {
            bd_result = bd_first;
            if (get_bigSign(bd_first) == 1) {
                set_bigSign(&bd_result);
            }
        }
        big_setScale(&bd_result, big_getScale(bd_second));

        error = big_to_dec(bd_result, result);
    }
    return error;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error = 0;
    s21_decimal zero = {0};
    *result = zero;
    big_decimal bd_first = {0};
    big_decimal bd_second = {0};
    big_decimal bd_result = {0};

    to_one_scale(&value_1, &value_2, &bd_first, &bd_second);
    int sign1 = get_bigSign(bd_first);
    int sign2 = get_bigSign(bd_second);
    if (s21_is_equal(value_1, value_2)) {
    } else if (sign1 == 0 && sign2 == 0) {
        if (big_big_small(bd_first, bd_second)) {
            big_minus(bd_first, bd_second, &bd_result);
        } else {
            big_minus(bd_second, bd_first, &bd_result);
            set_bigSign(&bd_result);
        }
    } else if (sign1 && sign2) {
        if (big_big_small(bd_first, bd_second)) {
            big_minus(bd_first, bd_second, &bd_result);
            set_bigSign(&bd_result);
        } else {
            big_minus(bd_second, bd_first, &bd_result);
        }
    } else if (sign1 == 0 && sign2) {
        big_plus(bd_first, bd_second, &bd_result);
    } else if (sign1 && sign2 == 0) {
        big_plus(bd_first, bd_second, &bd_result);
        set_bigSign(&bd_result);
    }
    big_setScale(&bd_result, big_getScale(bd_first));
    error = big_to_dec(bd_result, result);

    return error;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error = 0;
    s21_decimal zero = {0};
    *result = zero;
    big_decimal bd_first = {0};
    big_decimal bd_second = {0};
    big_decimal bd_result = {0};

    to_one_scale(&value_1, &value_2, &bd_first, &bd_second);
    if (get_bigSign(bd_first) == get_bigSign(bd_second)) {
        big_plus(bd_first, bd_second, &bd_result);
        if (get_bigSign(bd_first)) set_bigSign(&bd_result);
    } else {
        if (big_big_small(bd_first, bd_second)) {
            big_minus(bd_first, bd_second, &bd_result);
            if (get_bigSign(bd_first)) set_bigSign(&bd_result);
        } else if (big_big_small(bd_second, bd_first)) {
            big_minus(bd_second, bd_first, &bd_result);
            if (get_bigSign(bd_second)) set_bigSign(&bd_result);
        }
    }
    big_setScale(&bd_result, big_getScale(bd_first));
    error = big_to_dec(bd_result, result);

    return error;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error = 0;
    s21_decimal zero = {0};
    s21_decimal near_null = {{1, 0, 0, 0x001C0000}};
    *result = zero;
    big_decimal bd_first = {0};
    big_decimal bd_second = {0};
    big_decimal bd_result = {0};

    if (s21_is_less_or_equal(value_1, near_null) && get_sign(&value_1) == 0) {
        result->bits[0] = 0;
        result->bits[1] = 0;
        result->bits[2] = 0;
        result->bits[3] = value_1.bits[3];
        if (getSign(value_1) != getSign(value_2)) {
            setSign(result);
        }
    } else if (s21_is_less_or_equal(value_2, near_null) &&
               get_sign(&value_2) == 0) {
        result->bits[0] = 0;
        result->bits[1] = 0;
        result->bits[2] = 0;
        result->bits[3] = value_2.bits[3];
        if (getSign(value_1) != getSign(value_2)) {
            setSign(result);
        }
    } else {
        to_one_scale(&value_1, &value_2, &bd_first, &bd_second);
        big_mult(bd_first, bd_second, &bd_result);
        if (get_bigSign(bd_first) != get_bigSign(bd_second)) {
            set_bigSign(&bd_result);
        }
        big_setScale(&bd_result, big_getScale(bd_first) + big_getScale(bd_second));
        error = big_to_dec(bd_result, result);
    }
    return error;
}

int to_one_scale(s21_decimal *first, s21_decimal *second, big_decimal *bd_first,
                 big_decimal *bd_second) {
    big_decimal bd_result = {0};
    big_decimal tmp_first = {0};
    big_decimal tmp_second = {0};

    dec_to_big(&tmp_first, *first);
    dec_to_big(&tmp_second, *second);
    if (getScale(*first) < getScale(*second)) {
        scale_up(&tmp_first, getScale(*second) - getScale(*first), &bd_result);
        *bd_first = bd_result;
        *bd_second = tmp_second;
    } else if (getScale(*first) > getScale(*second)) {
        scale_up(&tmp_second, getScale(*first) - getScale(*second), &bd_result);
        *bd_second = bd_result;
        *bd_first = tmp_first;
    } else {
        *bd_second = tmp_second;
        *bd_first = tmp_first;
    }
    return 1;
}

int big_div(big_decimal n, big_decimal d, big_decimal *result) {
    to_zero(result);
    if (big_big_or(n, d)) {
        big_decimal tmp_n = n;
        big_decimal tmp_d = d;
        big_decimal tmp_result = {0};
        int count_n = 0;
        int count_d = 0;
        int step_count = 0;
        count_n = get_bigCount(n);
        count_d = get_bigCount(d);
        step_count = count_n - count_d;
        for (int i = 0; i < count_n - count_d; i++) {
            big_shift(&tmp_d, 1);
        }
        if (!big_big_or(tmp_n, tmp_d)) {
            big_shift(&tmp_d, -1);
            step_count--;
        }
        for (int hz = 0; hz != step_count + 1; hz++) {
            if (!big_big_or(tmp_n, tmp_d)) {
                big_shift(&tmp_d, -1);
                if (hz != 0) {
                    big_shift(result, 1);
                }
                continue;
            }
            big_minus(tmp_n, tmp_d, &tmp_result);
            big_shift(result, 1);
            set_bigBit(result, 0);
            big_shift(&tmp_d, -1);
            tmp_n = tmp_result;
            to_zero(&tmp_result);
        }
    }
    return 1;
}

int clean_bigBit(big_decimal *d, int i) {
    int result = 1;
    unsigned int mask = 1;
    int val = i / 32;

    d->bits[val] = d->bits[val] & (~(mask << (i - (32 * val))));

    return result;
}

int cleanBit(s21_decimal *d, int i) {
    int result = 1;
    unsigned int mask = 1;

    if (i >= 0 && i <= 31) {
        d->bits[0] = d->bits[0] & (~(mask << i));
    } else if (i >= 32 && i <= 63) {
        d->bits[1] = d->bits[1] & (~(mask << (i - 32)));
    } else if (i >= 64 && i <= 95) {
        d->bits[2] = d->bits[2] & (~(mask << (i - 64)));
    } else {
        result = -1;
    }
    return result;
}

int set_bigBit(big_decimal *d, int i) {
    int result = 0;
    unsigned int mask = 1;
    int val = i / 32;
    d->bits[val] = d->bits[val] | (mask << (i - (32 * val)));

    return result;
}

int setBit(s21_decimal *d, int i) {
    int result = 1;
    unsigned int mask = 1;

    if (i >= 0 && i <= 31) {
        d->bits[0] = d->bits[0] | (mask << i);
    } else if (i >= 32 && i <= 63) {
        d->bits[1] = d->bits[1] | (mask << (i - 32));
    } else if (i >= 64 && i <= 95) {
        d->bits[2] = d->bits[2] | (mask << (i - 64));
    } else {
        result = -1;
    }
    return result;
}

int get_bigBit(big_decimal d, int i) {
    int result = 0;
    unsigned int mask = 1;
    int val = i / 32;
    if (d.bits[val] & (mask << (i - (32 * val)))) {
        result = 1;
    }
    return result;
}

int getBit(s21_decimal d, int i) {
    int result = 0;
    unsigned int mask = 1;

    if (i >= 0 && i <= 31) {
        if (d.bits[0] & (mask << i)) {
            result = 1;
        }
    } else if (i >= 32 && i <= 63) {
        if (d.bits[1] & (mask << (i - 32))) {
            result = 1;
        }
    } else if (i >= 64 && i <= 95) {
        if (d.bits[2] & (mask << (i - 64))) {
            result = 1;
        }
    } else {
        result = -1;
    }
    return result;
}

int big_mult(big_decimal a, big_decimal b, big_decimal *result) {
    big_decimal tmp_a = a;
    big_decimal zero = {0};
    *result = zero;

    for (int i = 0; i < 256; i++) {
        if (get_bigBit(b, i)) {
            big_decimal tmp_res = {0};

            big_plus(tmp_a, *result, &tmp_res);
            *result = tmp_res;
        }
        big_shift(&tmp_a, 1);
    }

    return 1;
}

int mult(s21_decimal a, s21_decimal b, s21_decimal *result) {
    s21_decimal tmp_a = a;
    s21_decimal tmp_res = {0};

    for (int i = 0; i < 96; i++) {
        if (getBit(b, i)) {
            tmp_res.bits[0] = 0;
            tmp_res.bits[1] = 0;
            tmp_res.bits[2] = 0;
            plus(tmp_a, *result, &tmp_res);
            *result = tmp_res;
        }
        shift(&tmp_a, 1);
    }

    return 1;
}

int big_plus(big_decimal a, big_decimal b, big_decimal *result) {
    int tmp = 0;
    big_decimal zero = {0};
    *result = zero;

    for (int i = 0; i < 256; i++) {
        if (get_bigBit(a, i) && get_bigBit(b, i) && tmp == 1) {
            tmp = 1;
            set_bigBit(result, i);
        } else if (get_bigBit(a, i) && get_bigBit(b, i) && tmp == 0) {
            tmp = 1;
        } else if ((get_bigBit(a, i) || get_bigBit(b, i)) && tmp == 1) {
            tmp = 1;
        } else if ((get_bigBit(a, i) || get_bigBit(b, i)) && tmp == 0) {
            set_bigBit(result, i);
        } else if ((!get_bigBit(a, i) && !get_bigBit(b, i)) && tmp == 1) {
            set_bigBit(result, i);
            tmp = 0;
        }
    }
    return 1;
}

int plus(s21_decimal a, s21_decimal b, s21_decimal *result) {
    int tmp = 0;

    for (int i = 0; i < 96; i++) {
        if (getBit(a, i) && getBit(b, i) && tmp == 1) {
            tmp = 1;
            setBit(result, i);
        } else if (getBit(a, i) && getBit(b, i) && tmp == 0) {
            tmp = 1;
        } else if ((getBit(a, i) || getBit(b, i)) && tmp == 1) {
            tmp = 1;
        } else if ((getBit(a, i) || getBit(b, i)) && tmp == 0) {
            setBit(result, i);
        } else if ((!getBit(a, i) && !getBit(b, i)) && tmp == 1) {
            setBit(result, i);
            tmp = 0;
        }
    }

    return 1;
}

int big_minus(big_decimal first, big_decimal second, big_decimal *result) {
    big_decimal d = {0};
    *result = d;
    if (big_not_null(second) == 1) {
        big_to_dop_code(second, &d);
    }
    big_plus(first, d, result);
    return 1;
}

int big_to_dop_code(big_decimal b, big_decimal *d) {
    big_decimal tmp = {0};
    tmp.bits[0] = ~(b.bits[0]);
    tmp.bits[1] = ~(b.bits[1]);
    tmp.bits[2] = ~(b.bits[2]);
    tmp.bits[3] = ~(b.bits[3]);
    tmp.bits[4] = ~(b.bits[4]);
    tmp.bits[5] = ~(b.bits[5]);
    tmp.bits[6] = ~(b.bits[6]);
    tmp.bits[7] = ~(b.bits[7]);
    big_decimal plus_one = {{1, 0, 0, 0, 0, 0, 0, 0}, 0};
    big_plus(plus_one, tmp, d);
    return 1;
}

int not_null(s21_decimal d) {
    int result = 0;
    for (int i = 0; i < 3; i++) {
        if (d.bits[i] != 0) {
            result = 1;
        }
    }
    return result;
}

int big_not_null(big_decimal d) {
    int result = 0;
    for (int i = 0; i < 8; i++) {
        if (d.bits[i] != 0) {
            result = 1;
        }
    }
    return result;
}

int shift(s21_decimal *d, int flag) {
    if (flag == 1) {
        for (int i = 96; i != 0; i--) {
            if (getBit(*d, i - 1)) {
                setBit(d, i);
            } else {
                cleanBit(d, i);
            }
        }
        cleanBit(d, 0);
    } else if (flag == -1) {
        for (int i = 0; i < 96; i++) {
            if (getBit(*d, i + 1)) {
                setBit(d, i);
            } else {
                cleanBit(d, i);
            }
        }
        cleanBit(d, 95);
    }
    return 1;
}

int big_shift(big_decimal *d, int flag) {
    if (flag == 1) {
        for (int i = 255; i != 0; i--) {
            if (get_bigBit(*d, i - 1)) {
                set_bigBit(d, i);
            } else {
                clean_bigBit(d, i);
            }
        }
        clean_bigBit(d, 0);
    } else if (flag == -1) {
        for (int i = 0; i < 256; i++) {
            if (get_bigBit(*d, i + 1)) {
                set_bigBit(d, i);
            } else {
                clean_bigBit(d, i);
            }
        }
        clean_bigBit(d, 255);
    }
    return 1;
}

int big_big_small(big_decimal a, big_decimal b) {
    int result = 0;
    for (int i = 255; i != -1; i--) {
        if (get_bigBit(a, i) != get_bigBit(b, i)) {
            if (get_bigBit(a, i) == 1) {
                result = 1;
            }
            break;
        }
    }
    return result;
}

int big_big_or(big_decimal a, big_decimal b) {
    int result = 1;
    for (int i = 255; i != -1; i--) {
        if (get_bigBit(a, i) != get_bigBit(b, i)) {
            if (get_bigBit(a, i) == 0) {
                result = 0;
            }
            break;
        }
    }
    return result;
}

int get_bigCount(big_decimal a) {
    int count;
    for (int i = 255; i != -1; i--) {
        if (get_bigBit(a, i)) {
            count = i;
            break;
        }
    }
    return count;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    for (int i = 0; i < 4; i++) dst->bits[i] = 0;
    if (src < 0) {
        src *= -1;
        setSign(dst);
    }
    int scale = 0, res = 0, exp = getExp(src);
    if ((src > 0. && src < 1e-28) || src == INFINITY || is_nan(src) || exp > 95 ||
        exp < -95) {
        res = 1;
    } else {
        if (exp > -95 && exp < 0) {
            for (; !((int) src); src *= 10, scale++) {
            }
            for (int i = AFTER_DOT; i != 0; src *= 10, scale++, i--) {
            }
            exp = getExp(src);
            unsigned int fbits = *((unsigned *) &src), mask = 0x400000;
            setBit(dst, exp);
            for (int j = exp - 1; mask; mask >>= 1, j--)
                if (fbits & mask) setBit(dst, j);
            if (scale >= 28 + AFTER_DOT + 3) res = 1;
            setScale(dst, scale);
        } else if (exp == 0 || exp > 0) {
            s21_decimal tmp_d_first = {0}, tmp_d_second = {0},
                    for_mult = {{pow(10, AFTER_DOT), 0, 0, 0}}, tmp_result = {0};
            float tmp_first = (float) ((int) src);
            float tmp_second = src - tmp_first;
            unsigned int fbits, mask = 0x400000;
            if (tmp_second) {
                for (int i = AFTER_DOT; i != 0; tmp_second *= 10, i--) {
                }
                exp = getExp(tmp_second);
                setBit(&tmp_d_second, exp);
                fbits = *((unsigned *) &tmp_second);
                for (int j = exp - 1; mask; mask >>= 1, j--)
                    if (fbits & mask) setBit(&tmp_d_second, j);
            }
            unsigned int u_tmp_first = (unsigned int) tmp_first;
            fbits = u_tmp_first, mask = 0x80000000;
            for (int j = 31; mask; mask >>= 1, j--)
                if (fbits & mask) setBit(&tmp_d_first, j);
            mult(for_mult, tmp_d_first, &tmp_result);
            plus(tmp_result, tmp_d_second, dst);
            if (tmp_second) {
                setScale(dst, AFTER_DOT);
            } else {
                s_down(dst, AFTER_DOT, dst);
                clean_Scale(dst);
            }
        }
    }
    return res;
}

int getExp(float f) {
    unsigned int fbits = *((unsigned int *) &f);
    int res = 0;
    fbits <<= 1;
    res = res | (fbits >> 24);
    return res - 127;
}

int big_setScale(big_decimal *dst, int scale) {
    unsigned int mask = 0;
    mask = mask | scale;
    dst->opt = dst->opt | (mask << 16);
    return 1;
}

int clean_bigScale(big_decimal *dc) {
    unsigned int mask = 0x10000000;
    dc->opt = dc->opt & mask;
    return 1;
}

int clean_Scale(s21_decimal *dc) {
    unsigned int mask = 0x10000000;
    dc->bits[3] = dc->bits[3] & mask;
    return 1;
}

int setScale(s21_decimal *dst, int scale) {
    unsigned int mask = 0;
    mask = mask | scale;
    dst->bits[3] = dst->bits[3] | (mask << 16);
    return 1;
}

int big_getScale(big_decimal src) {
    src.opt <<= 8;
    unsigned int res = src.opt >> 24;
    return res;
}

int getScale(s21_decimal src) {
    src.bits[3] <<= 8;
    unsigned int res = src.bits[3] >> 24;
    return res;
}

int set_bigSign(big_decimal *dst) {
    dst->opt |= 0x80000000;
    return 1;
}

int setSign(s21_decimal *dst) {
    dst->bits[3] |= 0x80000000;
    return 1;
}

int get_bigSign(big_decimal dst) { return dst.opt & 0x80000000 ? 1 : 0; }

int getSign(s21_decimal dst) { return dst.bits[3] & 0x80000000 ? 1 : 0; }

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    double tmp = 0;

    *dst = 0.0;
    for (int i = 0; i < 96; i++) {
        tmp *= 2;
        if (getBit(src, 95 - i)) {
            tmp += 1;
        }
    }
    for (int j = getScale(src); j != 0; tmp /= 10, j--) {
    }

    if (getSign(src)) {
        tmp *= -1;
    }
    *dst = tmp;
    return 0;
}

int big_to_dec(big_decimal big_d, s21_decimal *d) {  // !!!!!!! ИСПРАВИЛ ФУНКЦИЮ
    int error = 0;
    int scale = big_getScale(big_d);
    int for_bank = 0;
    big_decimal tmp = big_d;
    big_decimal tmp_result = {0};

    if (scale <= 28 && tmp.bits[3] == 0 && tmp.bits[4] == 0 && tmp.bits[5] == 0 &&
        tmp.bits[6] == 0 && tmp.bits[7] == 0) {
        d->bits[0] = tmp.bits[0];
        d->bits[1] = tmp.bits[1];
        d->bits[2] = tmp.bits[2];
        d->bits[3] = tmp.opt;
    } else {
        while (scale && (scale > 28 || tmp.bits[3] || tmp.bits[4] || tmp.bits[5] ||
                         tmp.bits[6] || tmp.bits[7])) {
            scale_down(&tmp, 1, &tmp_result);
            tmp = tmp_result;
            for_bank = tmp_result.bits[0] % 10;
            to_zero(&tmp_result);
            scale--;
        }
        if (for_bank > 5 || (for_bank == 5 && get_bigBit(tmp, 0))) {
            big_decimal tmp_bank = {0};
            big_decimal tmp_bank2 = {{1, 0, 0, 0, 0, 0, 0, 0}, 0};
            big_plus(tmp, tmp_bank2, &tmp_bank);
            tmp_bank.opt = tmp.opt;
            tmp = tmp_bank;
        }

        if (tmp.bits[3] == 0 && tmp.bits[4] == 0 && tmp.bits[5] == 0 &&
            tmp.bits[6] == 0 && tmp.bits[7] == 0) {
            d->bits[0] = tmp.bits[0];
            d->bits[1] = tmp.bits[1];
            d->bits[2] = tmp.bits[2];
            d->bits[3] = tmp.opt;
        } else {
            if (get_bigSign(big_d))
                error = 2;
            else
                error = 1;
        }
    }
    return error;
}

int dec_to_big(big_decimal *big_d, s21_decimal d) {
    big_d->bits[0] = d.bits[0];
    big_d->bits[1] = d.bits[1];
    big_d->bits[2] = d.bits[2];
    big_d->opt = d.bits[3];
    return 1;
}

int to_zero(big_decimal *d) {
    for (int i = 0; i < 8; i++) {
        d->bits[i] = 0;
    }
    return 0;
}

int to_z(s21_decimal *d) {
    for (int i = 0; i < 3; i++) {
        d->bits[i] = 0;
    }
    return 0;
}

int div(s21_decimal n, s21_decimal d, s21_decimal *result) {
    s21_decimal tmp_n = n;
    s21_decimal tmp_d = d;
    s21_decimal tmp_result = {0};
    int count_n = 0;
    int count_d = 0;
    int step_count = 0;
    count_n = getCount(n);
    count_d = getCount(d);
    step_count = count_n - count_d;
    for (int i = 0; i < count_n - count_d; i++) {
        shift(&tmp_d, 1);
    }
    if (!big_or(tmp_n, tmp_d)) {
        shift(&tmp_d, -1);
        step_count--;
    }
    for (int hz = 0; hz != step_count + 1; hz++) {
        if (!big_or(tmp_n, tmp_d)) {
            shift(&tmp_d, -1);
            if (hz != 0) {
                shift(result, 1);
            }
            continue;
        }
        minus(tmp_n, tmp_d, &tmp_result);
        shift(result, 1);
        setBit(result, 0);
        shift(&tmp_d, -1);
        tmp_n = tmp_result;
        tmp_result.bits[0] = 0;
        tmp_result.bits[1] = 0;
        tmp_result.bits[2] = 0;
    }

    return 1;
}

int s_down(s21_decimal *bd, int val, s21_decimal *result) {
    s21_decimal bd_for_mult = {{10, 0, 0, 0}};
    s21_decimal bd_tmp_result = {0};
    for (int i = 0; i < val; i++) {
        div(*bd, bd_for_mult, &bd_tmp_result);
        *bd = bd_tmp_result;
        to_z(&bd_tmp_result);
    }
    *result = *bd;
    clean_Scale(result);
    setScale(result, val);

    return 0;
}

int minus(s21_decimal a, s21_decimal b, s21_decimal *result) {
    s21_decimal d = {0};
    if (not_null(b) == 1) {
        to_dop_code(b, &d);
    }
    plus(a, d, result);
    return 1;
}

int to_dop_code(s21_decimal b, s21_decimal *d) {
    s21_decimal tmp = {0};
    tmp.bits[0] = ~b.bits[0];
    tmp.bits[1] = ~b.bits[1];
    tmp.bits[2] = ~b.bits[2];
    s21_decimal plus_one = {{1, 0, 0, 0}};
    plus(plus_one, tmp, d);
    return 1;
}

int getCount(s21_decimal a) {
    int count;
    for (int i = 95; i != -1; i--) {
        if (getBit(a, i)) {
            count = i;
            break;
        }
    }
    return count;
}

int big_or(s21_decimal a, s21_decimal b) {
    int result = 1;
    for (int i = 95; i != -1; i--) {
        if (getBit(a, i) != getBit(b, i)) {
            if (getBit(a, i) == 0) {
                result = 0;
            }
            break;
        }
    }
    return result;
}

int scale_down(big_decimal *bd, int val, big_decimal *result) {
    big_decimal bd_for_mult = {{10, 0, 0, 0, 0, 0, 0, 0}, 0};
    big_decimal bd_tmp_result = {0};
    int sign = get_bigSign(*bd);
    int old_scale = big_getScale(*bd);
    for (int i = 0; i < val; i++) {
        big_div(*bd, bd_for_mult, &bd_tmp_result);
        *bd = bd_tmp_result;
        to_zero(&bd_tmp_result);
    }
    *result = *bd;
    clean_bigScale(result);
    big_setScale(result, old_scale - val);
    if (sign) {
        set_bigSign(result);
    }

    return 0;
}

int scale_up(big_decimal *bd, int val, big_decimal *result) {
    big_decimal bd_for_mult = {{10, 0, 0, 0, 0, 0, 0, 0}, 0};
    big_decimal bd_tmp_result = {0};
    int sign = get_bigSign(*bd);
    int old_scale = big_getScale(*bd);
    for (int i = 0; i < val; i++) {
        big_mult(*bd, bd_for_mult, &bd_tmp_result);
        *bd = bd_tmp_result;
        to_zero(&bd_tmp_result);
    }
    *result = *bd;
    clean_bigScale(result);
    big_setScale(result, old_scale + val);
    if (sign) {
        set_bigSign(result);
    }
    return 0;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    int result = 0;
    big_decimal bd1 = {0};
    big_decimal bd2 = {0};
    to_one_scale(&value_1, &value_2, &bd1, &bd2);
    int sign_1 = get_bigSign(bd1);
    int sign_2 = get_bigSign(bd2);
    if (!big_not_null(bd1) && !big_not_null(bd2)) {
        result = 0;
    } else if (sign_1 && !sign_2) {
        result = 1;
    } else if (!sign_1 && sign_2) {
        result = 0;
    } else if (sign_1 == sign_2) {
        for (int i = 255; i > -1; i--) {
            int bit1 = get_bigBit(bd1, i);
            int bit2 = get_bigBit(bd2, i);
            if (bit1 != bit2) {
                if (!sign_1 && !bit1) {
                    result = 1;
                } else if (!sign_1) {
                    result = 0;
                } else if (sign_1 && bit1) {
                    result = 1;
                } else if (sign_1 && !bit1) {
                    result = 0;
                }
                break;
            }
        }
    }
    return result;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
    return s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    return !s21_is_less(value_1, value_2) && !s21_is_equal(value_1, value_2);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    return !s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    int result = 1;
    big_decimal bd1 = {0};
    big_decimal bd2 = {0};
    to_one_scale(&value_1, &value_2, &bd1, &bd2);
    int sign_1 = get_bigSign(bd1);
    int sign_2 = get_bigSign(bd2);
    if (!big_not_null(bd1) && !big_not_null(bd2)) {
        result = 1;
    } else if (sign_1 != sign_2) {
        result = 0;
    } else {
        for (int i = 0; i < 256; i++) {
            if (get_bigBit(bd1, i) != get_bigBit(bd2, i)) {
                result = 0;
                break;
            }
        }
    }
    return result;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    return !s21_is_equal(value_1, value_2);
}

int s21_floor(s21_decimal value, s21_decimal *dst) {
    int result = 0;
    int scale = getScale(value);
    if (scale > 28) {
        result = 1;
    } else if (scale) {
        s21_truncate(value, dst);
        if (getSign(value) && (value.bits[0] || value.bits[1] || value.bits[2])) {
            s21_decimal one = {{1, 0, 0, 0x80000000}};
            s21_add(*dst, one, dst);
        }
    } else {
        *dst = value;
    }
    return result;
}

int s21_round(s21_decimal value, s21_decimal *dst) {
    int result = 0;
    int scale = getScale(value);
    if (scale > 28) {
        result = 1;
    } else if (scale) {
        s21_decimal five = {{5, 0, 0, 0x00010000}};
        if (getSign(value)) {
            five.bits[3] = 0x80010000;
        }
        s21_add(value, five, dst);
        s21_truncate(*dst, dst);
    } else {
        *dst = value;
    }
    return result;
}

int s21_truncate(s21_decimal value, s21_decimal *dst) {
    int result = 0;
    int scale = getScale(value);
    if (scale > 28) {
        result = 1;
    } else if (scale) {
        big_decimal big = {0};
        dec_to_big(&big, value);
        big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}, 0};
        while (scale) {
            big_decimal temp = {0};
            big_div(big, ten, &temp);
            big = temp;
            scale--;
        }
        clean_bigScale(&big);
        big_to_dec(big, dst);
        if (getSign(value)) setSign(dst);
    } else {
        *dst = value;
    }
    return result;
}

int s21_negate(s21_decimal value, s21_decimal *dst) {
    int result = 0;
    int scale = getScale(value);
    if (scale > 28) {
        result = 1;
    } else {
        if (getSign(value)) {
            cleanSign(&value);
        } else {
            setSign(&value);
        }
        *dst = value;
    }
    return result;
}

void cleanSign(s21_decimal *value) {
    value->bits[3] &= 0b01111111111111111111111111111111;
}

unsigned char *p_bite(s21_decimal *p_value, int *index) {
    int index_bite = 0;
    int index_i = (++(*index) % 32) ? *index / 32 : *index / 32 - 1;
    void *data = (void *) (&p_value->bits[index_i]);
    *index = *index - 1 - 32 * index_i;
    while (*index > 7) *index -= 8, index_bite++;
    return ((unsigned char *) data) + index_bite;
}

char read_bit(s21_decimal *p_value, int index) {
    unsigned char tmp = 1, *p_tmp = p_bite(p_value, &index);
    return (char) (((tmp << (index)) & (*p_tmp)) >> (index));
}

void change_bit(s21_decimal *p_value, int index, unsigned char bit) {
    int tmp = index;
    unsigned char *p_tmp = p_bite(p_value, &index);
    *p_tmp = (read_bit(p_value, tmp) && !bit) ? *p_tmp & (~(++bit << index))
                                              : *p_tmp | (bit << index);
}

int get_sign(s21_decimal *p_value) { return read_bit(p_value, 127); }

int get_exp(s21_decimal *p_value) {
    int index = 119;
    return *p_bite(p_value, &index);
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    dst->bits[0] = src;
    dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    if (read_bit(dst, 31)) {
        dst->bits[0] = (~dst->bits[0]) + 1;
        change_bit(dst, 127, 1);
    }
    return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int rez = 0, exp = get_exp(&src);
    if (exp > 28) {
        rez = 1;
    } else if (not_null(src)) {
        big_decimal tmp = {0}, rezult = {0};
        dec_to_big(&tmp, src);
        scale_down(&tmp, exp, &rezult);
        big_to_dec(rezult, &src);
    }
    for (int i = 32; i < 96; i++) {
        if (read_bit(&src, i)) {
            rez = 1;
            break;
        }
    }
    if (!rez) {
        for (int i = 31; i >= 0; i--) {
            *dst <<= 1;
            *dst += read_bit(&src, i);
        }
        if (get_sign(&src)) *dst = ~(*dst - 1);
    }
    return rez;
}
