//
// Created by Matan on 18/12/2018.
//



#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <string>
#include <unordered_map>
#include "commands.h"
#include "expression.h"
using namespace std;
class Parser {
 private:
  int curr_index=0;
  vector<string> string_command;
  vector<Command*> delete_vector;
  unordered_map<string,Expression *> command_map;

 public:
  Parser(vector<string>& string_command){this->string_command = string_command;}
  void initializeCommandMap(DataHandler * d_h);
  void run();
  Expression * initializeConditionCommand(DataHandler * d_h ,int& index,string condition);
  void setConditionCommand(DataHandler* d_h ,string& condition_command);
  void deleteCommands();

};

#endif //PARSER_H
