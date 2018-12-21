#include <iostream>
#include "utils.h"
#include "expression.h"
#include "parser.h"
#include "maps.h"

int main() {
 // vector<string> tokens;
  string s = "-5.3+-3.98";
  //DataHandler d()
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
  p.run();
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
  return 0;
}