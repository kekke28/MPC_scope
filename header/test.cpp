#include "common.h"
#include "envinfo.h"
#include "tcp_connection.h"

using namespace std;

int party_id;

int main(int argc, char *argv[]){
    party_id = atoi(argv[1]);
    cout<<"party_id "<<party_id<<endl;
    Tcp_Connection *tcp = new Tcp_Connection[PARTY_NUM+1];
    if(party_id == 1){
        tcp[2].init(1,2);
    }
    if(party_id == 2){
        tcp[1].init(2,1);
    }
    cout<<"connection done"<<endl;
    integer_t x = party_id;
    tcp[3-party_id].send_nums(&x,1,8);
    tcp[3-party_id].receive_nums(&x,1,8);

    cout<<party_id<<" "<<x<<endl;
    delete[] tcp;

    return 0;
}