#include <iostream>
#include "utils.h"
#include "parser.h"
#define NUM_OF_ARGV 2
#define FILE_NAME_INDEX 1
//void dataParser(vector<string>& result , char* buffer_index);
//void updateData(char * buffer , DataHandler * d_h,int input_size);
int main(int argc, char *argv[]) {
 // if(argc != NUM_OF_ARGV) {
   // cout<< "invalid num of CLI"<<endl;
  //}
  //string file_name = argv[FILE_NAME_INDEX];

  //char buffer[] ="0.0000,1.00000,3.00000,0.1111";
  //string lolo = buffer;

  //vector<string> result;
  //dataParser(result ,buffer);
  //int x =5;
  //vector<string> kkaka {"kaka"};
  //DataHandler * d_h = new DataHandler(kkaka, &x);
  //updateData(buffer,d_h,lolo.length());
 // vector<string> tokens;2
  //string s = "-5.3+-3.98";
  //DataHandler d();
  //cout <<addExpressionTokens(s,tokens);
  Lexer lex("example.txt");
  //vector<string> tokens = lex.get_string_command();
  //int a =3;
  //vector<string>t;
  //DataHandler d(tokens ,&a);
  //d.addExpressionTokens(s ,t);
  //lex.print_vector();
  vector<string> string_command =lex.get_string_command();
  Parser p(string_command);
  try {
    p.run();
  } catch (...) {
   cout<< "faild"<<endl;
  }
  //Expression* e= new Plus(new Number(3) , new Mul( new Div(new Number(4), new
    //  Number(2)) , new Number(5)));
  //Expression *e2 = new Min(e,new Number(7));
  //cout<<e2->calculate();
  //vector<string> v{"1","+","-3"};
  //vector<string> v{"(","50","+","(","42","*","(","3","-","5",")",")",")","+","(","4","/","2",")"};
  //vector<string>v{"10","/","2","+","3","*","5","-","10"};
  //stack<string> result = infixToPostfix(v);
  //Expression* e = createExpression(result);
  //cout<<e->calculate();
  //delete_expression(e);
  //while (!result.empty()) {
    //cout<<result.front();
    //result.pop();
  //}
    //pthread_exit(NULL);
  return 0;
}


