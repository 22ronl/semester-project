//
// Created by ron on 13/01/19.
//

#ifndef SEMSTERPROJECT_SERVER_H
#define SEMSTERPROJECT_SERVER_H


namespace server_side {
    class Server {
     public:
        virtual void open(int port) = 0;
        virtual void stop() = 0;
        //virtual int getClientSocket(int port) = 0;
        //virtual void closeSocket(int socket) = 0;
    };
}

class MyParallelServer : public server_side::Server{
 private:
  void open(int port);
  void stop() {};
  //int getPrimeSocket(int primeSocket)
};

#endif //SEMSTERPROJECT_SERVER_H
