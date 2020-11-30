#include "./tcp_connection.h"

using namespace std;

unsigned long long send_bytes[PARTY_NUM+1] = {};
unsigned long long receive_bytes[PARTY_NUM+1] = {};

char *PORTS[][5] = {
  {"dummy", "12339", "12338", "12337"},
  {"12336", "dummy", "12340", "12341"},
  {"12335", "12342", "dummy", "12343"},
  {"12334", "12344", "12345", "dummy"},
};
char *HOSTS[] = {"127.0.0.1","127.0.0.1","127.0.0.1","127.0.0.1", "127.0.0.1"};


Tcp_Server::Tcp_Server(void){};
Tcp_Server::~Tcp_Server(void){
    close(sckt);
}

void Tcp_Server::init(uint port){
    // create socket
    _sd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(_sd < 0){
        cout<<"server : socket create error"<<endl;
        exit(1);
    }
    // optionをつけるならここに
    int opt = 1;
    if (setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                            &opt, sizeof(opt))) 
        { 
            perror("setsockopt"); 
            exit(EXIT_FAILURE); 
        } 
    int addrlen = sizeof(sv_addr);
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = INADDR_ANY;
    sv_addr.sin_port = htons(port);

    // bind
    if(bind(_sd,(struct sockaddr *)&sv_addr, sizeof(sv_addr))<0){
        cout<<"server : bind error"<<endl;
        exit(1);
    }

    //listen
    if(listen(_sd,MAXPENDING)<0){
        cout<<"server : listen error"<<endl;
        exit(1);
    }

    // accept
    sckt = accept(_sd,(struct sockaddr*)&sv_addr,(socklen_t *)&addrlen);
    if(sckt < 0) {
        cout<<"server : accept error"<<endl;
        exit(1);
    }
};

void Tcp_Server::receive_nums(char *str,int numlen){
    while(numlen > 0){
        int recv_msglen = recv(sckt, str, numlen, 0);
        numlen -= recv_msglen;
        str+=recv_msglen;
    }
};

void Tcp_Server::send_nums(char *str,int numlen){
    while(numlen>0){
        int send_msglen = send(sckt,str,numlen,0);
        numlen -= send_msglen;
        str += send_msglen;
    }
}

Tcp_Client::Tcp_Client(void){};
Tcp_Client::~Tcp_Client(void){
    close(sckt);
}

void Tcp_Client::init(uint port, char *ip){
    // char errmsg[BUFSIZ];
    sckt = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
    if(sckt < 0){
        cout<<"client : socket create error"<<endl;
        exit(1);
    }
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = inet_addr(ip);
    sv_addr.sin_port = htons(port);

    // connection
    if(connect(sckt,(struct sockaddr*)&sv_addr,sizeof(sv_addr))<0){
        cout<<"client : connection error"<<endl;
        // sprintf(errmsg, "(line:%d) %s\n", __LINE__, strerror(errno));
        // printf("(line:%d) %s\n", __LINE__, strerror(errno));
        exit(1);
    }
}

void Tcp_Client::receive_nums(char *str,int numlen){
    while(numlen > 0){
        int recv_msglen = recv(sckt, str, numlen, 0);
        numlen -= recv_msglen;
        str+=recv_msglen;
    }
}

void Tcp_Client::send_nums(char *str,int numlen){
    while(numlen>0){
        int send_msglen = send(sckt,str,numlen,0);
        numlen -= send_msglen;
        str += send_msglen;
    }
}

Tcp_Connection::Tcp_Connection(void){};
Tcp_Connection::~Tcp_Connection(void){
    if(other_id<id) delete tcp_server;
    else delete tcp_client;
}

void Tcp_Connection::init(int _id,int _other_id){
    id = _id;
    other_id = _other_id;
    if(other_id < id){
        tcp_server = new Tcp_Server();
        tcp_server->init(atoi(PORTS[id][other_id]));
    }else{
        tcp_client = new Tcp_Client();
        tcp_client->init(atoi(PORTS[other_id][id]),HOSTS[other_id]);
    }
}

void Tcp_Connection::send_nums(char*str,int numlen){
    send_bytes[other_id] += numlen;
    if(other_id < id) tcp_server->send_nums(str,numlen);
    else tcp_client->send_nums(str,numlen);
}

void Tcp_Connection::send_nums(integer_t *vals,int numlen,int num_char){
    char *str = new char[num_char*numlen];
    to_bytes(numlen,vals,str,num_char);
    send_nums(str,numlen*num_char);
    delete[] str;
}

void Tcp_Connection::send_nums(bool *vals,int numlen){
    char *str = new char[numlen/8+1];
    for(int i=0;i<numlen;i++){
        if(i%8==0) str[i/8]=vals[i];
        else str[i/8] = (str[i/8]<<1)+vals[i];
    }
    send_nums(str,numlen/8+1);
    delete[] str;
}

void Tcp_Connection::receive_nums(char *str,int numlen){
    receive_bytes[other_id] += numlen;
    if(other_id < id) tcp_server->receive_nums(str,numlen);
    else tcp_client->receive_nums(str,numlen);
}

void Tcp_Connection::receive_nums(integer_t *vals,int numlen, int num_char){
    char *str = new char[num_char*numlen];
    receive_nums(str,numlen*num_char);
    from_bytes(numlen,vals,str,num_char);
    delete[] str;
}

void Tcp_Connection::receive_nums(bool *vals,int numlen){
    char *str = new char[numlen/8+1];
    receive_nums(str,numlen/8+1);
    for(int i=numlen-1;i>-1;i--){
        vals[i] = str[i/8]&1;
        str[i/8]=str[i/8]>>1;
    }
    delete[] str;
}

void print_data_transfer_size(void){
    cout<<"my party_id : "<<party_id<<endl;
    for(int i=1;i<PARTY_NUM+1;i++){
        if(i==party_id) continue;
        cout<<"send to party "<<i<<" : "<<send_bytes[i]<<" bytes"<<endl;
    }
    for(int i=1;i<PARTY_NUM+1;i++){
        if(i==party_id) continue;
        cout<<"receive to party "<<i<<" : "<<receive_bytes[i]<<" bytes"<<endl;
    }
}