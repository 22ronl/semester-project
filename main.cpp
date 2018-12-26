#include <iostream>
#include "utils.h"
#include "parser.h"
#define NUM_OF_ARGV 2
#define FILE_NAME_INDEX 1
int main(int argc, char *argv[]) {
     if(argc != NUM_OF_ARGV) {
    cout<< "invalid num of CLI"<<endl;
  }
  string file_name = argv[FILE_NAME_INDEX];
  Lexer lex(file_name);
  vector<string> string_command =lex.get_string_command();
  Parser p(string_command);
  try {
    p.run();
  } catch (...) {
   cout<< "failed"<<endl;
  }
  exit(0);
}


