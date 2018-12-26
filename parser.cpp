//
// Created by Matan on 18/12/2018.
//

#include "parser.h"
# define SKIP_STRING 1
void Parser::initializeCommandMap(DataHandler* d_h) {
  auto command_expression = new CommandExpression (new DefineVarCommand(d_h));
  this->command_map["var"] = command_expression;
  this->delete_vector.push_back(command_expression);
  command_expression =new CommandExpression (new EqualCommand(d_h));
  this->command_map["="] =command_expression ;
  this->delete_vector.push_back(command_expression);
  command_expression = new CommandExpression(new PrintCommand(d_h));
  this->command_map["print"] = command_expression;
  this->delete_vector.push_back(command_expression);
  command_expression = new CommandExpression (new SleepCommand(d_h));
  this->command_map["sleep"] = command_expression;
  this->delete_vector.push_back(command_expression);
  command_expression = new CommandExpression(new OpenDataServer(d_h));
  this->command_map["openDataServer"]= command_expression;
  this->delete_vector.push_back(command_expression);
  command_expression = new CommandExpression(new Enterc(d_h));
  this->command_map["Enterc"]= command_expression;
  this->delete_vector.push_back(command_expression);
  command_expression = new CommandExpression(new Connect(d_h));
  this->command_map["connect"]= command_expression;
  this->delete_vector.push_back(command_expression);
  this->command_map["if"] = nullptr;
  this->command_map["while"] = nullptr;
}

Expression * Parser::initializeConditionCommand(DataHandler *d_h , int& i ,string condition) {
  string token;
  ConditionParser* condition_command;
  CommandExpression * command_expression;
  if(condition == "if") {
    condition_command = new IfCommand(d_h);
  } else {
    condition_command = new LoopCommand(d_h);
  }
  do{
    // get next lexer symbol
    i++;
    token = d_h->getSymbolString(i);
    if(this->command_map.count(token)) {
      if(token == "if") {
        condition_command->addCommand(initializeConditionCommand(d_h,i,"if"));
      } else if(token == "while") {
        condition_command->addCommand(initializeConditionCommand(d_h,i,"while"));
      }else {
        condition_command->addCommand(this->command_map[token]);
      }
    }
  }while (token != "}");
  condition_command->setSkipCondition(i + d_h->getCurrIndex() + SKIP_STRING);
  command_expression = new CommandExpression(condition_command);
  this->delete_vector.push_back(command_expression);
  return command_expression;
}

void Parser::setConditionCommand(DataHandler *d_h ,string& condition_command) {
  int index =0;
  if(condition_command == "if") {
    this->command_map["if"] = this->initializeConditionCommand(d_h, index ,"if");
  } else {
    this->command_map["while"] = this->initializeConditionCommand(d_h,index,"while");
  }
}

void Parser::run() {
  auto * d_h = new DataHandler(this->string_command,&curr_index);
  this->initializeCommandMap(d_h);
  while(true) {
    if(d_h->isSymbol(string_command[curr_index])) {
      curr_index++;
    }
    string command =string_command[curr_index];
    if(command_map.count(command)){
      if(command =="if" || command == "while") {
        this->setConditionCommand(d_h ,command);
      }
      command_map[command]->calculate();
    } else {
      throw "invalid command";
    }
    if(curr_index >= (int) string_command.size()-1) {
      break;
    }
  }
  this->deleteCommands();
  pthread_t t = d_h->thread;
  delete d_h;
  pthread_join(t,NULL);
}

void Parser::deleteCommands() {
  for(auto c: this->delete_vector) {
    delete (c);
  }
}


