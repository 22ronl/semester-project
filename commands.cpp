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
#include <cstdlib>
#include <pthread.h>
#define NEXT_STRING_POS 1
#define NEXT_COMMAND_POS 2
#define NEXT_COMMAND_POS_1 1
#define NEXT_COMMAND_POS_3 3
#define VAR_POS -1
#define PATH_INDEX 2
#define CURR_SYMBOL_INDEX 0
#define BUFFER_SIZE 1024
#define UNIX_END_OF_LINE '\n'
#define FIRST_CONDITION_COMMAND_INDEX 2
#define RPM_MIN_VALUE 760
void DefineVarCommand::doCommand() {
  string var = this->data_Handler->getSymbolString(NEXT_STRING_POS);
  this->data_Handler->addSymbol(var);
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS);
}
void EqualCommand::doCommand() {

  string var = this->data_Handler->getSymbolString(VAR_POS);
  double val;
  string is_bind = this->data_Handler->getSymbolString(NEXT_COMMAND_POS_1);
  if(is_bind == "bind") {
    string path_or_var =  this->data_Handler->getSymbolString(PATH_INDEX);
    //check if var already exist.
    if(this->data_Handler->isPath(path_or_var)) {
      path_or_var = path_or_var.substr(1, path_or_var.length()-2);
      this->data_Handler->addPathToTable(var,path_or_var);
      if(this->data_Handler->plane_data.count(path_or_var)) {
        this->data_Handler->addToUpdateFromSimulator(var,path_or_var);
        //this->data_Handler->setVarPathValue(var);
      }
    } else {
      this->data_Handler->addPathToTable(var,data_Handler->getVarPath(path_or_var));
      if(this->data_Handler->plane_data.count(data_Handler->getVarPath(path_or_var))) {
        this->data_Handler->addToUpdateFromSimulator(var, data_Handler->getVarPath(path_or_var));
        //this->data_Handler->setVarPathValue(var);
      } else {
        this->data_Handler->setSymbolValue(var,data_Handler->getSymbolValue(path_or_var));
      }
    }
    this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_3);
  }else {
    val = this->data_Handler->getExpressionValue();
    this->data_Handler->setSymbolValue(var, val);
    if(this->data_Handler->isBinded(var)) {
      this->setValueInSimulator(this->data_Handler->getVarPath(var),val);
    }
    this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
  }

}

void EqualCommand::setValueInSimulator(string serverPath, double value) {
  string massage = "set " + serverPath +" " + to_string(value) + "\r\n";
  //char buffer[1024];
  //strcpy(buffer, massage.c_str());
  int rc = send(this->data_Handler->getClientSocket(), massage.c_str(),strlen(massage.c_str()), 0);
  //cout<<"sent rc" + to_string(rc) <<endl;
}


//void EqualCommand::doCommand() {
 // string var = this->data_Handler->getSymbolString(VAR_POS);
 // double val = this->data_Handler->getExpressionValue();
 // this->data_Handler->setSymbolValue(var, val);
 // this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
//}

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

bool dataParser(vector<string>& result,string sim_input,string& remainder ) {
  string var_value;
  remainder.clear();
  int i=0;
  bool is_end_of_line = false;
  bool cut_middle_of_input = false;
  while(i < sim_input.length()) {
    while(sim_input[i] != ',' )  {
       if(sim_input[i] == UNIX_END_OF_LINE) {
         is_end_of_line = true;
         break;
       }
      var_value += sim_input[i];
      i++;
      if(i >= sim_input.length()) {
        cut_middle_of_input= true;
        break;
      }
    }
    if (cut_middle_of_input) {
      break;
    }
    result.push_back(var_value);
    var_value.clear();
      if(sim_input[i] == UNIX_END_OF_LINE) {
        is_end_of_line = true;
        break;
      }
    i++;
  }

  if(!is_end_of_line) {
    remainder = var_value;
    return false;
  }
  remainder = sim_input.substr(i+1 ,sim_input.length() -i );
  return true;
}
void updateData(char * buffer , DataHandler * d_h, int input_size) {

  bool full_input;
  buffer[input_size] ='\0';
  string sim_input = d_h ->input_remainder + buffer;


  full_input = dataParser(d_h->plane_data_input ,sim_input ,d_h->input_remainder);
  if(full_input) {
    for (int i = 0; i < d_h->plane_data_input.size(); i++) {
      d_h->updatePlaneData(d_h->plane_data_list[i], stof(d_h->plane_data_input[i]));
    }
    d_h->rpm = stof(d_h->plane_data_input[d_h->plane_data_input.size() -1]);
    d_h->plane_data_input.clear();
  }
}

void* ThreadGetPlaneData(void *param) {
  auto params = (struct Params*) param;
  int n;
  char buffer[BUFFER_SIZE];
  while (params->d_h->reading_data) {

      bzero(buffer, BUFFER_SIZE);
      n =read(params->newsockfd, buffer, BUFFER_SIZE-1);
      if (n < 0) {
        perror("ERROR reading from socket");
       exit(1);
      }
      updateData(buffer,params->d_h,n);

  }

}
bool valid_value(string& input) {
  for(int i=0 ; i<input.length()-1 ; i++) {
    if(input[i] != '0' && input[i] != ',' && input[i] !='.' && input[i] != '1' ){
      return true;
    }
  }
  return false;
}
void wait_for_valid_value(int newsockfd) {
  char buffer[BUFFER_SIZE];
  string sim_input;
  int n;
  while (true) {
    bzero(buffer, BUFFER_SIZE);
    n =read(newsockfd, buffer, BUFFER_SIZE-1);
    if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
    }
    buffer[n] ='\0';
    sim_input = buffer;
    if(valid_value(sim_input)) {
      break;
    }
  }
}

void OpenDataServer::doCommand() {
  int sockfd, newsockfd, portno, clilen;
  char buffer[BUFFER_SIZE];
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
  if (newsockfd < 0) {
    perror("ERROR on accept");
    exit(1);
  }

  wait_for_valid_value(newsockfd);
  this->data_Handler->server_socket =newsockfd;
  close(sockfd);
  /* If connection is established then start communicating */

  //bzero(buffer,BUFFER_SIZE);
  //n = read( newsockfd,buffer,BUFFER_SIZE-1 );
  //if (n < 0) {
   // perror("ERROR reading from socket");
   // exit(1);
  //}
  //printf("Here is the first message: %s\n",buffer);
  DataHandler * d_h = this->data_Handler;

  thread_params.newsockfd = newsockfd;
  thread_params.d_h = d_h;
  pthread_t t1;
  pthread_create(&t1, nullptr,ThreadGetPlaneData,(void*)&thread_params);
  this->data_Handler->thread = t1;
  //if (n < 0) {
   // perror("ERROR reading from socket");
    //exit(1);
  //}

  //

  // will need to deal with writing every ten seconds
  this->data_Handler->getExpressionValue();
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);

}

void Connect::doCommand() {
  int sockfd, portno;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  std::string ip = this->data_Handler->getSymbolString(NEXT_COMMAND_POS_1);
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
  portno  = (int) this->data_Handler->getExpressionValue();


  /* Create a socket point */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }
  char* write_able = new char[ip.size()+ 1];
  std::copy(ip.begin(), ip.end(), write_able);
  write_able[ip.size()] = '\0';
  server = gethostbyname(write_able);

  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  /* Now connect to the server */
  if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR connecting");
    exit(1);
  }
  this->data_Handler->setClientSocket(sockfd);
  //while(true) {
    //cout<< "rpm is : " + to_string(this->data_Handler->rpm)<<endl;
    //if(this->data_Handler->rpm > RPM_MIN_VALUE) {
     // break;
   // }
 // }
  delete[] write_able;
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
}

void Enterc::doCommand() {
  string input;
  cout<<"please press any character to continue"<<endl;
  cin >> input;
  this->data_Handler->increaseCurrIndex(NEXT_COMMAND_POS_1);
}