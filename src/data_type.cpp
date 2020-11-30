#include "data_type.hpp"

const s_integer operator*(const s_integer &lhs,s_integer obj){
    s_integer ans;
    integer_t ans_val;
    multiplication(lhs.val,obj.val,ans_val);
    ans = ans_val;
    return ans;
}

const s_integer operator==(const s_integer &lhs,s_integer obj){
    s_integer ans;
    integer_t ans_val;
    equality(lhs.val,obj.val,ans_val);
    ans = ans_val;
    return ans;
}
const s_integer operator==(const s_integer &lhs,integer_t value){
    s_integer ans;
    integer_t ans_val;
    if(party_id==1) equality(lhs.val,value,ans_val);
    else equality(lhs.val,(integer_t)0, ans_val);
    ans = ans_val;
    return ans;
}
const s_integer operator==(integer_t value,const s_integer &lhs){
    s_integer ans;
    integer_t ans_val;
    if(party_id==1) equality(lhs.val,value,ans_val);
    else equality(lhs.val,(integer_t)0, ans_val);
    ans = ans_val;
    return ans;
}

const s_integer operator!=(const s_integer &lhs,s_integer obj){
    s_integer ans = (lhs == obj);
    if(party_id==1) ans.val = (integer_t)1 - ans.val;
    else            ans.val =              - ans.val; 
    return ans;
}
const s_integer operator!=(const s_integer &lhs,integer_t value){
    s_integer ans = (lhs == value);
    if(party_id==1) ans.val = (integer_t)1 - ans.val;
    else            ans.val =              - ans.val; 
    return ans;
}
const s_integer operator!=(integer_t value,const s_integer &lhs){
    s_integer ans = (lhs == value);
    if(party_id==1) ans.val = (integer_t)1 - ans.val;
    else            ans.val =              - ans.val; 
    return ans;
}

const s_integer operator<(const s_integer &lhs,s_integer obj){
    s_integer ans;
    integer_t ans_val;
    lessthan(lhs.val,obj.val,ans_val);
    ans.val = ans_val;
    return ans;
}
const s_integer operator<(const s_integer &lhs,integer_t value){
    s_integer ans;
    integer_t ans_val;
    lessthan(lhs.val,value,ans_val);
    ans.val = ans_val;
    return ans;
}
const s_integer operator<(integer_t value,const s_integer &lhs){
    s_integer ans;
    integer_t ans_val;
    lessthan(value,lhs.val,ans_val);
    ans.val = ans_val;
    return ans;
}

const s_integer operator<=(const s_integer &lhs,s_integer obj){
    s_integer ans = (lhs > obj);
    if(party_id == 1) ans.val = (integer_t) 1 - ans.val;
    else              ans.val =               - ans.val;
    return ans;
}
const s_integer operator<=(const s_integer &lhs,integer_t value){
    s_integer ans = (lhs > value);
    if(party_id == 1) ans.val = (integer_t) 1 - ans.val;
    else              ans.val =               - ans.val;
    return ans;
}
const s_integer operator<=(integer_t value,const s_integer &lhs){
    s_integer ans = (value > lhs);
    if(party_id == 1) ans.val = (integer_t) 1 - ans.val;
    else              ans.val =               - ans.val;
    return ans;
}

const s_integer operator>(const s_integer &lhs,s_integer obj){
    s_integer ans;
    integer_t ans_val;
    greaterthan(lhs.val,obj.val,ans_val);
    ans.val = ans_val;
    return ans;
}
const s_integer operator>(const s_integer &lhs,integer_t value){
    s_integer ans;
    integer_t ans_val;
    greaterthan(lhs.val,value,ans_val);
    ans.val = ans_val;
    return ans;
}
const s_integer operator>(integer_t value,const s_integer &lhs){
    s_integer ans;
    integer_t ans_val;
    greaterthan(value,lhs.val,ans_val);
    ans.val = ans_val;
    return ans;
}

const s_integer operator>=(const s_integer &lhs,s_integer obj){
    s_integer ans = (lhs < obj);
    if(party_id == 1) ans.val = (integer_t) 1 - ans.val;
    else              ans.val =               - ans.val;
    return ans;
}
const s_integer operator>=(const s_integer &lhs,integer_t value){
    s_integer ans = (lhs < value);
    if(party_id == 1) ans.val = (integer_t) 1 - ans.val;
    else              ans.val =               - ans.val;
    return ans;
}
const s_integer operator>=(integer_t value,const s_integer &lhs){
    s_integer ans = (value < lhs);
    if(party_id == 1) ans.val = (integer_t) 1 - ans.val;
    else              ans.val =               - ans.val;
    return ans;
}