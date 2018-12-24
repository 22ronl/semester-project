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
  unordered_map<string,string> path_table;
  int client_socket;
 public:
  bool reading_data= true;
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
  void updatePlaneData(string path , double value);
  ~DataHandler(){reading_data = false;}
  void printPlane();
  bool isPath(string& path);
  void addPathToTable(string var, string path);
  void setVarPathValue(string &var);
  bool isBinded(string& var) { return (bool) this->path_table.count(var);}
  string getVarPath(string & var);
  int getClientSocket(){ return client_socket;}
  void setClientSocket(int client_socket){this->client_socket = client_socket;}
  std::vector<std::string> plane_data_list{"/instrumentation/airspeed-indicator/indicated-speed-kt",
                                             "/instrumentation/altimeter/indicated-altitude-ft",
                                             "/instrumentation/altimeter/pressure-alt-ft",
                                             "/instrumentation/attitude-indicator/indicated-pitch-deg",
                                             "/instrumentation/attitude-indicator/indicated-roll-deg",
                                             "/instrumentation/attitude-indicator/internal-pitch-deg",
                                             "/instrumentation/attitude-indicator/internal-roll-deg",
                                             "/instrumentation/encoder/indicated-altitude-ft",
                                             "/instrumentation/encoder/pressure-alt-ft",
                                             "/instrumentation/gps/indicated-altitude-ft",
                                             "/instrumentation/gps/indicated-ground-speed-kt",
                                             "/instrumentation/gps/indicated-vertical-speed",
                                             "/instrumentation/heading-indicator/indicated-heading-deg",
                                             "/instrumentation/magnetic-compass/indicated-heading-deg",
                                             "/instrumentation/slip-skid-ball/indicated-slip-skid",
                                             "/instrumentation/turn-indicator/indicated-turn-rate",
                                             "/instrumentation/vertical-speed-indicator/indicated-speed-fpm",
                                             "/controls/flight/aileron",
                                             "/controls/flight/elevator",
                                             "/controls/flight/rudder",
                                             "/controls/flight/flaps",
                                             "/controls/engines/engine/throttle",
                                             "/engines/engine/rpm"};
};

#endif //SEMSTER_PROJECT_MAPS_H
