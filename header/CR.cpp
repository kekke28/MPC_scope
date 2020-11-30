
#include "CR.h"
#include "tcp_connection.h"
using namespace std;

rand_seed::rand_seed(unsigned char *userkey){
    id = new unsigned char[16];
    for(int i=0;i<16;i++) id[i] = 0;
    key = new unsigned char[15*16];
    AES_128_Key_Expansion(userkey, key);
    num_call_AES = 0;
}
rand_seed::~rand_seed(){
    delete[] id;
    delete[] key;
}

void rand_seed::update(){
    for(int k=0;++id[k]==0;k++);
}

void rand_seed::next_rand(unsigned char *out){
    AES_ECB_encrypt(id,out,16,key,10);
    num_call_AES++;
    update();
}

void gen_random_m(rand_seed &seed, integer_t *ans,int num,integer_t p, int num_char){
    num_char++;
    if(num_char<=2) num_char = 2;
    else if(num_char<=4) num_char = 4;
    else if(num_char<=8) num_char = 8;
    else{
        cout<<"overflow p"<<endl;
        exit(1);
    }

    integer_t thre =( (integer_t)1 << (8*num_char-1) ) * 2  /p * p;
    int num_per_one_cyphertext = 16 / num_char;
    int done = 0;
    bool fin = false;
    unsigned char out[16];
    while(true){
        seed.next_rand(out);
        for(int i=0;i<num_per_one_cyphertext;i++){
            integer_t val = 0;
            for(int j=0;j<num_char;j++) val = (val<<8)+out[i*num_char + j];
            if(val<thre){
                ans[done] = val%p;
                done++;
                if(done == num) fin = true;
            }
            if(fin) break;
        }
        if(fin) break;
    }
}

void gen_random(rand_seed &seed,integer_t *ans,int num,int bit_length){
    // generate num random integers in Z_{2^bit_length} using AES-CTR with 128 bit security. 
    //! bit_lengthは8の倍数であることを仮定(暗号文の出力がchar*なので，byte単位がやりやすい)
    int num_per_one_cyphertext = 128 / bit_length; // 128bitの中にbit_lengthが何ブロック存在するか
    int num_cyphertext = (num-1) / num_per_one_cyphertext + 1; // いくつの暗号文が必要か
    unsigned char out[16];
    for(int i=0;i<num_cyphertext; i++){
        seed.next_rand(out);
        for(int j=0;j<num_per_one_cyphertext;j++){
            int ind = i*num_per_one_cyphertext + j;
            ans[ind] = 0;
            for(int k=0;k<bit_length/8;k++) ans[ind] = (ans[ind]<<8) + out[j*INTEGER_BIT/8 + k];
            num--;
            if(num==0) break;
        }
    }
}

void bool_gen_random(rand_seed &seed,bool *ans,int num){
    // generate num random booleans.
    // bit_length = 1
    unsigned char out[16];
    int num_cyphertext = (num-1) / 128 + 1;
    for(int i=0;i<num_cyphertext;i++){
        seed.next_rand(out);
        for(int j=0;j<128;j++){
            int ind = i*128 + j;
            ans[ind] = (((out[j/8]>>(j%8)) & 1)== 1);
            num--;
            if(num==0) break;
        }
    }
}

void BT_gen(rand_seed &seed, 
            integer_t *ans1, integer_t *ans2,
            int num,int fan_num){
    // Beaver Triple (over Z_{2^INTEGER_BIT}) for multi_fan_in gate. [Ohata et al., FC 2020]
    // the length of ans is equal to (2^fan_num -1) * num
    int len = ((1<<fan_num) - 1);
    int total_len = len * num;
    integer_t *temp = new integer_t[total_len + num * fan_num];
    gen_random(seed, temp, total_len + num * fan_num);
    
    for(int i=0;i<total_len;i++) ans1[i] = temp[i];
    
    integer_t *x = new integer_t[fan_num];
    integer_t *prod_x = new integer_t[1<<fan_num];
    prod_x[0] = 1;
    for(int i=0; i<num;i++){
        for(int j=0;j<fan_num;j++) x[j] = temp[total_len + i*fan_num + j];

        for(int j=1;j<(1<<fan_num);j++){
            for(int k=0;k<fan_num;k++){
                if((j>>k)&1){
                    prod_x[j] = prod_x[j - (1<<k)] * x[k];
                    break;
                }
            }
        }

        for(int j=1;j<(1<<fan_num);j++) ans2[len * i + j - 1] = prod_x[j] - ans1[len * i + j - 1];
    }
    delete[] x;
    delete[] prod_x;
    delete[] temp;
}

void bool_BT_gen(rand_seed &seed,
                 bool* ans1, bool* ans2,
                 int num, int fan_num){
    // Beaver Triple (over Z_2) for multi_fan_in gate. [Ohata et al., FC 2020]
    // the length of ans is equal to (2^fan_num -1) * num
    int len = ((1<<fan_num) - 1);
    int total_len = len * num;
    bool *temp = new bool[total_len + num * fan_num];
    bool_gen_random(seed, temp, total_len + num * fan_num);
    
    for(int i=0;i<total_len;i++) ans1[i] = temp[i];
    
    bool *x = new bool[fan_num];
    bool *prod_x = new bool[1<<fan_num];
    prod_x[0] = true;
    for(int i=0; i<num;i++){
        for(int j=0;j<fan_num;j++) x[j] = temp[total_len + i*fan_num + j];
        for(int j=1;j<(1<<fan_num);j++){
            for(int k=0;k<fan_num;k++){
                if((j>>k)&1){
                    prod_x[j] = prod_x[j - (1<<k)] & x[k];
                    break;
                }
            }
        }
        for(int j=1;j<(1<<fan_num);j++) ans2[len * i + j - 1] = prod_x[j] ^ ans1[len * i + j - 1];
    }
    delete[] x;
    delete[] prod_x;
    delete[] temp;
}

// ! pが素数のときは（用途によっては）Damgardのmulti_fan_inが使える．が，これは実装していない．
void BT_gen_p(rand_seed &seed,
              integer_t *ans1, integer_t *ans2,
              int num,int fan_num,
              integer_t p, int num_char){
    // Beaver Triple (over Z_p) for multi_fan_in gate. [Ohata et al., FC 2020]
    // num_char : pのバイト数（切り上げ）
    // the length of ans is equal to (2^fan_num -1) * num
    int len = ((1<<fan_num) - 1);
    int total_len = len * num;
    integer_t *temp = new integer_t[total_len + num * fan_num];
    gen_random_m(seed, temp, total_len + num * fan_num, p, num_char);
    
    for(int i=0;i<total_len;i++) ans1[i] = temp[i];
    
    integer_t *x = new integer_t[fan_num];
    integer_t *prod_x = new integer_t[1<<fan_num];
    prod_x[0] = 1;
    for(int i=0; i<num;i++){
        for(int j=0;j<fan_num;j++) x[j] = temp[total_len + i*fan_num + j];

        for(int j=1;j<(1<<fan_num);j++){
            for(int k=0;k<fan_num;k++){
                if((j>>k)&1){
                    prod_x[j] = (prod_x[j - (1<<k)] * x[k])%p;
                    break;
                }
            }
        }

        for(int j=1;j<(1<<fan_num);j++) ans2[len * i + j - 1] = (prod_x[j] + p - ans1[len * i + j - 1])%p;
    }
    delete[] x;
    delete[] prod_x;
    delete[] temp;
    }

void ex_BT_gen(rand_seed &seed,integer_t *ans1, integer_t *ans2,int num){
    //(a,c1), (b,c2), where c1 + c2 = ab
    integer_t *temp = new integer_t[3*num];
    gen_random(seed,temp,3*num);
    for(int i=0;i<num;i++){
        ans1[2*i] = temp[3*i];
        ans2[2*i] = temp[3*i+1];
    
        ans1[2*i+1] = temp[3*i+2];
        ans2[2*i+1] = ans1[2*i] * ans2[2*i] - ans1[2*i+1];
    }
    delete[] temp;
}

void ex_bool_BT_gen(rand_seed &seed,bool *ans1,bool *ans2,int num){
    //(a,c1), (b,c2), where c1 + c2 = ab
    bool *temp = new bool[3*num];
    bool_gen_random(seed,temp,3*num);
    for(int i=0;i<num;i++){
        ans1[2*i] = temp[3*i];
        ans2[2*i] = temp[3*i+1];
    
        ans1[2*i+1] = temp[3*i+2];
        ans2[2*i+1] = ans1[2*i] * ans2[2*i]- ans1[2*i+1];
    }
    delete[] temp;
}

void ex_BT_gen_p(rand_seed &seed, 
                 integer_t *ans1,integer_t *ans2,
                 int num,
                 integer_t p, int num_char){
    integer_t *temp = new integer_t[3*num];
    gen_random_m(seed,temp,3*num,p,num_char);
    // ! p^2 + pがオーバーフローするとバグる
    for(int i=0;i<num;i++){
        ans1[2*i]   = temp[3*i];
        ans1[2*i+1] = temp[3*i+1];
        
        ans2[2*i]   = temp[3*i+2];
        ans2[2*i]   = (ans1[2*i] * ans2[2*i] + p - ans1[2*i+1] ) % p;
    }
    delete[] temp;
}


void BT_Segment::init(int _mod_type,integer_t _p, int _fan_num,bool _is_ex,int _max_store){
    mod_type = _mod_type;
    p = _p;
    fan_num = _fan_num;
    is_ex = _is_ex;
    max_store = _max_store;
    counter = 0;
    if(is_ex) length = 2;
    else length = (1<<fan_num)-1;
    if(mod_type == 1 || mod_type == 2) cr = new integer_t[length*max_store];
    else bool_cr = new bool[length*max_store];
    supply();
}
BT_Segment::~BT_Segment(){
    if(mod_type == 1 || mod_type == 2) delete[] cr;
    else delete[] bool_cr;
}

void BT_Segment::supply(){
    integer_t query[5] = {(integer_t)mod_type,p,(integer_t)fan_num,(integer_t)is_ex,(integer_t)max_store};
    tcp[0].send_nums(query,5);
    if(mod_type == 1 || mod_type == 2) tcp[0].receive_nums(cr,length*max_store);
    else                               tcp[0].receive_nums(bool_cr,length*max_store);
    counter = 0;
}

void BT_Segment::call(bool *&aux){
    if(counter == max_store) supply();
    aux = bool_cr + counter * length;
    counter++;
}

void BT_Segment::call(integer_t *&aux){
    if(counter == max_store) supply();
    aux = cr + counter * length;
    counter++;
}
bool CR_gen_by_server(rand_seed &seed){
    integer_t query_from_1[5], query_from_2[5];
    tcp[1].receive_nums(query_from_1,5);
    tcp[2].receive_nums(query_from_2,5);
    if(query_from_1[0] == (integer_t) -1){
        return false;
    }
    integer_t mod_type = query_from_1[0];
    integer_t p = query_from_1[1];
    int fan_num = (int) query_from_1[2];
    bool is_ex = (bool) query_from_1[3];
    int max_store = (int) query_from_1[4];
    int num_char=0;
    integer_t hoge=1;
    while(hoge<p){
        num_char+=1;
        hoge *= 256;
    }
    int length;
    if(is_ex) length = 2;
    else length = (1<<fan_num)-1;
    integer_t *cr_1, *cr_2;
    bool *bool_cr_1, *bool_cr_2;
    if(mod_type==1||mod_type==2){
        cr_1 = new integer_t[length*max_store];
        cr_2 = new integer_t[length*max_store];
    }
    else{
        bool_cr_1 = new bool[length*max_store];
        bool_cr_2 = new bool[length*max_store];
    }

    if(is_ex){
        if(mod_type == 1) ex_BT_gen(seed,cr_1,cr_2,max_store);
        else if(mod_type == 2) ex_BT_gen_p(seed,cr_1,cr_2,max_store,p,num_char);
        else ex_bool_BT_gen(seed,bool_cr_1,bool_cr_2,max_store);
    }else{
        if(mod_type == 1) BT_gen(seed,cr_1,cr_2,max_store,fan_num);
        else if(mod_type == 2) BT_gen_p(seed,cr_1,cr_2,max_store,fan_num,p,num_char);
        else bool_BT_gen(seed,bool_cr_1,bool_cr_2,max_store,fan_num);
    }

    if(mod_type==1||mod_type==2){
        tcp[1].send_nums(cr_1,length*max_store);
        tcp[2].send_nums(cr_2,length*max_store);
    }else{
        tcp[1].send_nums(bool_cr_1,length*max_store);
        tcp[2].send_nums(bool_cr_2,length*max_store);
    }

    if(mod_type==1||mod_type==2) {delete[] cr_1; delete[] cr_2;}
    else                         {delete[] bool_cr_1; delete[] bool_cr_2;}
    return true;
}

BT_Store::BT_Store(){
    unordered_map<unsigned long long, BT_Segment> store = {};
};
BT_Store::~BT_Store(){}

void BT_Store::call(int mod_type,integer_t p,int fan_num,bool is_ex,bool *&aux){
    unsigned long long key = to_type(mod_type,p,fan_num,is_ex);
    auto itr = store.find(key);
    if(itr != store.end()){// すでにBT_Segmentが存在していた場合
        itr->second.call(aux);
    }else{// BT_Segmentが存在しなかった場合
        store[key].init(mod_type,p,fan_num,is_ex);
        store[key].call(aux);
    }
}
void BT_Store::call(int mod_type,integer_t p,int fan_num,bool is_ex,integer_t *&aux){
    unsigned long long key = to_type(mod_type,p,fan_num,is_ex);   
    auto itr = store.find(key);
    if(itr != store.end()){// すでにBT_Segmentが存在していた場合
        itr->second.call(aux);
    }else{// BT_Segmentが存在しなかった場合
        store[key].init(mod_type,p,fan_num,is_ex);
        store[key].call(aux);
    }
}

void BT_Store::program_finish(){
    integer_t query[5] = {(integer_t)-1,(integer_t)0,(integer_t)0,(integer_t)0,(integer_t)0};
    tcp[0].send_nums(query,5);
}

unsigned long long BT_Store::to_type(int mod_type,integer_t p, int fan_num,bool is_ex){
    // ! pは32bit未満であることを仮定
    // ! fan_numは32未満を想定
    return is_ex+(mod_type<<1)+(fan_num<<3)+(p<<8);
}

// ! BT_Store CRの定義はここ
BT_Store CR;