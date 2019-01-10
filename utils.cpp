//
// Created by Matan on 16/12/2018.
//
#include <iostream>
#include <fstream>
#include "utils.h"
#define END_OF_LINE_WIN "\r\n"
#define END_OF_LINE_UNIX "\n"
#define END_OF_LINE_MAC "\r"
Lexer::Lexer(const string& file_name) {
  this->file_name = file_name;
  this->create_string_command();
}
void Lexer::create_string_command() {
  ifstream myFile(this->file_name);
  if(!(myFile.is_open())) {
    throw "could not open file";
  }
  string word;
  while (myFile>>word) {
    if(word == END_OF_LINE_MAC ||
       word == END_OF_LINE_UNIX||
       word == END_OF_LINE_WIN) {
      continue;
    } else {
      this->string_command.push_back(word);
    }
  }
}

vector<string> Lexer::get_string_command() {
  return this->string_command;
}

void Lexer::print_vector() {
  for(const string & line : this->string_command) {
    cout<<line<<endl;
  }
}

vector<string> split(string delimiter ,string& to_split){
  vector<string> tokens;
  size_t pos = 0;
  string token;
  while ((pos = to_split.find(delimiter)) != string::npos) {
    token = to_split.substr(0, pos);
    tokens.push_back(token);
    to_split.erase(0, pos + delimiter.length());
  }
  tokens.push_back(to_split);
  return tokens;
}