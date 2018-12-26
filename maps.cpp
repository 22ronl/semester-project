//
// Created by Matan on 18/12/2018.
//

#include "maps.h"
#include <unistd.h>
#define FIRST_CHAR 0
#define ONLY_MINUS_OP 1
#define NEXT_STRING 1
#define SECOND_CHAR 1
#define ONE_CHAR 1
#define ONE_DOT 1
#define FIRST_STRING 0
#define SECOND_STRING 1
#include "expression.h"
#include <map>
#include <iostream>
#include "plane_data.h"
using namespace std;

using namespace std;
string DataHandler::getSymbolString(int parm_num){
  if(*this->curr_index + parm_num >= this->string_command.size()) {
    return "out of bound";
  }
  return this->string_command[*this->curr_index +parm_num];
}

bool isDigit(char& c) {
  return (c >= '0' && c <= '9');
}
bool isBigLetter(char& c) {
  return (c>= 'A' && c <='Z');
}

bool isSmallLetter(char& c) {
  return (c >= 'a' && c <='z');
}

bool isValidVar(string& var) {
  if (var.at(FIRST_CHAR) == '_' || isDigit(var.at(FIRST_CHAR))) {
    return false;
  }

  for(char & c : var) {
    if (!isSmallLetter(c) && !isBigLetter(c) && c !='_' && !isDigit(c)) {
      return false;
    }
  }
  return true;

}

bool isSystemName(string& symbol) {
  map<string,int> system_names{{"var",1} ,{"print",1},{"sleep",1}
  ,{"if",1},{"while",1},{"connect",1},{"openDataServer",1},{"Enterc",1} };
  return (bool) system_names.count(symbol);
}

void DataHandler::addSymbol(string symbol){
  if(!isValidVar(symbol) || this->isSymbol(symbol) || isSystemName(symbol)) {
    throw "invalid var";
  }
  // create a new symbol with zero value
  this->symbol_table[symbol] = 0;
}

DataHandler::DataHandler(vector<string> &string_command1, int *curr_index) {
  this->string_command =string_command1;
  this->curr_index =curr_index;
  for(string s : plane_data_list) {
    this->plane_data.insert(make_pair(s ,vector<string>()));
  }


}
void DataHandler::addPathToTable(string var, string path) {
    this->path_table[var] = path;
}

string DataHandler::getVarPath(string &var) {
  if(this->path_table.count(var)) {
    return this->path_table[var];
  } else {
    throw "invalid var to bind";
  }
}
//void DataHandler::setVarPathValue(string &var) {
 // double var_value;
 // var_value = this->plane_data[this->path_table[var]];
 // this->setSymbolValue(var ,var_value);
//}
bool DataHandler::isPath(string& path) {
  return path[FIRST_CHAR] == '\"';
}
void DataHandler::setSymbolValue(string symbol, double value){
  this->mutex_symbol_table.lock();
  if(!this->symbol_table.count(symbol)) {
    throw "symbol does not exist";
  }
  this->symbol_table[symbol] = value;
  this->mutex_symbol_table.unlock();
}

bool DataHandler::isSymbol(string &symbol) {

  if(this->symbol_table.count(symbol)){
    return true;
  } else {
    return false;
  }
}

bool isOperator(char& c) {
  return (c == '*' ||
          c == '/' ||
          c == '+' ||
          c == '-' );
}

double DataHandler::getSymbolValue(string &symbol) {
  if(!this->isSymbol(symbol)) {
    throw "symbol does not exist";
  }
  return this->symbol_table[symbol];
}

bool isDot (char& c , int& num_of_dots) {
  if(c == '.') {
    num_of_dots++;
    return true;
  } else {
    return false;
  }
}
// because expression can be one liner
bool DataHandler:: addExpressionTokens(string& string1,vector<string>& tokens) {
  string temp_num;
  string temp_name;
  string op;
  int i=0;
  int num_of_dots =0;
  bool is_negative = false;
  bool last_is_operator= false;
  double value;
  if(string1[i] == '-') {
    if(string1.size() == ONLY_MINUS_OP) {
      tokens.push_back(string1);
      return true;
    }
    is_negative = true;
    i++;
  }
  while (i<string1.size()) {
    if(isDigit(string1[i])){
      do {
        temp_num += string1[i];
        i++;
      }while(isDigit(string1[i]) || isDot(string1[i],num_of_dots));
      if(num_of_dots > ONE_DOT) {
        throw "invalid number";
      }
      if(is_negative) {
        temp_num = "-" + temp_num;
        is_negative = false;
      }
      i--;
      tokens.push_back(temp_num);
      temp_num.clear();
      last_is_operator= false;
    } else if(isSmallLetter(string1[i]) || isBigLetter(string1[i])) {
      do {
        temp_name += string1[i];
        i++;
      } while (isSmallLetter(string1[i]) || isBigLetter(string1[i])
              || isDigit(string1[i]) || string1[i] == '_');
      value = this->getSymbolValue(temp_name);
      if(is_negative) {
        value *=-1;
        is_negative = false;
      }
      i--;
      tokens.push_back(to_string(value));
      temp_name.clear();
      last_is_operator= false;
    } else if(string1[i] =='(' || string1[i] == ')') {
      op += string1[i];
      tokens.push_back(op);
      op.clear();
      last_is_operator= false;
    } else if(isOperator(string1[i])) {
      if(string1[i] == '-' && last_is_operator) {
        is_negative = true;
      } else {
        op += string1[i];
        tokens.push_back(op);
        op.clear();
        last_is_operator = true;
      }
    } else {
      throw "invalid token";
    }
    i++;
  }
  return last_is_operator;
}

bool DataHandler::isNextTokenOp(int index) {
  string token = this->getSymbolString(index);
  if(token == "out of bound") {
    return false;
  }
  return isOperator(token[FIRST_CHAR]);
}
void correct_first_minus(vector<string>& tokens) {
  if(tokens[FIRST_STRING] == "-") {
    if(tokens[SECOND_STRING][FIRST_CHAR] == '-') {
      tokens[SECOND_STRING] = tokens[SECOND_STRING].substr(SECOND_CHAR,tokens[SECOND_STRING].length() -ONE_CHAR);
    } else {
      tokens[SECOND_STRING] = "-" + tokens[SECOND_STRING];
    }
    tokens.erase(tokens.begin());
  }
}
// will put string in index of the parm we return his value
double DataHandler::getExpressionValue() {
  vector<string> expression_token;
  stack<string> postfix_tokens;
  string curr_token;
  bool is_last_op;
  Expression * e;
  double expression_value;
  int i=0;
  do{
    i++;
    curr_token = this->getSymbolString(i);
    is_last_op = this->addExpressionTokens(curr_token ,expression_token);
  }while (is_last_op || this->isNextTokenOp(i+1));
  correct_first_minus(expression_token);
  postfix_tokens = infixToPostfix(expression_token);
  e = createExpression(postfix_tokens);
  expression_value = e->calculate();
  // clear memory;
  deleteExpression(e);
  *this->curr_index += i;

  return expression_value;
}

int DataHandler::getCurrIndex() {
  return *this->curr_index;
}
void DataHandler::setCurrIndex(int index) {
  *this->curr_index = index;
}

void DataHandler::increaseCurrIndex(int num) {
  *this->curr_index+= num;
}

bool DataHandler::toPrintString() {
  return (this->getSymbolString(NEXT_STRING).at(FIRST_CHAR) == '"');
}

void DataHandler::printString() {
  int i=1;
  string to_print = this->getSymbolString(i);
  to_print = to_print.substr(SECOND_CHAR,to_print.size()-ONE_CHAR);
  if(to_print.find('"') == string::npos ) {
    cout<<to_print + " ";
    i++;
    while ((this->getSymbolString(i).find('"') == string::npos)) {
      cout << this->getSymbolString(i) + " ";
      i++;
    }
    to_print = this->getSymbolString(i);
  }
  to_print = to_print.substr(FIRST_CHAR,to_print.size() - ONE_CHAR);
  cout<< to_print<<endl;
  this->increaseCurrIndex(i);
}

void DataHandler::updatePlaneData(string path, double value) {
  if(!this->plane_data[path].empty()) {
    for(const string & var : this->plane_data[path]) {
      this->setSymbolValue(var,value);
    }
  }
}
DataHandler::~DataHandler() {
  reading_data = false;
  close(server_socket);
  close(client_socket);
  pthread_cancel(thread);
}
void DataHandler::addToUpdateFromSimulator(string &var , string path) {
  this->plane_data[path].push_back(var);
}

//void DataHandler::printPlane() {
 // for(auto& x : this->plane_data) {
  //  cout<< x.second << " |";
 // }
  //cout << endl;
//}