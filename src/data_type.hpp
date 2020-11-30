#ifndef DATATYPE
#define DATATYPE

#include "../header/common.h"
#include "../header/tcp_connection.h"
#include "../header/CR.h"
#include "basic_operation.hpp"

struct s_integer{
    // 2 out of 2 additive secret sharing
    integer_t val;

    s_integer(){};
    ~s_integer(){};

    s_integer operator =(integer_t input){
        this->val = input;
        return (*this);
    };
    s_integer operator =(const s_integer input){
        this->val = input.val;
        return (*this);
    }
};


inline const s_integer operator+(const s_integer &lhs,s_integer obj){
    s_integer ans;
    ans.val = lhs.val+obj.val;
    return ans;
}
inline const s_integer operator+(const s_integer &lhs,integer_t value){
    s_integer ans;
    if(party_id==1) ans.val = lhs.val + value;
    else ans.val = lhs.val;
    return ans;
}
inline const s_integer operator+(integer_t value,const s_integer &lhs){
    s_integer ans;
    if(party_id==1) ans.val = lhs.val + value;
    else ans.val = lhs.val;
    return ans;    
}

inline const s_integer operator-(const s_integer &lhs,s_integer obj){
    s_integer ans;
    ans.val = lhs.val-obj.val;
    return ans;
}
inline const s_integer operator-(const s_integer &lhs,integer_t value){
    s_integer ans;
    if(party_id==1) ans.val = lhs.val - value;
    else ans.val = lhs.val;
    return ans;
}
inline const s_integer operator-(integer_t value,const s_integer &lhs){
    s_integer ans;
    if(party_id==1) ans.val = value - lhs.val;
    else ans.val = -lhs.val;
    return ans; 
}

const s_integer operator*(const s_integer &lhs,s_integer obj);
inline const s_integer operator*(const s_integer &lhs,integer_t value){
    s_integer ans;
    ans.val = lhs.val * value;
    return ans;
}
inline const s_integer operator*(integer_t value,const s_integer &lhs){
    s_integer ans;
    ans.val = lhs.val * value;
    return ans;
}

// ! 出力はinteger_t上のシェアにしている
const s_integer operator==(const s_integer &lhs,s_integer obj); // todo 通信が必要
const s_integer operator==(const s_integer &lhs,integer_t value); // todo 通信が必要
const s_integer operator==(integer_t value,const s_integer &lhs); // todo 通信が必要

const s_integer operator!=(const s_integer &lhs,s_integer obj); // todo 通信が必要
const s_integer operator!=(const s_integer &lhs,integer_t value); // todo 通信が必要
const s_integer operator!=(integer_t value,const s_integer &lhs); // todo 通信が必要

const s_integer operator<(const s_integer &lhs,s_integer obj); // todo 通信が必要
const s_integer operator<(const s_integer &lhs,integer_t value); // todo 通信が必要
const s_integer operator<(integer_t value,const s_integer &lhs); // todo 通信が必要

const s_integer operator<=(const s_integer &lhs,s_integer obj); // todo 通信が必要
const s_integer operator<=(const s_integer &lhs,integer_t value); // todo 通信が必要
const s_integer operator<=(integer_t value,const s_integer &lhs); // todo 通信が必要

const s_integer operator>(const s_integer &lhs,s_integer obj); // todo 通信が必要
const s_integer operator>(const s_integer &lhs,integer_t value); // todo 通信が必要
const s_integer operator>(integer_t value,const s_integer &lhs); // todo 通信が必要

const s_integer operator>=(const s_integer &lhs,s_integer obj); // todo 通信が必要
const s_integer operator>=(const s_integer &lhs,integer_t value); // todo 通信が必要
const s_integer operator>=(integer_t value,const s_integer &lhs); // todo 通信が必要



#endif