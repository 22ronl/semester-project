//
// Created by Matan on 11/01/2019.
//

#ifndef SEMSTERPROJECT_SEARCHER_H
#define SEMSTERPROJECT_SEARCHER_H
#include "searchable.h"
#include "queue"
#include <set>
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

template<class E,class T>  class Searcher {
 public:
  virtual Solution<E> search(Searchable<T>* searchable) =0;
  virtual int getNumberOfNodesEvaluated()=0;
};

template <class E ,class T> class BFS : public Searcher<E,T> {
  Solution<E> search(Searchable<T>* searchable) {
    Solution<E> solution;
    std::set<State<T>> visited;
    State<T> curr_state = searchable->getInitialState();
    visited.insert(curr_state);

    return solution;
  }
  int getNumberOfNodesEvaluated() {
    return 1;
  }
};




template<class E ,class T> class BestFirstSearch : public Searcher<E,T> {
 public:
  struct Compare {
    bool operator()(const State<T>& left,const State<T>& right ) {
      return left  > right ;
    }
  };
  std::priority_queue<State<T>, std::vector<State<T>> ,Compare> open;
  typedef typename
  std::priority_queue<
      State<T>,
      std::vector<State<T>>,
      Compare>::container_type::const_iterator const_iterator;
  const_iterator find(const State<T>&val) const
  {
    auto first = open.begin();
    auto last = open.end();
    while (first!=last) {
      if (*first==val) return first;
      ++first;
    }
    return last;
  }

  Solution<State<E>> search(Searchable<T>* searchable) {
    Solution<E> solution;
    //auto cmp = [](State<T> left, State<T> right) { return left  > right ;};
    //std::priority_queue<State<T>, std::vector<State<T>>, decltype(cmp)> open(cmp);
    //this->open.
    // first to open
    std::set<State<T>> close;
    std::vector<State<T>> possible_states;
    State<T> curr_state;
    State<T> came_from;
    const_iterator state_it;
    open.push(searchable->getInitialState());
    while(!open.empty()) {
      curr_state = open.pop();
      close.insert(curr_state);
      if(searchable->isGoalState(curr_state)) {
        do {
          solution.addSolutionElement(curr_state);
          came_from = curr_state.getFrom();
        } while (came_from!= nullptr);
        return solution;
      }
      possible_states = searchable->getAllPossibleStates(curr_state);
      for(auto& state : possible_states) {
        state_it = this->find(state);
        if(!close.count(state) && state_it == open.end()) {
          state.setFrom(curr_state);
          state.setCostToGet(state.getCost() + curr_state.getCostToGet());
          open.push(state);
        } else if (state.getCostToGet() > curr_state.getCostToGet() + state.getCost()) {
          if(state_it == open.end()) {
            open.push(state);
          } else {
            *state_it.setCostToGet(state.getCost() + curr_state.getCostToGet());
          }
        }
      }


    }
    return solution;
  }
  int getNumberOfNodesEvaluated();
};
#endif //SEMSTERPROJECT_SEARCHER_H
