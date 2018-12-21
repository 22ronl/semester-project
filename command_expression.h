//
// Created by matsha on 12/21/18.
//

#ifndef COMMAND_EXPRESSION_H
#define COMMAND_EXPRESSION_H

#include "maps.h"

class Command {
protected:
    DataHandler* data_Handler;

public:
    Command(DataHandler* data_handler1){this->data_Handler = data_handler1;}
    virtual void doCommand() =0;
    virtual ~Command(){};
};

class Expression {
public:
    virtual double calculate()=0;
    //virtual Expression* getLeftExp()=0;
    //virtual Expression* getRightExp()=0;
    virtual Expression* getLeftExp(){ return nullptr;}
    virtual Expression* getRightExp(){ return nullptr;}
    virtual ~Expression(){};
};
#endif // COMMAND_EXPRESSION_H
