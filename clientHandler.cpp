//
// Created by ron on 13/01/19.
//
#define REMOVE_END 4
#include "clientHandler.h"

namespace server_side {
    class MyTestClientHandler:public ClientHandler{
        Solver<string,string> *solver;
        CacheManager* cm;
        Server* s;
    public:
        void handelClient(int sock_number) {

  //          string input = s->reciverFromClient(sock_number);
    //        input = input.substr(0, input.length()-REMOVE_END);
    //        string sol = "";
        }
    };
}