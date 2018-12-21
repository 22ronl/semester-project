//
// Created by Matan on 18/12/2018.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#include "maps.h"
#include <list>
class Command {
 protected:
  DataHandler* data_Handler;

 public:
  Command(DataHandler* data_handler1){this->data_Handler = data_handler1;}
  virtual void doCommand() =0;
  virtual ~Command(){};
};

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
 list<Command*> list_command;
 int skip_condition;

 public:
  ConditionParser(DataHandler* data_handler1) : Command(data_handler1){};
  void addCommand(Command * command);
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
