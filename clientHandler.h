//
// Created by ron on 13/01/19.
//

#ifndef SEMSTERPROJECT_CLIENTHANDLER_H
#define SEMSTERPROJECT_CLIENTHANDLER_H
#include <strings.h>
#include <sys/socket.h>
#include "cacheHandler.h"
#include "solver.h"
//#include <netdb.h>
#include <unistd.h>
#include <iostream>
#define BUFFER_SIZE 1024
class ClientHandler {
public:
    virtual void handelClient(int sock_number) = 0;
    virtual bool handels()=0;
};


template <class Solution ,class Problem >
    class MyClientHandler : public ClientHandler {
     private:
      CacheManager<Solution,Problem>* cache_manager;
      Solver<Problem,Solution>* solver;
      bool still_handle=true;
     public:
        MyClientHandler(CacheManager<Solution,Problem> *cache_manager ,Solver<Problem,Solution>* solver) {
          this->cache_manager = cache_manager;
          this->solver = solver;
      }
      std::string readClient(int sock_number) {
          //std::cout<<"in socket :" + std::to_string(*n)<< std::endl;
          char buffer[BUFFER_SIZE];
          int n;
          std::string input;
          std::string curr_input;
          bool get_input=true;
          while (get_input) {
              bzero(buffer, BUFFER_SIZE);
              n = read(sock_number, buffer, BUFFER_SIZE - 1);
              if (n < 0) {
                  perror("ERROR reading from socket");
                  exit(1);
              }
              curr_input = buffer;
              if (curr_input.find("end") != std::string::npos) {
                  get_input = false;
              }
              input += curr_input;
          }
          return input;
      }
      bool handels() {
          return still_handle;
        }
      void handelClient(int sock_number){
          //std::cout<<"in handle c";
          std::string input = this->readClient(sock_number);
          send(sock_number,input.c_str(),input.size(),0);
          //this->still_handle= false;
      }



};


#endif //SEMSTERPROJECT_CLIENTHANDLER_H
