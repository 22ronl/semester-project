//
// Created by Matan on 16/12/2018.
//

using namespace std;
#include <stack>
#include <string>
#include <vector>
#include <queue>
#include <map>




#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "command_expression.h"

class Number :public Expression {
 private:
  double value;
 public:
  Number(double num);
  double calculate();
};

class BinaryExpression : public Expression {
 protected:
  Expression* left_exp;
  Expression* right_exp;

 public:
  BinaryExpression(Expression* left, Expression* right);
  Expression* getLeftExp(){ return this->left_exp;}
  Expression* getRightExp(){ return this->right_exp;}
  double calculate() =0;
  ~BinaryExpression(){};

};

class Plus : public BinaryExpression {
 public:
  double calculate();
  Plus(Expression* right ,Expression* left) : BinaryExpression(right,left){};
};

class Mul : public BinaryExpression {
 public:
  Mul(Expression* left, Expression* right) : BinaryExpression(left, right){};
  double calculate();
};

class Div : public BinaryExpression {
 public:
  Div(Expression* left, Expression* right) : BinaryExpression(left, right){};
  double calculate();
};

class Min : public BinaryExpression {
 public:
  Min(Expression* left, Expression* right) : BinaryExpression(left, right){};
  double calculate();
};
stack<string> infixToPostfix(vector<string>& tokens);
Expression *createExpression(stack<string>& postfix);
void deleteExpression(Expression * e);


class CommandExpression : public Expression {
 private:
    Command * command;
 public:
    CommandExpression(Command * command1) {this->command = command1;}
    ~CommandExpression() {delete command;}
    // copy constructor
    CommandExpression(const CommandExpression& c_e): command(c_e.command) {};
    CommandExpression& operator=(const CommandExpression& other);
    double calculate();


};
#endif //EXPRESSION_H