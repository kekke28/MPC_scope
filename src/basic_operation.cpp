#include "basic_operation.hpp"

using namespace std;

void Reconst(integer_t *val,int numlen, integer_t *ans){
    if(party_id==1){
        tcp[2].send_nums(val,numlen);
        tcp[2].receive_nums(ans,numlen);
    }else{
        tcp[1].send_nums(val,numlen);
        tcp[1].receive_nums(ans,numlen);
    }
    for(int i=0;i<numlen;i++) ans[i] += val[i];
}
void Reconst(bool *val,int numlen, bool *ans){
    if(party_id==1){
        tcp[2].send_nums(val,numlen);
        tcp[2].receive_nums(ans,numlen);
    }else{
        tcp[1].send_nums(val,numlen);
        tcp[1].receive_nums(ans,numlen);
    }
    for(int i=0;i<numlen;i++) ans[i] ^= val[i];
}

void multiplication(integer_t *val1, integer_t *val2, int numlen, integer_t *ans){
    // CRのtype
    int mod_type = 1;
    integer_t p = 0;
    int fan_num = 2;
    bool is_ex = false;

    integer_t *comm_vals = new integer_t[2*numlen];
    integer_t *aux_store = new integer_t[3*numlen];
    integer_t *aux;
    for(int i=0;i<numlen;i++){
        CR.call(mod_type,p,fan_num,is_ex,aux);
        comm_vals[i]        = val1[i] - aux[0];
        comm_vals[numlen+i] = val2[i] - aux[1];
        aux_store[3*i+0] = aux[0];
        aux_store[3*i+1] = aux[1];
        aux_store[3*i+2] = aux[2];
    }
    if(party_id == 1){        
        tcp[2].send_nums(comm_vals,2*numlen);
        tcp[2].receive_nums(comm_vals,2*numlen);

        for(int i=0;i<numlen;i++){
            integer_t x_prime = comm_vals[i]        + val1[i] - aux_store[3*i+0];
            integer_t y_prime = comm_vals[numlen+i] + val2[i] - aux_store[3*i+1];
            ans[i] = x_prime * y_prime + aux_store[3*i+0] * y_prime + aux_store[3*i+1] * x_prime + aux_store[3*i+2];
        }
    }else{
        tcp[1].send_nums(comm_vals,2*numlen);
        tcp[1].receive_nums(comm_vals,2*numlen);

        for(int i=0;i<numlen;i++){
            integer_t x_prime = comm_vals[i]        + val1[i] - aux_store[3*i+0];
            integer_t y_prime = comm_vals[numlen+i] + val2[i] - aux_store[3*i+1];
            ans[i] = aux_store[3*i+0] * y_prime + aux_store[3*i+1] * x_prime + aux_store[3*i+2];
        }
    }
    delete[] comm_vals;
    delete[] aux_store;
}
void multiplication(integer_t val1, integer_t val2, integer_t &ans){
    multiplication(&val1,&val2,1,&ans);
}
void multiplication(bool *val1, bool *val2, int numlen, bool *ans){
    // CRのtype
    int mod_type = 3;
    integer_t p = 0;
    int fan_num = 2;
    bool is_ex = false;

    bool *comm_vals = new bool[2*numlen];
    bool *aux_store = new bool[3*numlen];
    bool *aux;
    
    for(int i=0;i<numlen;i++){
        CR.call(mod_type,p,fan_num,is_ex,aux);
        comm_vals[i]        = val1[i] ^ aux[0];
        comm_vals[numlen+i] = val2[i] ^ aux[1];
        aux_store[3*i+0] = aux[0];
        aux_store[3*i+1] = aux[1];
        aux_store[3*i+2] = aux[2];
    }
    if(party_id == 1){
        tcp[2].send_nums(comm_vals,2*numlen);
        tcp[2].receive_nums(comm_vals,2*numlen);

        for(int i=0;i<numlen;i++){
            bool x_prime = comm_vals[i]        ^ val1[i] ^ aux_store[3*i+0];
            bool y_prime = comm_vals[numlen+i] ^ val2[i] ^ aux_store[3*i+1];
            ans[i] = x_prime & y_prime ^ aux_store[3*i+0] & y_prime ^ aux_store[3*i+1] & x_prime ^ aux_store[3*i+2];
        }
    }else{
        tcp[1].send_nums(comm_vals,2*numlen);
        tcp[1].receive_nums(comm_vals,2*numlen);

        for(int i=0;i<numlen;i++){
            bool x_prime = comm_vals[i]        ^ val1[i] ^ aux_store[3*i+0];
            bool y_prime = comm_vals[numlen+i] ^ val2[i] ^ aux_store[3*i+1];
            ans[i] = aux_store[3*i+0] & y_prime ^ aux_store[3*i+1] & x_prime ^ aux_store[3*i+2];
        }
    }
    delete[] comm_vals;
    delete[] aux_store;
}
void multiplication(bool val1, bool val2, bool &ans){
    multiplication(&val1,&val2,1,&ans);
}


void ex_multiplication(integer_t *val, int numlen, integer_t *ans){
    // ans = (party 1のval) * (party 2のval)

    // CRのtype
    int mod_type = 1;
    integer_t p = 0;
    int fan_num = 2;
    bool is_ex = true;

    integer_t *aux;
    integer_t *aux_store = new integer_t[2*numlen];
    integer_t *comm_val = new integer_t[numlen];
    for(int i=0;i<numlen;i++){
        CR.call(mod_type,p,fan_num,is_ex,aux);
        comm_val[i] = val[i] - aux[0];
        aux_store[i*2 + 0] = aux[0];
        aux_store[i*2 + 1] = aux[1];
    }
    if(party_id == 1){
        tcp[2].send_nums(comm_val,numlen);
        tcp[2].receive_nums(comm_val,numlen);

        for(int i=0;i<numlen;i++) ans[i] = val[i] * comm_val[i] + aux_store[2*i+1];
    }else{
        tcp[1].send_nums(comm_val,numlen);
        tcp[1].receive_nums(comm_val,numlen);
        for(int i=0;i<numlen;i++) ans[i] = aux_store[2*i+0] * comm_val[i] + aux_store[2*i+1];
    }
    delete[] aux_store;
    delete[] comm_val;    

}
void ex_multiplication(bool *val, int numlen, integer_t *ans){
    // ans = (party 1のval) * (party 2のval)

    // CRのtype
    int mod_type = 1;
    integer_t p = 0;
    int fan_num = 2;
    bool is_ex = true;

    integer_t *aux;
    integer_t *aux_store = new integer_t[2*numlen];
    integer_t *comm_val = new integer_t[numlen];
    for(int i=0;i<numlen;i++){
        CR.call(mod_type,p,fan_num,is_ex,aux);
        comm_val[i] = (integer_t)val[i] - aux[0];
        aux_store[i*2 + 0] = aux[0];
        aux_store[i*2 + 1] = aux[1];
    }
    if(party_id == 1){
        tcp[2].send_nums(comm_val,numlen);
        tcp[2].receive_nums(comm_val,numlen);
        for(int i=0;i<numlen;i++) ans[i] = val[i] * comm_val[i] + aux_store[2*i+1];
    }else{
        tcp[1].send_nums(comm_val,numlen);
        tcp[1].receive_nums(comm_val,numlen);
        for(int i=0;i<numlen;i++) ans[i] = aux_store[2*i+0] * comm_val[i] + aux_store[2*i+1];
    }
    delete[] aux_store;
    delete[] comm_val;    

}

void multi_fan_in_mult(integer_t **vals, int numlen, int fan_num, integer_t *ans){
    // valのサイズはnumlen * fan_num
    // ans[i] = vals[i][0]*vals[i][1]*...*vals[i][fan_num-1]
    // CRのtype
    int mod_type = 1;
    integer_t p = 0;
    bool is_ex = false;

    integer_t **prod = new integer_t*[numlen];
    for(int i=0;i<numlen;i++) prod[i] = new integer_t[1<<fan_num];
    integer_t *comm_val = new integer_t[numlen*fan_num]; // 通信に使う配列なので1次元配列にしている
    integer_t **xs_prime = new integer_t*[numlen];
    for(int i=0;i<numlen;i++) xs_prime[i] = new integer_t[fan_num];
    integer_t *aux;
    integer_t **aux_store = new integer_t*[numlen];
    for(int i=0;i<numlen;i++) aux_store[i] = new integer_t[1<<fan_num];

    // comm_valの計算
    for(int i=0;i<numlen;i++){
        CR.call(mod_type,p,fan_num,is_ex,aux);
        for(int j=0;j<fan_num;j++) comm_val[i*fan_num + j] = vals[i][j] - aux[(1<<j)-1];
        // auxの保存
        for(int j=0;j<(1<<fan_num)-1;j++) aux_store[i][j] = aux[j];
    }
    if(party_id == 1) {tcp[2].send_nums(comm_val,numlen*fan_num);tcp[2].receive_nums(comm_val,numlen*fan_num);}
    else              {tcp[1].send_nums(comm_val,numlen*fan_num);tcp[1].receive_nums(comm_val,numlen*fan_num);}

    // xs_primeの計算(x-aみたいなのに対応する値)
    for(int i=0;i<numlen;i++) for(int j=0;j<fan_num;j++) xs_prime[i][j] = comm_val[i*fan_num+j] + vals[i][j]- aux_store[i][(1<<j)-1];
    
    // prodの計算
    for(int i=0;i<numlen;i++){
        prod[i][0] = (integer_t) 1;
        for(int j=1;j<(1<<fan_num);j++){
            for(int k=0;k<fan_num;k++){
                if((j>>k)&1){
                    prod[i][j] = prod[i][j-(1<<k)] * xs_prime[i][k]; 
                }
            }
        }
    }

    if(party_id == 1) for(int i=0;i<numlen;i++) ans[i] = prod[i][(1<<fan_num)-1];
    else              for(int i=0;i<numlen;i++) ans[i] = 0;

    int all_one_bit = (1<<fan_num) - 1;
    for(int i=0;i<numlen;i++) for(int j=1;j<(1<<fan_num);j++) ans[i] += aux_store[i][j-1]*prod[i][all_one_bit^j]; 

    for(int i=0;i<numlen;i++) delete[] prod[i];
    delete[] prod;
    delete[] comm_val;
    for(int i=0;i<numlen;i++) delete[] xs_prime[i];
    delete[] xs_prime;
    for(int i=0;i<numlen;i++) delete[] aux_store[i];
    delete[] aux_store;
}
void multi_fan_in_mult(integer_t *vals, int fan_num, integer_t &ans){
    // valのサイズはfan_num
    // ans = vals[0]*...*vals[fan_num-1]
    
    // CRのtype
    int mod_type = 1;
    integer_t p = 0;
    bool is_ex = false;
    
    integer_t *prod = new integer_t[1<<fan_num];
    prod[0] = (integer_t) 1;
    integer_t *comm_val = new integer_t[fan_num];
    integer_t *xs_prime = new integer_t[fan_num];
    integer_t *aux;
    
    // CRの呼び出し
    CR.call(mod_type,p,fan_num,is_ex,aux);

    // 送信する値の計算
    for(int i=0;i<fan_num;i++) comm_val[i] = vals[i] - aux[(1<<i)-1];

    if(party_id==1) {tcp[2].send_nums(comm_val,fan_num);tcp[2].receive_nums(comm_val,fan_num);}
    else            {tcp[1].send_nums(comm_val,fan_num);tcp[1].receive_nums(comm_val,fan_num);}
    
    // xs_primeの計算(x-aみたいなのに対応する値)
    for(int i=0;i<fan_num;i++) xs_prime[i] = comm_val[i] + vals[i] - aux[(1<<i)-1];
    
    for(int j=1;j<(1<<fan_num);j++){
        for(int k=0;k<fan_num;k++){
            if((j>>k)&1){
                prod[j] = prod[j - (1<<k)] * xs_prime[k];
                break;
            }
        }
    }

    if(party_id==1) ans = prod[(1<<fan_num)-1];
    else            ans = 0;

    int all_one_bit = (1<<fan_num) - 1;
    for(int i=1;i<(1<<fan_num);i++) ans += aux[i-1]*prod[all_one_bit^i];
    delete[] prod;
    delete[] comm_val;
    delete[] xs_prime;
}

void multi_fan_in_and(bool **vals, int numlen, int fan_num, bool *ans){
    // valのサイズはnumlen * fan_num 
    // ans[i] = vals[i][0]*vals[i][1]*...*vals[i][fan_num-1]
    // CRのtype
    int mod_type = 3;
    integer_t p = 0;
    bool is_ex = false;

    bool **prod = new bool*[numlen];
    for(int i=0;i<numlen;i++) prod[i] = new bool[1<<fan_num];
    bool *comm_val = new bool[numlen*fan_num]; // 通信に使う配列なので1次元配列にしている
    bool **xs_prime = new bool*[numlen];
    for(int i=0;i<numlen;i++) xs_prime[i] = new bool[fan_num];
    bool *aux;
    bool **aux_store = new bool*[numlen];
    for(int i=0;i<numlen;i++) aux_store[i] = new bool[(1<<fan_num) -1];

    // comm_valの計算
    for(int i=0;i<numlen;i++){
        CR.call(mod_type,p,fan_num,is_ex,aux);
        for(int j=0;j<fan_num;j++) comm_val[i*fan_num + j] = vals[i][j] ^ aux[(1<<j)-1];
        // auxの保存
        for(int j=0;j<(1<<fan_num)-1;j++) aux_store[i][j] = aux[j];
    }
    if(party_id == 1) {tcp[2].send_nums(comm_val,numlen*fan_num);tcp[2].receive_nums(comm_val,numlen*fan_num);}
    else              {tcp[1].send_nums(comm_val,numlen*fan_num);tcp[1].receive_nums(comm_val,numlen*fan_num);}

    // xs_primeの計算(x-aみたいなのに対応する値)
    for(int i=0;i<numlen;i++) for(int j=0;j<fan_num;j++) xs_prime[i][j] = comm_val[i*fan_num+j] ^ vals[i][j] ^ aux_store[i][(1<<j)-1];
    
    // prodの計算
    for(int i=0;i<numlen;i++){
        prod[i][0] = true;
        for(int j=1;j<(1<<fan_num);j++){
            for(int k=0;k<fan_num;k++){
                if((j>>k)&1){
                    prod[i][j] = prod[i][j-(1<<k)] & xs_prime[i][k]; 
                }
            }
        }
    }

    if(party_id == 1) for(int i=0;i<numlen;i++) ans[i] = aux_store[i][(1<<fan_num)-1];
    else              for(int i=0;i<numlen;i++) ans[i] = 0;

    int all_one_bit = (1<<fan_num) - 1;
    for(int i=0;i<numlen;i++) for(int j=1;j<(1<<fan_num);j++) ans[i] ^= aux_store[i][j-1]&prod[i][all_one_bit^j]; 

    for(int i=0;i<numlen;i++) delete[] prod[i];
    delete[] prod;
    delete[] comm_val;
    for(int i=0;i<numlen;i++) delete[] xs_prime[i];
    delete[] xs_prime;
    for(int i=0;i<numlen;i++) delete[] aux_store[i];
    delete[] aux_store;


}
void multi_fan_in_and(bool *vals, int fan_num, bool &ans){
    // valのサイズはfan_num
    // ans = vals[0]*...*vals[fan_num-1]
    
    // CRのtype
    int mod_type = 3;
    integer_t p = 0;
    bool is_ex = false;
    
    bool *prod = new bool[1<<fan_num];
    prod[0] = true;
    bool *comm_val = new bool[fan_num];
    bool *xs_prime = new bool[fan_num];
    bool *aux;
    
    // CRの呼び出し
    CR.call(mod_type,p,fan_num,is_ex,aux);

    // 送信する値の計算
    for(int i=0;i<fan_num;i++) comm_val[i] = vals[i] ^ aux[(1<<i)-1];

    if(party_id==1) {tcp[2].send_nums(comm_val,fan_num);tcp[2].receive_nums(comm_val,fan_num);}
    else            {tcp[1].send_nums(comm_val,fan_num);tcp[1].receive_nums(comm_val,fan_num);}
    
    // xs_primeの計算(x-aみたいなのに対応する値)
    for(int i=0;i<fan_num;i++) xs_prime[i] = comm_val[i] ^ vals[i] ^ aux[(1<<i)-1];
    
    // prodの計算
    for(int j=1;j<(1<<fan_num);j++){
        for(int k=0;k<fan_num;k++){
            if((j>>k)&1){
                prod[j] = prod[j - (1<<k)] & xs_prime[k];
                break;
            }
        }
    }

    if(party_id==1) ans = prod[(1<<fan_num)-1];
    else            ans = false;

    int all_one_bit = (1<<fan_num) - 1;
    for(int i=1;i<(1<<fan_num);i++) ans ^= aux[i-1]&prod[all_one_bit^i];
    delete[] prod;
    delete[] comm_val;
    delete[] xs_prime;
}
void multi_fan_in_and(bool **vals, int numlen, int *fan_num, bool *ans){
    // ans[i] = vals[i][0]*...*vals[i][fan_num[i]-1]

    // CRのtype
    int mod_type = 3;
    integer_t p = 0;
    bool is_ex = false;

    bool **prod = new bool*[numlen];
    for(int i=0;i<numlen;i++) prod[i] = new bool[1<<fan_num[i]];
    int total_fan_num = 0;
    for(int i=0;i<numlen;i++) total_fan_num += fan_num[i];
    bool *comm_val = new bool[total_fan_num];
    bool **xs_prime = new bool*[numlen];
    for(int i=0;i<numlen;i++) xs_prime[i] = new bool[fan_num[i]];
    bool *aux;
    bool **aux_store = new bool*[numlen];
    for(int i=0;i<numlen;i++) aux_store[i] = new bool[(1<<fan_num[i]) - 1];

    // comm_valの計算
    int thre=0;
    for(int i=0;i<numlen;i++){
        CR.call(mod_type,p,fan_num[i],is_ex,aux);
        for(int j=0;j<fan_num[i];j++) comm_val[thre + j] = vals[i][j] ^ aux[(1<<j)-1];
        // auxの保存
        for(int j=0;j<(1<<fan_num[i])-1;j++) aux_store[i][j] = aux[j];
        thre += fan_num[i];
    }
    if(party_id == 1) {tcp[2].send_nums(comm_val,total_fan_num);tcp[2].receive_nums(comm_val,total_fan_num);}
    else              {tcp[1].send_nums(comm_val,total_fan_num);tcp[1].receive_nums(comm_val,total_fan_num);}

    // xs_primeの計算(x-aみたいな値)
    thre = 0;
    for(int i=0;i<numlen;i++){
        for(int j=0;j<fan_num[i];j++){
            xs_prime[i][j] = comm_val[thre+j]^vals[i][j]^aux_store[i][(1<<j)-1];
        }
        thre += fan_num[i];
    }
    delete[] comm_val;
    // prodの計算
    for(int i=0;i<numlen;i++){
        prod[i][0] = true;
        for(int j=1;j<(1<<fan_num[i]);j++){
            for(int k=0;k<fan_num[i];k++){
                if((j>>k)&1){
                    prod[i][j] = prod[i][j-(1<<k)] & xs_prime[i][k];
                }
            }
        }
    }

    if(party_id==1) for(int i=0;i<numlen;i++) ans[i] = prod[i][(1<<fan_num[i])-1];
    else            for(int i=0;i<numlen;i++) ans[i] = 0;

    for(int i=0;i<numlen;i++){
        int all_one_bit = (1<<fan_num[i])-1;
        for(int j=1;j<(1<<fan_num[i]);j++) ans[i] ^= aux_store[i][j-1]&prod[i][all_one_bit^j]; 
    }

    for(int i=0;i<numlen;i++) delete[] prod[i];
    delete[] prod;
    for(int i=0;i<numlen;i++) delete[] xs_prime[i];
    delete[] xs_prime;
    for(int i=0;i<numlen;i++) delete[] aux_store[i];
    delete[] aux_store;
    
}


void multi_fan_in_or(bool **vals, int numlen, int fan_num, bool *ans){
    // valのサイズはnumlen * fan_num
    // ans[i] = vals[i][0] OR ... OR vals[i][fan_num-1]

    bool **not_vals = new bool*[numlen];
    for(int i=0;i<numlen;i++) not_vals[i] = new bool[fan_num];

    if(party_id == 1) {
        for(int i=0;i<numlen;i++) for(int j=0;j<fan_num;j++) not_vals[i][j] = vals[i][j]^1;
    }else{
        for(int i=0;i<numlen;i++) for(int j=0;j<fan_num;j++) not_vals[i][j] = vals[i][j];
    }
    multi_fan_in_and(not_vals,numlen,fan_num,ans);
    if(party_id==1) for(int i=0;i<numlen;i++) ans[i] ^= 1;

    for(int i=0;i<numlen;i++) delete[] not_vals[i];
    delete[] not_vals;
}
void multi_fan_in_or(bool *vals, int fan_num, bool &ans){
    // valのサイズはfan_num
    // ans = vals[0] OR ... OR vals[fan_num-1]

    bool *not_vals = new bool[fan_num];
    if(party_id==1) for(int i=0;i<fan_num;i++) not_vals[i] = vals[i] ^ 1;
    else            for(int i=0;i<fan_num;i++) not_vals[i] = vals[i];
    multi_fan_in_and(not_vals,fan_num,ans);
    if(party_id==1) ans ^= 1;
    delete[] not_vals;
}
void multi_fan_in_or(bool **vals,int numlen,int *fan_num,bool *ans){
    bool **not_vals = new bool*[numlen];
    for(int i=0;i<numlen;i++) not_vals[i] = new bool[fan_num[i]];
    if(party_id==1){
        for(int i=0;i<numlen;i++){
            for(int j=0;j<fan_num[i];j++) not_vals[i][j] = vals[i][j] ^ 1;
        }
    }else{
        for(int i=0;i<numlen;i++){
            for(int j=0;j<fan_num[i];j++) not_vals[i][j] = vals[i][j];
        }
    }
    multi_fan_in_and(not_vals,numlen,fan_num,ans);
    if(party_id==1) for(int i=0;i<numlen;i++) ans[i] ^= 1;
    for(int i=0;i<numlen;i++) delete[] not_vals[i];
    delete[] not_vals;
}

void B2A(bool *val, int numlen,integer_t *ans){
    integer_t *temp = new integer_t[numlen];
    ex_multiplication(val, numlen,temp);
    for(int i=0;i<numlen;i++) ans[i] = (integer_t) val[i] - 2 * temp[i];
    delete[] temp;
}
void B2A(bool val, integer_t &ans){
    B2A(&val,1,&ans);
}


// !ここ以降は64bitであることを暗に仮定
// !出力がinteger_t になっているものはboolの後にB2Aをかけている．
void equal_zero(integer_t *val, int numlen, bool *ans){
    bool **bit_decom = new bool*[numlen]; // ビット分解したものにparty 1のみxor 1をしたもの
    for(int i=0;i<numlen;i++) bit_decom[i] = new bool[64];
    
    if(party_id==1){
        for(int i=0;i<numlen;i++){
            for(int j=0;j<64;j++) bit_decom[i][j] = ((val[i]>>j)&1)^1;
        }
    }else{
        for(int i=0;i<numlen;i++){
            for(int j=0;j<64;j++) bit_decom[i][j] = ((-val[i])>>j)&1;
        }
    }
    
    //一回目のcommunication
    bool **first_step = new bool*[numlen*8];
    for(int i=0;i<numlen*8;i++) first_step[i] = new bool[8];
    bool *first_step_ans = new bool[numlen*8];
    for(int i=0;i<numlen;i++){
        for(int j=0;j<8;j++){
            for(int k=0;k<8;k++) first_step[8*i+j][k] = bit_decom[i][8*j+k];
        }
    }
    for(int i=0;i<numlen;i++) delete[] bit_decom[i];
    delete[]bit_decom;
    multi_fan_in_and(first_step,8*numlen,8,first_step_ans);
    for(int i=0;i<numlen*8;i++) delete[] first_step[i];
    delete[] first_step; 
    
    //二回目のcommunication
    bool **second_step = new bool*[numlen];
    for(int i=0;i<numlen;i++) second_step[i] = new bool[8];
    for(int i=0;i<numlen;i++){
        for(int j=0;j<8;j++) second_step[i][j] = first_step_ans[8*i+j];
    }
    delete[] first_step_ans;
    multi_fan_in_and(second_step,numlen,8,ans);
    for(int i=0;i<numlen;i++) delete[] second_step[i];
    delete[] second_step;
}
void equal_zero(integer_t val, bool &ans){
    equal_zero(&val,1,&ans);
}
void equal_zero(integer_t *val,int numlen, integer_t *ans){
    bool *bool_ans = new bool[numlen];
    equal_zero(val,numlen,bool_ans);
    // bool から integer_t への変換
    B2A(bool_ans,numlen,ans);
    delete[] bool_ans;
}
void equal_zero(integer_t val, integer_t &ans){
    equal_zero(&val,1,&ans);
}

void equality(integer_t *val1, integer_t *val2, int numlen, bool *ans){
    integer_t *vals = new integer_t[numlen];
    for(int i=0;i<numlen;i++) vals[i] = val1[i]-val2[i];
    equal_zero(vals,numlen,ans);
    delete[] vals;
}
void equality(integer_t val1,integer_t val2, bool &ans){
    equality(&val1,&val2,1,&ans);
}
void equality(integer_t *val1, integer_t *val2, int numlen, integer_t *ans){
    integer_t *vals = new integer_t[numlen];
    for(int i=0;i<numlen;i++) vals[i] = val1[i]-val2[i];
    equal_zero(vals,numlen,ans);
    delete[] vals;
}
void equality(integer_t val1, integer_t val2, integer_t &ans){
    equality(&val1,&val2,1,&ans);
}

void lessthan(integer_t *val1, integer_t *val2, int numlen, bool *ans){
    // ans[i] = val1[i] < val2[i]
    integer_t *for_msb = new integer_t[3*numlen];
    for(int i=0;i<numlen;i++){
        for_msb[3*i+0] = val1[i];
        for_msb[3*i+1] = val2[i];
        for_msb[3*i+2] = val1[i]-val2[i];
    }
    bool *msb_ans = new bool[3*numlen];
    MSB(for_msb,3*numlen,msb_ans);
    delete[] for_msb;
    bool *xs = new bool[numlen];
    bool *ys = new bool[numlen];
    for(int i=0;i<numlen;i++){
        xs[i] = msb_ans[3*i+0] ^ msb_ans[3*i+1];
        ys[i] = msb_ans[3*i+1] ^ msb_ans[3*i+2];
    }
    multiplication(xs,ys,numlen,ans);
    for(int i=0;i<numlen;i++) ans[i] ^= msb_ans[3*i+2];
    delete[] xs;
    delete[] ys;
    delete[] msb_ans;
}
void lessthan(integer_t val1, integer_t val2, int numlen, bool &ans){
    lessthan(&val1,&val2,1,&ans);
}
void lessthan(integer_t *val1, integer_t *val2, int numlen, integer_t *ans){
    // ans[i] = val1[i] < val2[i]
    bool *bool_ans = new bool[numlen];
    lessthan(val1,val2,numlen,bool_ans);
    B2A(bool_ans,numlen,ans);
    delete[] bool_ans;
}
void lessthan(integer_t val1, integer_t val2, integer_t &ans){
    lessthan(&val1,&val2,1,&ans);
}

void greaterthan(integer_t *val1,integer_t *val2, int numlen, bool *ans){
    lessthan(val2,val1,numlen,ans);
}
void greaterthan(integer_t val1,integer_t val2, bool &ans){
    greaterthan(&val1,&val2,1,&ans);
}
void greaterthan(integer_t *val1,integer_t *val2,int numlen, integer_t *ans){
    lessthan(val2,val1,numlen,ans);
}
void greaterthan(integer_t val1,integer_t val2, integer_t &ans){
    lessthan(val2,val1,ans);
}

void overflow(integer_t *val, int numlen, int mod_bit, bool *ans){
    if(mod_bit==0){
        for(int i=0;i<numlen;i++) ans[i] = false;
        return;
    }
    // mod_bit > 0を想定
    // mod_bit = INTEGER_BITになるようにスケーリング
    integer_t *after_scaling = new integer_t[numlen];
    for(int i=0;i<numlen;i++) after_scaling[i] = val[i]<<(INTEGER_BIT-mod_bit);
    
    // (party 1の)after_scaling > (2^INTEGER_BIT)-1 - (party 2の)after_scalingなので，右辺を置き換え．
    if(party_id == 2) for(int i=0;i<numlen;i++) after_scaling[i] = ((integer_t) -1) - after_scaling[i];

    // after_scaling (of party 1) XOR after_scaling (of party 2)のMSNZBとafter_scaling (of party 1)の積
    // Step 1, ORを取っていく段階
    bool **for_step1 = new bool*[numlen*56];
    for(int i=0;i<numlen*56;i++) for_step1[i] = new bool[i%7+2];
    int *fan_num_for_step1 = new int[numlen*56];
    for(int i=0;i<numlen*56;i++) fan_num_for_step1[i] = i%7+2;
    for(int i=0;i<numlen;i++){
        for(int j=0;j<8;j++){
            for(int k=0;k<7;k++){
                // (上から)j*8 bit目から(上から)j*8+k+1bit目までのOR
                for(int l=0;l<k+2;l++){
                    // (上から)j*8+l bit目 
                    for_step1[i*56+j*7+k][l] = (bool) ((after_scaling[i]>>(INTEGER_BIT-1-(j*8+l)))&1);
                }
            }
        }
    }
    bool *ans_for_step1 = new bool[numlen*56];
    multi_fan_in_or(for_step1,numlen*56,fan_num_for_step1,ans_for_step1);
    for(int i=0;i<numlen*56;i++) delete[] for_step1[i];
    delete[] for_step1;
    delete[] fan_num_for_step1;

    // Step 2, ANDを取っていく段階
    bool **for_step2 = new bool*[numlen*64];
    for(int i=0;i<numlen*64;i++) for_step2[i] = new bool[(i%64)/8+2];
    int *fan_num_for_step2 = new int[numlen*64];
    for(int i=0;i<numlen*64;i++) fan_num_for_step2[i] = (i%64)/8+2;
    
    if(party_id == 1){
        for(int i=0;i<numlen;i++){
            for(int j=0;j<8;j++){
                // j*8 bit目の処理
                for(int l=0;l<j;l++){
                    for_step2[i*64+j*8][l] = ans_for_step1[56*i + 7*l + 6]^1;
                }
                for_step2[i*64+j*8][j]   = (after_scaling[i]>>(INTEGER_BIT-1-(j*8)))&1;
                for_step2[i*64+j*8][j+1] = (after_scaling[i]>>(INTEGER_BIT-1-(j*8)))&1;
                // j*8 +1 bit目の処理
                for(int l=0;l<j;l++){
                    for_step2[i*64+j*8+1][l] = ans_for_step1[56*i + 7*l + 6]^1;
                }
                for_step2[i*64+j*8+1][j]   = ((after_scaling[i]>>(INTEGER_BIT-1-(j*8)))&1)^(ans_for_step1[56*i + 7*j]);
                for_step2[i*64+j*8+1][j+1] = (after_scaling[i]>>(INTEGER_BIT-1-(j*8+1)))&1;
                                
                for(int k=2;k<8;k++){
                    // j*8+k bit目の処理
                    for(int l=0;l<j;l++){
                        for_step2[i*64+j*8+k][l] = ans_for_step1[56*i + 7*l + 6]^1;
                    }
                    for_step2[i*64+j*8+k][j]   = (ans_for_step1[56*i + 7*j+k-1]) ^ (ans_for_step1[56*i + 7*j+k-2]);
                    for_step2[i*64+j*8+k][j+1] = (after_scaling[i]>>(INTEGER_BIT-1-(j*8+k)))&1; 
                }
            }
        }    
    }
    else{
        for(int i=0;i<numlen;i++){
            for(int j=0;j<8;j++){
                // j*8 bit目の処理
                for(int l=0;l<j;l++){
                    for_step2[i*64+j*8][l] = ans_for_step1[56*i + 7*l + 6];
                }
                for_step2[i*64+j*8][j]   = (after_scaling[i]>>(INTEGER_BIT-1-(j*8)))&1;
                for_step2[i*64+j*8][j+1] = false;
                // j*8 +1 bit目の処理
                for(int l=0;l<j;l++){
                    for_step2[i*64+j*8+1][l] = ans_for_step1[56*i + 7*l + 6];
                }
                for_step2[i*64+j*8+1][j]   = ((after_scaling[i]>>(INTEGER_BIT-1-(j*8)))&1)^(ans_for_step1[56*i + 7*j]);
                for_step2[i*64+j*8+1][j+1] = false;

                for(int k=2;k<8;k++){
                    // j*8+k bit目の処理
                    for(int l=0;l<j;l++){
                        for_step2[i*64+j*8+k][l] = ans_for_step1[56*i + 7*l + 6];
                    }
                    for_step2[i*64+j*8+k][j]   = (ans_for_step1[56*i + 7*j+k-1]) ^ (ans_for_step1[56*i + 7*j+k-2]);
                    for_step2[i*64+j*8+k][j+1] = false; 
                }
            }
        }
    }
    delete[] ans_for_step1;
    bool *ans_for_step2 = new bool[numlen*64];
    multi_fan_in_and(for_step2,numlen*64,fan_num_for_step2,ans_for_step2);
    for(int i=0;i<numlen*64;i++) delete[] for_step2[i];
    delete[] for_step2;
    delete[] fan_num_for_step2;

    for(int i=0;i<numlen;i++){
        ans[i] = false;
        for(int j=0;j<64;j++) ans[i] ^= ans_for_step2[64*i+j];
    }
    delete[] ans_for_step2;
    delete[] after_scaling;
}
void overflow(integer_t val, int mod_bit, bool &ans){
    overflow(&val,1,mod_bit,&ans);
}
void overflow(integer_t *val, int numlen, int mod_bit, integer_t *ans){
    bool *bool_ans = new bool[numlen];
    overflow(val,numlen,mod_bit,bool_ans);
    B2A(bool_ans,numlen,ans);
    delete[] bool_ans;
}
void overflow(integer_t val, int mod_bit, integer_t &ans){
    overflow(&val,1,mod_bit,&ans);
}

void rightshift(integer_t *val, int numlen, int shift, integer_t *ans){
    // shift > 0は仮定
    if(shift == 64) {
        for(int i=0;i<numlen;i++) ans[i] = 0;
        return;
    }
    integer_t *top_down_flow  = new integer_t[2*numlen];
    integer_t *for_overflow  = new integer_t[2*numlen];
    // top_down_flow = [top_flow, down_flow]
    // top_flow  : 2^INTEGER_BITでのoverflow
    // down_flow : 2^shift      でのoverflow
    // ans = (share >> shift) + down_flow - 2^{INTEGER_BIT - shift}*top_flow

    // 現状，overflowのベクトル化は単一のmod_bitを想定しているので，2^INTEGER_BITにそろえる
    for(int i=0;i<numlen;i++) for_overflow[i]        = val[i];
    for(int i=0;i<numlen;i++) for_overflow[i+numlen] = val[i]<<(INTEGER_BIT-shift);

    overflow(for_overflow,2*numlen,INTEGER_BIT,top_down_flow);
    for(int i=0;i<numlen;i++){
        ans[i] = (val[i]>>shift) - (top_down_flow[i]<<(INTEGER_BIT-shift)) + top_down_flow[i+numlen];
    }

    delete[] for_overflow;
    delete[] top_down_flow;    
}
void rightshift(integer_t val,int shift, integer_t &ans){
    rightshift(&val,1,shift,&ans);
}


void extractbit(integer_t *val, int numlen, int bit_pos, bool *ans){
    // bit_posは0からスタートしてINTEGER_BIT-1で終わる．
    // (val>>bit_pos) & 1を返す．
    integer_t *for_overflow = new integer_t[numlen];
    for(int i=0;i<numlen;i++) for_overflow[i] = val[i];
    overflow(val, numlen,bit_pos,ans);
    for(int i=0;i<numlen;i++) ans[i] ^= (val[i]>>bit_pos)&1 ;
    delete[] for_overflow;
}
void extractbit(integer_t val, int bit_pos, bool &ans){
    extractbit(&val,1,bit_pos,&ans);
}
void extractbit(integer_t *val, int numlen, int bit_pos, integer_t *ans){
    // bit_posは0からスタートしてINTEGER_BIT-1で終わる．
    // (val>>i) & 1を返す．
    bool *bool_ans = new bool[numlen];
    extractbit(val,numlen,bit_pos,bool_ans);
    B2A(bool_ans,numlen,ans);
    delete[] bool_ans;
}
void extractbit(integer_t val, int bit_pos, integer_t &ans){
    extractbit(&val,1,bit_pos,&ans);
}

void MSB(integer_t *val, int numlen, bool *ans){
    extractbit(val,numlen,INTEGER_BIT-1,ans);
}
void MSB(integer_t val, bool &ans){
    MSB(&val, 1, &ans);
}
void MSB(integer_t *val, int numlen, integer_t *ans){
    extractbit(val,numlen,INTEGER_BIT-1,ans);
}
void MSB(integer_t val, integer_t &ans){
    MSB(&val, 1, &ans);
}