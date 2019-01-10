//
// Created by Matan on 08/01/2019.
//

#ifndef SEMSTERPROJECT_SEARCHABLE_H
#define SEMSTERPROJECT_SEARCHABLE_H
#include <vector>
#include <utility>
#include <vector>
#include <string>

template <class T> class State  {
 private:
  T state;
  double cost;
  State* came_from;
 public:
  State(T state) { this->state = state;}
  void setCost(double cost) {this->cost = cost;}
  void setFrom(State * came_form) { this->came_from = came_form;}
  bool operator== (const State& s);
  T getState() { return this->state;}
};
template <class T> bool State<T>::operator==(const State &s) {
  return this->state == s.state;
}

template <class T> class Searchable {
  virtual State<T> getInitialState() =0;
  virtual bool isGoalState(State<T>& s) =0;
  virtual std::vector<State<T>> getAllPossibleStates(State<T>& s) =0;
};


typedef State<std::pair<int,int>> state_pair;
class MatrixProblem : public Searchable<std::pair<int,int>> {
 private:
  std::pair<int,int> initial_state;
  std::pair<int,int> goal_state;
  int matrix_size;
  std::vector<std::vector<state_pair*>> matrix_graph;
 public:
  MatrixProblem(std::pair<int,int> initial,std::pair<int,int> goal , int size,
                std::vector<std::vector<state_pair*>> matrix_graph);
  state_pair getInitialState();
  bool isGoalState(state_pair& s);
  std::vector<state_pair> getAllPossibleStates(state_pair& s);
};

std::vector<MatrixProblem> createMatrixProblem(std::string& file_name);





#endif //SEMSTERPROJECT_SEARCHABLE_H
