//
// Created by ron on 13/01/19.
//

#ifndef SEMSTERPROJECT_SERVER_H
#define SEMSTERPROJECT_SERVER_H
#include "clientHandler.h"
#include <string>

class string;
namespace server_side {
    class Server {
    protected:
//        virtual void start(int port, ClientHandler c) = 0;
        virtual void stop() = 0;
        virtual int getClientSocket(int primeSocket) = 0;
        virtual int getPrimeSocket(int primeSocket) = 0;
        virtual void closeSocket(int socket) = 0;
    public:
        string reciverFromClient(int clientSocket);
        void sendToClient(int socket);
    };
}

#endif //SEMSTERPROJECT_SERVER_H
