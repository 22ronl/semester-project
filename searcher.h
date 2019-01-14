//
// Created by Matan on 11/01/2019.
//

#ifndef SEMSTERPROJECT_SEARCHER_H
#define SEMSTERPROJECT_SEARCHER_H
#include "searchable.h"
#include "queue"
#include <set>
#include <algorithm>
#include <queue>
#include <set>
// assume every soultion needs a vetor
template <class T> class Solution {
 private:
  State<T>* goal;
  std::vector<State<T>*> solution;
 public:
  Solution(State<T>* goal) { this->goal =goal;}
  void addSolutionElement(State<T>* element) {
    solution.push_back(element);
    std::rotate(solution.rbegin(),solution.rbegin()+1,solution.rend());
    }
  std::vector<State<T>*> getSolution() {
    State<T>* curr_state = goal;
    do {
      this->addSolutionElement(curr_state);
      curr_state = curr_state->getFrom();
    } while (curr_state != nullptr);
    return solution;
  }
};

template<class E,class T>  class Searcher {
 public:
  virtual Solution<E> search(Searchable<T>* searchable) =0;
  virtual int getNumberOfNodesEvaluated()=0;
};

template <class E ,class T> class BFS : public Searcher<E,T> {
 private:
  int nodesEvaluated=0;
 public:
  Solution<E> search(Searchable<T>* searchable) {
    // 1 for source
    nodesEvaluated=1;
    //Solution<E> solution;
    std::set<State<T>*> visited;
    std::queue<State<T>*> bfs_queue;
    State<T>* curr_state = searchable->getInitialState();
    std::vector<State<T>*> adj;
    visited.insert(curr_state);
    bfs_queue.push(curr_state);
    while (!bfs_queue.empty()) {
      curr_state = bfs_queue.front();
      bfs_queue.pop();
      adj = searchable->getAllPossibleStates(curr_state);
      for(auto& vertex : adj ){
        // if we didnt visit
        if(visited.find(vertex) == visited.end()) {
          ++nodesEvaluated;
          if(searchable->isGoalState(vertex)) {
            vertex->setFrom(curr_state);
            return Solution<E>(vertex);
          }
          vertex->setFrom(curr_state);
          visited.insert(vertex);
          bfs_queue.push(vertex);
        }
      }
    }
    return nullptr;
  }
  int getNumberOfNodesEvaluated() {
    return nodesEvaluated;
  }
};



/**
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
 **/

template <class T> struct CompareCost {
 public:
  bool operator()(State<T> * left ,State<T> * right) const {
    return left->getCost() < right->getCost();
  }
};


template <class E ,class T> class BestFirstSearch : public Searcher<E,T> {
 private:
  int nodesEvaluated = 0;
  std::multiset<State<T>*,CompareCost<T>> open;
 public:
  void setPriority(State<T>* vertex) {
    for(auto & item : open ) {
      if(item == vertex) {
        open.erase(item);
        open.insert(vertex);
      }
    }
  }
  State<T>* popOpen() {
    nodesEvaluated++;
    auto it = open.begin();
    State<T>* best = *it;
    open.erase(it);
    return best;
  }
  Solution<E> search(Searchable<T> *searchable) {

    open.insert(searchable->getInitialState());
    std::set<State<T>*> close;
    State<T>* curr_state;
    std::vector<State<T>*> adj;
    while (!open.empty()) {
      curr_state = popOpen();
      close.insert(curr_state);
      if(searchable->isGoalState(curr_state)) {
        return Solution<T>(curr_state);
      }
      adj = searchable->getAllPossibleStates(curr_state);
      for(auto& vertex : adj) {
        if(close.find(vertex) == close.end() && open.find(vertex) == open.end()) {
          vertex->setFrom(curr_state);
          vertex->setCostToGet(curr_state->getCostToGet() + vertex->getCost());
          open.insert(vertex);
        } else if(curr_state->getCostToGet() + vertex->getCost() > vertex->getCostToGet()) {
          vertex->setCostToGet(curr_state->getCostToGet() + vertex->getCost());
          vertex->setFrom(curr_state);
          if(open.find(vertex) == open.end()) {
            open.insert(vertex);
          } else {
            this->setPriority(vertex);
          }
        }


      }

    }





  }
  int getNumberOfNodesEvaluated() {
    return nodesEvaluated;
  }

};
#endif //SEMSTERPROJECT_SEARCHER_H
