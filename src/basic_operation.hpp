#include "../header/common.h"
#include "../header/CR.h"
#include "../header/tcp_connection.h"

using namespace std;

// Ohata and Nuida, FC 2020

// !基本的にベクトル処理もしているが，data_type.cppでの各演算子オーバーロードでは長さ1で処理している，
// integer_t が unsigned long longであることを想定

// todo Reconstは場所を変えたほうがいいかも
void Reconst(integer_t *val,int numlen, integer_t *ans);
void Reconst(bool *val,int numlen, bool *ans);
// 乗算
void multiplication(integer_t *val1, integer_t *val2, int numlen, integer_t *ans);
void multiplication(integer_t val1, integer_t val2, integer_t &ans);
void multiplication(bool *val1, bool *val2, int numlen, bool *ans);
void multiplication(bool val1, bool val2, bool &ans);

void ex_multiplication(integer_t *val, int numlen, integer_t *ans);
void ex_multiplication(bool *val, int numlen, integer_t *ans);

// 他入力乗算
void multi_fan_in_mult(integer_t **vals, int numlen, int fan_num, integer_t *ans);
void multi_fan_in_mult(integer_t *vals, int fan_num, integer_t &ans);

void multi_fan_in_and(bool **vals, int numlen, int fan_num, bool *ans);
void multi_fan_in_and(bool *vals, int fan_num, bool &ans);
void multi_fan_in_and(bool **vals, int numlen, int *fan_num, bool *ans);

void multi_fan_in_or(bool **vals, int numlen, int fan_num, bool *ans);
void multi_fan_in_or(bool *vals,int fan_num, bool &ans);
void multi_fan_in_or(bool **vals, int numlen, int *fan_num, bool *ans);


void B2A(bool *val, int numlen, integer_t *ans);
void B2A(bool val, integer_t &ans);

// 等号判定(0に等しいかどうか)
void equal_zero(integer_t *val,int numlen, bool *ans);
void equal_zero(integer_t val, bool &ans);
void equal_zero(integer_t *val,int numlen, integer_t *ans);
void equal_zero(integer_t val, integer_t &ans);

// 等号判定
void equality(integer_t *val1,integer_t *val2, int numlen, bool *ans);
void equality(integer_t val1,integer_t val2, bool &ans);
void equality(integer_t *val1,integer_t *val2, int numlen, integer_t *ans);
void equality(integer_t val1,integer_t val2, integer_t &ans);

// 大小比較(<)
void lessthan(integer_t *val1,integer_t *val2, int numlen, bool *ans);
void lessthan(integer_t val1,integer_t val2, bool &ans);
void lessthan(integer_t *val1,integer_t *val2, int numlen, integer_t *ans);
void lessthan(integer_t val1,integer_t val2, integer_t &ans);

// 大小比較(>)
void greaterthan(integer_t *val1,integer_t *val2,int numlen, bool *ans);
void greaterthan(integer_t val1,integer_t val2, bool &ans);
void greaterthan(integer_t *val1,integer_t *val2,int numlen, integer_t *ans);
void greaterthan(integer_t val1,integer_t val2, integer_t &ans);

// オーバーフロー
void overflow(integer_t *val, int numlen, int mod_bit, bool *ans);
void overflow(integer_t val,int mod_bit, bool &ans);
void overflow(integer_t *val, int numlen, int mod_bit, integer_t *ans);
void overflow(integer_t val,int mod_bit, integer_t &ans);

// 右シフト
void rightshift(integer_t *val, int numlen, int shift, integer_t *ans);
void rightshift(integer_t val, int shift, integer_t &ans);

// ビット取り出し
void extractbit(integer_t *val, int numlen, int bit_pos, bool *ans);
void extractbit(integer_t val, int bit_pos, bool &ans);
void extractbit(integer_t *val, int numlen, int bit_pos, integer_t *ans);
void extractbit(integer_t val, int bit_pos, integer_t &ans);

// 最上位ビットの取り出し
void MSB(integer_t *val, int numlen, bool *ans);
void MSB(integer_t val, bool &ans);
void MSB(integer_t *val, int numlen, integer_t *ans);
void MSB(integer_t val, integer_t &ans);