//
// Created by Matan on 16/12/2018.
//
#include <string>
#include <vector>

using namespace std;
#ifndef UTILS_H
#define UTILS_H

class Lexer {
 private:
  vector<string> string_command;
  string file_name;
 public:
  Lexer(const string& file_name);
  void create_string_command();
  vector<string> get_string_command();
  void print_vector();


};

#endif //UTILS_H
