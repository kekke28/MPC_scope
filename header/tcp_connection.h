#ifndef TCPCONNECTION
#define TCPCONNECTION

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include "common.h"


inline void from_bytes(int num, integer_t *vals, char* str, int num_char){
  for(int i=0;i<num;i++){
    vals[i] = (integer_t)0;
    for(int j=num_char-1;j>=0;j--) vals[i] = (integer_t)(vals[i]<<8) + (unsigned char)str[i*num_char + j];
  }
}

inline void to_bytes(int num, integer_t *vals, char* str, int num_char){
  for(int i=0;i<num;i++){
    integer_t val = vals[i];
    for(int j=0;j<num_char;j++){
      str[i*num_char+j] = (char)(val%256);
      val=val>>8;
    }
  }
}

struct Tcp_Server{
    struct sockaddr_in sv_addr;
    int _sd;
    int sckt;

    Tcp_Server();
    ~Tcp_Server();
    void init(uint port);
    void receive_nums(char *str,int numlen);
    void send_nums(char *str,int numlen);
};

struct Tcp_Client{
    struct sockaddr_in sv_addr;
    int sckt;
    Tcp_Client();
    ~Tcp_Client();
    void init(uint port, char *ip);
    void receive_nums(char *str,int numlen);
    void send_nums(char *str,int numlen);
};


struct Tcp_Connection{
    int id;
    int other_id;
    Tcp_Server *tcp_server;
    Tcp_Client *tcp_client;
    Tcp_Connection();
    ~Tcp_Connection();
    void init(int _id,int _other_id);
    void send_nums(char *str,int numlen);
    void send_nums(integer_t *vals, int numlen,int num_char = INTEGER_NUM_CHAR);
    void send_nums(bool *vals,int numlen);
    void receive_nums(char *str,int numlen);
    void receive_nums(integer_t *vals, int numlen,int num_char = INTEGER_NUM_CHAR);
    void receive_nums(bool *vals,int numlen);
};

void print_data_transfer_size(void);

// ! party_idをもとに定義されるのでmain関数に定義を記述
extern Tcp_Connection *tcp; // main関数で定義

#endif