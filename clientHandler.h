//
// Created by ron on 13/01/19.
//

#ifndef SEMSTERPROJECT_CLIENTHANDLER_H
#define SEMSTERPROJECT_CLIENTHANDLER_H

#include "cacheHandler.h"
#include "server.h"
#include "solver.h"
#include <string>

class ClientHandler {
public:
    virtual void handelClient(int sock_number) = 0;
};


#endif //SEMSTERPROJECT_CLIENTHANDLER_H
