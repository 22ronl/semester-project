//
// Created by Matan on 11/01/2019.
//

#ifndef SEMSTERPROJECT_SEARCHER_H
#define SEMSTERPROJECT_SEARCHER_H
#include "searchable.h"
#include "queue"
#include <unordered_set>
#include <algorithm>
// assume every soultion needs a vetor
template <class T> class Solution {
 private:
  std::vector<State<T>> solution;
 public:
  void addSolutionElement(State<T> element) {
    solution.push_back(element);
    std::rotate(solution.rbegin(),solution.rbegin()+1,solution.rend());
  }
};

template<class T>  class Searcher {
 public:
  virtual Solution<T> search(Searchable<T> searchable) =0;
  virtual int getNumberOfNodesEvaluated()=0;
};

template<class T> class BestFirstSearch : public Searcher<T> {
 public:

  Solution<State<T>> search(Searchable<T> searchable) {
    Solution<std::string> solution;
    auto cmp = [](State<T> left, State<T> right) { return left  > right ;};
    std::priority_queue<State<T>, std::vector<State<T>>, decltype(cmp)> open(cmp);
    // first to open
    std::unordered_set<State<T>> close;
    State<T> curr_state;
    State<T> came_from;
    open.push(searchable.getInitialState());
    while(!open.empty()) {
      curr_state = open.pop();
      close.insert(curr_state);
      if(searchable.isGoalState(curr_state)) {
        do {
          solution.addSolutionElement(curr_state);
          came_from = curr_state.getFrom();
        } while (came_from!= nullptr);
      }

    }



  }
  int getNumberOfNodesEvaluated();
};


#endif //SEMSTERPROJECT_SEARCHER_H
