#ifndef COMMON
#define COMMON

#define PARTY_NUM 3
#define MAXPENDING 5

// ! party_id : 0はtrusted initializerであることを仮定
// party_idは実行時に指定することを想定しているので，main関数で定義

extern int party_id; // main関数で定義
extern unsigned long long send_bytes[PARTY_NUM+1]; // tcp_connection,cppで定義
extern unsigned long long receive_bytes[PARTY_NUM+1]; // tcp_connection,cppで定義

typedef unsigned long long int integer_t;
#define INTEGER_BIT 64
#define INTEGER_NUM_CHAR 8

// #define INTEGER_BIT 32
// #define INTEGER_NUM_CHAR 4
// typedef unsigned int integer_t;


#endif