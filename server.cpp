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
#include <fcntl.h>
#define TIME_OUT 10
#define BUFFER_SIZE 1024
struct Param {
  ClientHandler * c_h;
  int port_num;
  bool * handles_clients;
  std::mutex* thread_mutex;
};

struct ParamClient {
  ClientHandler * c_h;
  int socket_num;
  bool * is_open;
  std::mutex* thread_mutex;
};
void* ClientSocket(void *param){
  std::cout<<"in client socket"<<std::endl;
  auto params = (struct ParamClient*) param;
  auto socket_num = params->socket_num;
  auto c_h = params->c_h;
  c_h->handelClient(socket_num);
  std::cout<<"after call handle"<<std::endl;
  params->thread_mutex->lock();
  *params->is_open = false;
  params->thread_mutex->unlock();
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
  ParamClient* param_client = new ParamClient;
  int sockfd = params->port_num;
  int  clilen , newsockfd;
  struct sockaddr_in  cli_addr;
  clilen = sizeof(cli_addr);
  std::vector<pthread_t*> threads;
  std::vector<int> sockets;
  std::vector<bool*> is_thread_open;
  bool* thread_open;
  bool first_client=true;
  pthread_t* t;
  while (true) {
    if(!first_client) {
      sleep(TIME_OUT);

    }
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
    if(first_client) {
      first_client = false;
      fcntl(sockfd,F_SETFL,O_NONBLOCK);
    }
    if (newsockfd < 0) {
      break;
    }
    //while (true) {
    std::cout << "after accept" << std::endl;
    //params->c_h->handelClient(54);
    t = new pthread_t;
    threads.push_back(t);
    sockets.push_back(newsockfd);
    thread_open = new bool;
    *thread_open = true;
    is_thread_open.push_back(thread_open);
    param_client->is_open = thread_open;
    param_client->socket_num = newsockfd;
    param_client->c_h = params->c_h;
    param_client->thread_mutex = params->thread_mutex;
    //std::cout << params->port_num << std::endl;
    pthread_create(t, nullptr, ClientSocket, (void *) param_client);
    //}
    //}
  }

  std::cout<<"after sleep"<<std::endl;

  //bool threads_finished = false;
  int num_of_closed_thread=0;
  int size = is_thread_open.size();
  while (true) {
      for (auto thread : is_thread_open) {
          params->thread_mutex->lock();
          if(!*thread) {
            num_of_closed_thread++;
          }
          params->thread_mutex->unlock();
      }
      if (num_of_closed_thread == size) {
          break;
      }
      num_of_closed_thread=0;
  }
  std::cout << "after thread is closed check finish"<<std::endl;

    for(auto socket_num : sockets) {
      close(socket_num);
    }
    close(sockfd);
    for(auto thread: threads) {
      pthread_join(*thread,NULL);
      delete(thread);
    }
    for(auto is_open :is_thread_open) {
      delete (is_open);
    }
    params->thread_mutex->lock();
    *params->handles_clients = false;
    params->thread_mutex->unlock();

  //std::cout<<"in open clients sockets"<<std::endl;

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
  params->port_num = sockfd;
  params->c_h = client_handler;
  bool * handles_clients = new bool;
  *handles_clients=true;
  params->handles_clients = handles_clients;
  pthread_t t1;
  std::mutex thread_mutex;
  std::cout<<"before thread"<<std::endl;
  params->thread_mutex = &thread_mutex;
  pthread_create(&t1, nullptr,OpenClientsSockets,(void*) params);
  while (true) {
    thread_mutex.lock();
    if(!*handles_clients) {
      break;
    }
    thread_mutex.unlock();
  }
  pthread_join(t1,NULL);
  delete (handles_clients);
}
