//
// Created by Matan on 16/12/2018.
//

#include "expression.h"

#include <iostream>
#define ONLY_MINUS_OP 1
Number::Number(double num) {
  this->value = num;
}

double Number::calculate() {
  return this->value;
}

BinaryExpression::BinaryExpression(Expression *left, Expression *right) {
  this->left_exp=left;
  this->right_exp =right;
}

double Plus::calculate() {
  return this->left_exp->calculate() + this->right_exp->calculate();
}

double Min::calculate() {
  return this->left_exp->calculate() - this->right_exp->calculate();
}

double Mul::calculate() {
  return this->left_exp->calculate() * this->right_exp->calculate();
}

double Div::calculate() {
  if(this->right_exp->calculate() == 0.0) {
    throw  "can't divide with 0";
  }
  return this->left_exp->calculate() / this->right_exp->calculate();
}


bool is_num(const string & token) {
  int i=0;
  if(token[i] == '.') {
    return false;
  }
  if(token[i] == '-' ) {
    if(token.size() == ONLY_MINUS_OP) {
      return false;
    }
    i++;
  }
  for(; i<token.size();i++ ) {
    // for a double
    if(token[i] == '.') {
      continue;
    }
    if(token[i] < '0' || token[i] > '9') {
      return false;
    }
  }
  return true;
}

bool isHigherPriority(string curr_token,string stack_token) {
  map<string,int> priority_map = {{"+",1},{"-",1},{"*",2},{"/",2}};
  int curr_token_priority = priority_map[curr_token];
  int stack_token_priority = priority_map[stack_token];
  if(curr_token_priority == stack_token_priority) {
    return (stack_token == "-" || stack_token == "/" || stack_token == "*") ;
  } else {
    return (stack_token_priority > curr_token_priority);
  }
}
// take care of invalid input as well
stack<string> infixToPostfix(vector<string>& tokens){
  // use to find fast if the operand exists
  map<string,int> priority_map = {{"+",1},{"-",1},{"*",2},{"/",2}};
  stack<string> helper_stack;
  queue<string> token_queue;
  stack<string> result_stack;
  string top_token;
  string tok;
  for(const string & token : tokens) {
    if(is_num(token)) {
      token_queue.push(token);
    } else if(token == "(") {
      helper_stack.push(token);
    } else if(token == ")") {
      if(helper_stack.empty()) {
        throw "invalid input";
      }
      while (helper_stack.top() != "(") {
        token_queue.push(helper_stack.top());
        helper_stack.pop();
      }
      helper_stack.pop();
    } else if(priority_map.count(token)) {
      if(!helper_stack.empty()) {
        top_token = helper_stack.top();
        while (!helper_stack.empty()
               &&isHigherPriority(token,top_token)
               && priority_map.count(top_token)) {
          token_queue.push(top_token);
          helper_stack.pop();
          if(!helper_stack.empty()) {
            top_token = helper_stack.top();
          }
        }
      }
      helper_stack.push(token);
      // if not an operator or a num or "(" or ")"
    } else {
      throw "invalid input";
    }
  }

  while (!helper_stack.empty() && priority_map.count(helper_stack.top())) {
    token_queue.push(helper_stack.top());
    helper_stack.pop();
  }
  while (!token_queue.empty()) {
    result_stack.push(token_queue.front());
    token_queue.pop();
  }
  return result_stack;
}

void initializeExpressions(stack<string>& postfix ,Expression *& right_exp ,Expression *& left_exp) {
  postfix.pop();
  right_exp = createExpression(postfix);
  left_exp = createExpression(postfix);
}

Expression *createExpression(stack<string>& postfix) {
  if(postfix.empty()) {
    throw "invalid input";
  }

  if(is_num(postfix.top())) {
    double num = atof(postfix.top().c_str());
    Expression * e = new Number(num);
    postfix.pop();
    return e;
  }
  // if not a num its an operator

  Expression *right_exp;
  Expression *left_exp;
  if(postfix.top() == "+") {
    initializeExpressions(postfix,right_exp,left_exp);
    return new Plus(left_exp ,right_exp);

  }else if(postfix.top() == "-") {
    initializeExpressions(postfix,right_exp,left_exp);
    return new Min(left_exp ,right_exp);

  }else if(postfix.top() == "*") {
    initializeExpressions(postfix,right_exp,left_exp);
    return new Mul(left_exp ,right_exp);
  } else {
    initializeExpressions(postfix,right_exp,left_exp);
    return new Div(left_exp, right_exp);
  }
}


void deleteExpression(Expression * e) {
    if(e->getLeftExp() == nullptr) {
      delete(e);
      return;
    }
    deleteExpression(e->getLeftExp());
    deleteExpression(e->getRightExp());
    delete(e);
}


double CommandExpression::calculate() {
  this->command->doCommand();
  return 0;
}





