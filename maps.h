//
// Created by Matan on 18/12/2018.
//

#ifndef SEMSTER_PROJECT_MAPS_H
#define SEMSTER_PROJECT_MAPS_H
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
class DataHandler {
  int* curr_index;
  unordered_map<string,double> symbol_table;
  vector<string> string_command;
  unordered_map<string,double> plane_data;
 public:
  DataHandler(vector<string>& string_command1 ,int* curr_index);
  string getSymbolString(int parm_num);
  void addSymbol(string symbol);
  void setSymbolValue(string symbol, double value);
  bool isSymbol(string & symbol);
  bool addExpressionTokens(string& string1,vector<string>& tokens);
  double getSymbolValue(string& symbol);
  double getExpressionValue();
  bool isNextTokenOp(int index);
  int getCurrIndex();
  void setCurrIndex(int index);
  void increaseCurrIndex(int index);
  bool toPrintString();
  void printString();
};

#endif //SEMSTER_PROJECT_MAPS_H
