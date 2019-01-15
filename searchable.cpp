//
// Created by Matan on 08/01/2019.
//

#include <iostream>
#include "searchable.h"
#include "utils.h"
#define MATRIX_NUM_INDEX 0
#define SIZE_INDEX 0
#define START_INDEX 1
#define GOAL_INDEX 2
#define I_INDEX 0
#define J_INDEX 1
#define SKIP_TO_MATRIX 3

MatrixProblem::MatrixProblem(std::pair<int,int> initial,
                             std::pair<int,int> goal,
                             int row_num, int col_num,
                             std::vector<std::vector<state_pair*>> matrix_graph) :
                             initial_state(initial) , goal_state(goal){
  this->row_num = row_num;
  this->col_num =col_num;
  this->matrix_graph = matrix_graph;
}
MatrixProblem::~MatrixProblem() {
  for(int i=0; i<this->row_num ; ++i) {
    for(int j=0 ; j< this->col_num ;++j) {
      delete(this->matrix_graph[i][j]);
    }
  }
}
state_pair* MatrixProblem::getInitialState() {
  return this->matrix_graph[initial_state.first][initial_state.second];
}

bool MatrixProblem::isGoalState(state_pair* &s) {
  return *s == *this->matrix_graph[goal_state.first][goal_state.second];
}

std::vector<state_pair*> MatrixProblem::getAllPossibleStates(state_pair* &s) {
  std::vector<state_pair*> possible_states;
  std::pair<int,int> node = s->getState();
  int i = node.first;
  int j = node.second;
  if(j-1 > 0)  {
    // -1 is a wall
    if(this->matrix_graph[i][j-1]->getCost() != -1) {
      possible_states.push_back(this->matrix_graph[i][j - 1]);
    }
  }
  if (j+1 <= this->col_num -1) {
    if(this->matrix_graph[i][j+1]->getCost() != -1) {
      possible_states.push_back(this->matrix_graph[i][j + 1]);
    }
  }
  if (i-1> 0) {
    if(this->matrix_graph[i-1][j]->getCost() != -1) {
      possible_states.push_back(this->matrix_graph[i - 1][j]);
    }
  }
  if (i+1 <= this->row_num -1) {
    if(this->matrix_graph[i+1][j]->getCost() != -1) {
      possible_states.push_back(this->matrix_graph[i + 1][j]);
    }
  }
  return possible_states;
}

std::pair<int,int> createPair(string& index) {
  std::pair<int,int> node;
  std::vector<std::string> i_and_j;
  i_and_j = split(",",index);
  node.first = stoi(i_and_j[I_INDEX]);
  node.second = stoi(i_and_j[J_INDEX]);
  return node;
}

double getCost(string& cost_string) {
  size_t index = cost_string.find(',');
  if(index != std::string::npos) {
    cost_string.erase(cost_string.begin()+ index);
  }
  return stod(cost_string);
}
std::vector<MatrixProblem*> createMatrixProblem(std::string& file_name) {
  Lexer lexer(file_name);
  vector<MatrixProblem*> matrix_problems;
  std::vector<std::string> matrix_string = lexer.get_string_command();
  MatrixProblem* matrix_problem;
  int matrix_num = stoi(matrix_string[MATRIX_NUM_INDEX]);
  int n;
  std::pair<int,int> initial_index;
  std::pair<int,int> goal_index;
  state_pair* node;

  // start after num of matrix
  int vector_index=1;
  for(int i=0 ; i<matrix_num ;++i) {
    n = stoi(matrix_string[SIZE_INDEX+vector_index]);
    std::vector<std::vector<state_pair*>> matrix_graph(n,std::vector<state_pair*>(n));
    initial_index = createPair(matrix_string[vector_index+ START_INDEX]);
    goal_index = createPair(matrix_string[vector_index + GOAL_INDEX]);
    vector_index+= SKIP_TO_MATRIX;
    for(int j=0;j<n;++j) {
      for(int k=0; k<n;++k){
        std::pair<int,int> indexes(j,k);
        node = new state_pair(indexes);
        node->setCost(getCost(matrix_string[vector_index]));
        matrix_graph[j][k]= node;
        vector_index++;
      }
    }
    matrix_problem = new MatrixProblem(initial_index,goal_index,n,n,matrix_graph);
    matrix_problems.push_back(matrix_problem);
  }
  return matrix_problems;
}