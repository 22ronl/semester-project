#include <iostream>
#include <fstream>
#include "utils.h"
#include "parser.h"
#include "searchable.h"
void changeFile(string & file_name);
#define NUM_OF_ARGV 2
#define FILE_NAME_INDEX 1
int main(int argc, char *argv[]) {
  string file_name= "matrix.txt";
  //changeFile(file_name);
  vector<MatrixProblem> mp = createMatrixProblem(file_name);
  cout<<"kaka";
  //State<string> a("b");
  //State<string> b("b");
  //State<string> c("c");
  //cout<< to_string((a == b)) + " excpect 1" << endl;
  //cout<< to_string((b==c)) + " excpect 0" << endl;
 // string to_split = "3,4";
 // string del =",";
 // vector<string> result = split(del,to_split);

//     if(argc != NUM_OF_ARGV) {
//    cout<< "invalid num of CLI"<<endl;
//  }
//  string file_name = argv[FILE_NAME_INDEX];
//  Lexer lex(file_name);
//  vector<string> string_command =lex.get_string_command();
//  Parser p(string_command);
//  try {
 //   p.run();
 // } catch (...) {
 //  cout<< "failed"<<endl;
 // }
  return 0;
}


void changeFile(string & file_name) {
  ifstream myFile(file_name);
  ofstream fileWrite("matrix.txt");
  if(!(fileWrite.is_open())) {
    throw "could not open file";
  }
  if(!(myFile.is_open())) {
    throw "could not open file";
  }
  string line;
  string copy;
  size_t size ;
  size_t change;
  while (getline(myFile,line)) {
    size = line.length();
    copy=line;
    change =0;
    for(size_t i=0 ; i< size;i++) {

      if(line[i] == '-') {
        copy.insert(i +change," ");
        change++;
      } else if(i != size-2 && line[i]=='1' && line[i+1] == '0'){
        copy.insert(i+ change," ");
        change++;
      }
    }
    copy.append("\n");
    fileWrite<< copy;
  }
  myFile.close();
  fileWrite.close();
}