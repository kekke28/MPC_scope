#include "../src/data_type.hpp"
#include "unistd.h"
using namespace std;

int party_id;
Tcp_Connection *tcp;

int main(int argc, char *argv[]){
    
    //前処理
    party_id = atoi(argv[1]);
    tcp = new Tcp_Connection[3];
    if(party_id==0){tcp[1].init(0,1);tcp[2].init(0,2);                 }
    if(party_id==1){tcp[0].init(1,0);sleep(2);        tcp[2].init(1,2);}
    if(party_id==2){                 tcp[0].init(2,0);tcp[1].init(2,1);}

    if(party_id == 0){
        unsigned char userkey[16] = "00000000";
        rand_seed seed(userkey);
        while(CR_gen_by_server(seed));
        delete[] tcp;
        return 0;
    }
    
    // ここから秘密計算でやりたい処理

    s_integer x,y,ans;
    integer_t reconst_ans;
    if(party_id == 1){x = 123; y = 65;}
    else             {x = 45; y = 112;}
    
    ans = (x==y);
    Reconst(&ans.val,1,&reconst_ans);
    cout<<"x==y  :  "<<reconst_ans<<endl;
    ans = (x<y) ; 
    Reconst(&ans.val,1,&reconst_ans);
    cout<<"x<y  :  "<<reconst_ans<<endl;
    ans = x*y;
    Reconst(&ans.val,1,&reconst_ans);
    cout<<"x*y  :  "<<reconst_ans<<endl;

    CR.program_finish();
    delete[] tcp;
    return 0;
}