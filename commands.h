//
// Created by Matan on 18/12/2018.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#include "maps.h"
#include <list>
#include "expression.h"
#include "command_expression.h"

class DefineVarCommand : public Command {
 public:
  DefineVarCommand(DataHandler* data_handler1) : Command(data_handler1){};
  void doCommand();
};

class EqualCommand : public Command {
 public:
  EqualCommand(DataHandler* data_handler1) : Command(data_handler1){};
  void doCommand();
};

class PrintCommand : public Command {
 public:
  PrintCommand(DataHandler* data_handler1) : Command(data_handler1){};
  void doCommand();
};

class SleepCommand : public Command {
 public:
  SleepCommand(DataHandler* data_handler1) : Command(data_handler1){};
  void doCommand();
};

class ConditionParser : public Command {
 protected:
 list<Expression*> list_command;
 int skip_condition;

 public:
  ConditionParser(DataHandler* data_handler1) : Command(data_handler1){};
  void addCommand(Expression* command_expression);
  bool checkCondition(int condition_place);
  void doCommand()=0;
  void setSkipCondition(int value);

};

class IfCommand :  public ConditionParser {

 public:
  IfCommand(DataHandler* data_handler1) : ConditionParser(data_handler1){};
  void doCommand();
};

class LoopCommand : public ConditionParser {
 public:
  LoopCommand(DataHandler* data_handler1) : ConditionParser(data_handler1){};
  void doCommand();
};

#endif //
