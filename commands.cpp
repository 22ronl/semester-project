//
// Created by Matan on 18/12/2018.
//

#include <iostream>
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <chrono>
#include <thread>
#define NEXT_STRING_POS 1
#define NEXT_COMMAND_POS 2
#define NEXT_COMMAND_POS_1 1
#define VAR_POS -1
#define FIRST_CONDITION_COMMAND_INDEX 2
#define CURR_SYMBOL_INDEX 0
void DefineVarCommand::doCommand() {
  string var = this->data_Handler->getSymbolString(NEXT_STRING_POS);
  this->data_Handler->addSymbol(var);
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS);
}

void EqualCommand::doCommand() {
  string var = this->data_Handler->getSymbolString(VAR_POS);
  double val = this->data_Handler->getExpressionValue();
  this->data_Handler->setSymbolValue(var, val);
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
}

bool booleanOperatorResult(double first_parm ,string& boolean_operator, double second_parm){
  if(boolean_operator == "<") {
    return first_parm < second_parm;
  } else if(boolean_operator == ">") {
    return first_parm > second_parm;
  } else if(boolean_operator == ">=") {
    return first_parm >= second_parm;
  } else if(boolean_operator == "<=") {
    return first_parm <= second_parm;
  } else if(boolean_operator == "=="){
    return first_parm == second_parm;
  } else if(boolean_operator == "!="){
    return first_parm != second_parm;
  } else {
    throw "invalid boolean operator";
  }
}


bool ConditionParser::checkCondition(int condition_place) {
  this->data_Handler->setCurrIndex(condition_place);
  double first_parm_value = this->data_Handler->getExpressionValue();
  string bool_string = this->data_Handler->getSymbolString(NEXT_STRING_POS);
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
  double second_parm_value = this->data_Handler->getExpressionValue();
  bool result = booleanOperatorResult(first_parm_value,bool_string,second_parm_value);
  this->data_Handler->increaseCurrIndex(FIRST_CONDITION_COMMAND_INDEX);
  return result;
}

void ConditionParser::addCommand(Expression *command_expression) {
  this->list_command.push_back(command_expression);
}
void ConditionParser::setSkipCondition(int value) {
  this->skip_condition = value;
}
void IfCommand::doCommand() {
  int if_index = this->data_Handler->getCurrIndex();
  if(this->checkCondition(if_index)) {
    // index will be set on end of last parm
    string curr_symbol;
    for (auto &c : this->list_command) {
      curr_symbol = data_Handler->getSymbolString(CURR_SYMBOL_INDEX);
      if (data_Handler->isSymbol(curr_symbol)) {
        this->data_Handler->increaseCurrIndex(NEXT_STRING_POS);
      }
      c->calculate();
    }
    this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
  } else {
    this->data_Handler->setCurrIndex(this->skip_condition);
  }
}

void LoopCommand::doCommand() {
  int loop_index = this->data_Handler->getCurrIndex();
  while (this->checkCondition(loop_index)) {
    // index will be set on end of last parm
    string curr_symbol;
    for (auto &c : this->list_command) {
      curr_symbol = data_Handler->getSymbolString(CURR_SYMBOL_INDEX);
      if (data_Handler->isSymbol(curr_symbol)) {
        this->data_Handler->increaseCurrIndex(NEXT_STRING_POS);
      }
      c->calculate();
    }

    this->data_Handler->setCurrIndex(loop_index);
  }
  // skip after while
  this->data_Handler->setCurrIndex(this->skip_condition);
}

void PrintCommand::doCommand() {
  if(this->data_Handler->toPrintString()) {
    this->data_Handler->printString();
  } else {
    std::cout << this->data_Handler->getExpressionValue()<<endl;
  }
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
}

void SleepCommand::doCommand() {
  long sleep_time =(long) this->data_Handler->getExpressionValue();
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
}

struct Params {
  DataHandler * d_h;
  char * buffer;
  int newsockfd;
};

void* threadGetPlaneData(void* arg) {
  struct Params * params = (struct Params*) arg;
  int n;
  //while (true) {
   // bzero(params->buffer,256);
   // n = read(params->newsockfd,params->buffer,255 );
    //if (n < 0) {
    //  perror("ERROR reading from socket");
    //  exit(1);
    //}
  //}
  cout<<"socker number " + params->newsockfd;
}

void OpenDataServer::doCommand() {
  int sockfd, newsockfd, portno, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int  n;

  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }

  /* Initialize socket structure */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno =(int) this->data_Handler->getExpressionValue();
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

  listen(sockfd,5);
  clilen = sizeof(cli_addr);

  /* Accept actual connection from the client */
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen);

  //if (newsockfd < 0) {
   // perror("ERROR on accept");
   // exit(1);
  //}

  /* If connection is established then start communicating */
  //bzero(buffer,256);
  //n = read( newsockfd,buffer,255 );

  struct Params* thread_params = new Params();
  pthread_t t1;
  DataHandler * d_h = this->data_Handler;
  pthread_create(&t1, nullptr,threadGetPlaneData,thread_params);
  if (n < 0) {
    perror("ERROR reading from socket");
    exit(1);
  }

  printf("Here is the message: %s\n",buffer);

  // will need to deal with writing every ten seconds
  this->data_Handler->getExpressionValue();
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);

}