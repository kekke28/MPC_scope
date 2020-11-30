#ifndef CRFILE
#define CRFILE

#include <wmmintrin.h>
#include <iostream>
#include "./common.h"
#include <unordered_map>
#include <string>

using namespace std;

inline __m128i AES_128_ASSIST (__m128i temp1, __m128i temp2){
    __m128i temp3;
    temp2 = _mm_shuffle_epi32 (temp2 ,0xff);
    temp3 = _mm_slli_si128 (temp1, 0x4);
    temp1 = _mm_xor_si128 (temp1, temp3);
    temp3 = _mm_slli_si128 (temp3, 0x4);
    temp1 = _mm_xor_si128 (temp1, temp3);
    temp3 = _mm_slli_si128 (temp3, 0x4);
    temp1 = _mm_xor_si128 (temp1, temp3);
    
    temp1 = _mm_xor_si128 (temp1, temp2);
    return temp1;
}
inline void AES_128_Key_Expansion (const unsigned char *userkey,unsigned char *key){
    __m128i temp1, temp2;
    __m128i *Key_Schedule = (__m128i*)key;

    temp1 = _mm_loadu_si128((__m128i*)userkey);
    Key_Schedule[0] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1 ,0x1);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[1] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x2);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[2] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x4);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[3] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x8);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[4] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x10);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[5] = temp1; 
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x20);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[6] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x40);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[7] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x80);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[8] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x1b);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[9] = temp1;
    temp2 = _mm_aeskeygenassist_si128 (temp1,0x36);
    temp1 = AES_128_ASSIST(temp1, temp2);
    Key_Schedule[10] = temp1;
}
inline void AES_ECB_encrypt(const unsigned char *in, unsigned char *out, unsigned long length,const unsigned char *key,int number_of_rounds){
    __m128i tmp;
    int i,j;
    if(length%16)
    length = length/16+1;
    else
    length = length/16;
    for(i=0; i < length; i++){
        tmp = _mm_loadu_si128 (&((__m128i*)in)[i]);
        tmp = _mm_xor_si128 (tmp,((__m128i*)key)[0]);
        for(j=1; j <number_of_rounds; j++){
            tmp = _mm_aesenc_si128 (tmp,((__m128i*)key)[j]);
        }
        tmp = _mm_aesenclast_si128 (tmp,((__m128i*)key)[j]);
    _mm_storeu_si128 (&((__m128i*)out)[i],tmp);
    }
}

struct rand_seed{
    unsigned char *id;
    unsigned char *key;
    unsigned long long num_call_AES;
    
    rand_seed(unsigned char *userkey);
    ~rand_seed();
    void update();
    void next_rand(unsigned char *out);
};

void gen_random_m(rand_seed &seed, integer_t *ans,int num,integer_t p, int num_char);

void gen_random(rand_seed &seed,integer_t *ans,int num,int bit_length = INTEGER_BIT);

void bool_gen_random(rand_seed &seed,bool *ans,int num);

void BT_gen(rand_seed &seed,integer_t *ans1, integer_t *ans2, int num,int fan_num);

void bool_BT_gen(rand_seed &seed, bool* ans1, bool* ans2,int num, int fan_num);

void BT_gen_p(rand_seed &seed,integer_t *ans1, integer_t *ans2,int num,int fan_num,integer_t p, int num_char);

void ex_BT_gen(rand_seed &seed,integer_t *ans1, integer_t *ans2,int num);

void ex_bool_BT_gen(rand_seed &seed,bool *ans1,bool *ans2,int num);

void ex_BT_gen_p(rand_seed &seed, integer_t *ans1, integer_t *ans2, int num, integer_t p, int num_char);

bool CR_gen_by_server(rand_seed &seed);// 終わったらfalseを返す

struct BT_Segment{
    // 1つタイプのBTを保存する構造体
    // 1つのタイプ，とは{mod 2^INTEGER_BIT / mod p / mod 2}とfan_numが同一のものを指す．
    int mod_type; // 1 : mod 2^INTEGER_BIT, 2 : mod p, 3 : mod 2
    integer_t p;
    int fan_num;
    bool is_ex;

    int length;// 一つのCRの(配列上の)長さ．
    int max_store;// 最大何個のBTを蓄えるか.無くなったらまたmax_store分蓄える
    int counter;// 次何番目を使うか

    integer_t *cr;
    bool *bool_cr;
    
    // ! max_storeは規定値100にしている
    BT_Segment(){};
    ~BT_Segment();

    void init(int _mod_type,integer_t _p,int _fan_num,bool _is_ex,int _max_store=100);
    void supply();
    void call(bool *&aux);
    void call(integer_t *&aux);
};

struct BT_Store{
    // BT_Segmentの集まり
    // BT_Segmentのtype(mod,is_ex,fan_numとか)をtagとしたunordered_mapで管理
    unordered_map<unsigned long long,BT_Segment> store;
    BT_Store();
    ~BT_Store();
    unsigned long long to_type(int mod_type,integer_t p,int fan_num,bool is_ex);
    void call(int mod_type,integer_t p,int fan_num,bool is_ex,bool *&aux);
    void call(int mod_type,integer_t p,int fan_num,bool is_ex,integer_t *&aux);
    void program_finish();// もうこれ以降プログラムがないときにserverに終わったことを伝える．
};

extern BT_Store CR; // CR.cppで定義

#endif