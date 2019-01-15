//
// Created by ron on 13/01/19.
//
#include "server.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <iostream>
#include <vector>
#include <string>
#define BUFFER_SIZE 1024
struct Param {
  ClientHandler * c_h;
  int port_num;
};
void* ClientSocket(void *param){
  auto params = (struct Param*) param;
  auto port_num = params->port_num;
  auto c_h = params->c_h;
  c_h->handelClient(port_num);
  //int newsockfd =  *((int*) sockfd);
  //std::cout<<"in socket :" + std::to_string(*n)<< std::endl;
  //char buffer[BUFFER_SIZE];
  //int n;
  //std::string input;
  //std::string curr_input;
  //bool get_input=true;
  //while (get_input) {
   // bzero(buffer, BUFFER_SIZE);
   // n = read(newsockfd, buffer, BUFFER_SIZE - 1);
   // if (n < 0) {
    //  perror("ERROR reading from socket");
     // exit(1);
    //}
    //curr_input = buffer;
    //if (curr_input.find("end") != std::string::npos) {
      //get_input = false;
    //}
    //input += curr_input;
  //}
  //char input_char[input.size()]
  //send(newsockfd,input.c_str(),input.size(),0);
  //std::cout<<input;
}



void* OpenClientsSockets(void *param) {
  auto params = (struct Param*) param;
  int sockfd = params->port_num;
  int  clilen , newsockfd;
  struct sockaddr_in  cli_addr;
  clilen = sizeof(cli_addr);
  std::vector<pthread_t*> threads;
  pthread_t* t;
  while (true) {
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
    if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
    }
    //while (true) {
    t = new pthread_t;
    threads.push_back(t);
    params->port_num=newsockfd;
    pthread_create(t, nullptr, ClientSocket, (void *) &params);
    //}
  }
  std::cout<<"in open clients sockets"<<std::endl;
}


void MyParallelServer::open(int port ,ClientHandler* client_handler) {

  int sockfd, portno;
  struct sockaddr_in serv_addr;
  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }
  /* Initialize socket structure */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = port;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  /* Now bind the host address using bind() call.*/
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
  }
  /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
  */
  listen(sockfd,SOMAXCONN);
  auto * params = new Param;
  params->port_num = port;
  params->c_h = client_handler;
  pthread_t t1;
  pthread_create(&t1, nullptr,OpenClientsSockets,(void*) params);
  while (true) {

  }
}
