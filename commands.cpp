//
// Created by Matan on 18/12/2018.
//

#include <iostream>
#include "commands.h"
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