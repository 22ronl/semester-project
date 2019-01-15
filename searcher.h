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
#include <math.h>
#include <stack>
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
  virtual Solution<E>* search(Searchable<T>* searchable) =0;
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
            return new Solution<E>(vertex);
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
    return left->getCostToGet() < right->getCostToGet();
  }
};

template <class E ,class T> class SearcherAlgo : public Searcher<E,T> {
 private:
  int nodesEvaluated = 0;
  std::multiset<State<T> *, CompareCost<T>> open;
 protected:
  State<T> *goal_state;
 public:
  void setPriority(State<T> *vertex) {
    for (auto &item : open) {
      if (item == vertex) {
        open.erase(item);
        open.insert(vertex);
      }
    }
  }
  State<T> *popOpen() {
    nodesEvaluated++;
    auto it = open.begin();
    State<T> *best = *it;
    open.erase(it);
    return best;
  }
  virtual double getWeightedCost(State<T> *came_from, State<T> *vertex)=0;
  Solution<E>* search(Searchable<T> *searchable) {
    this->goal_state = searchable->getGoalState();
    State<T> *initialState = searchable->getInitialState();
    initialState->setCostToGet(initialState->getCost());
    open.insert(initialState);
    std::set<State<T> *> close;
    State<T> *curr_state;
    std::vector<State<T> *> adj;
    while (!open.empty()) {
      curr_state = popOpen();
      close.insert(curr_state);
      if (searchable->isGoalState(curr_state)) {
        return new Solution<T>(curr_state);
      }
      adj = searchable->getAllPossibleStates(curr_state);
      for (auto &vertex : adj) {
        if (close.find(vertex) == close.end() && open.find(vertex) == open.end()) {
          vertex->setFrom(curr_state);
          //vertex->setCostToGet(curr_state->getCostToGet() + vertex->getCost());
          vertex->setCostToGet(this->getWeightedCost(curr_state,vertex));
          open.insert(vertex);
        } else if (this->getWeightedCost(curr_state,vertex) < vertex->getCostToGet()) {
          vertex->setCostToGet(this->getWeightedCost(curr_state,vertex));
          vertex->setFrom(curr_state);
          if (open.find(vertex) == open.end()) {
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

template <class E ,class T> class BestFirstSearch : public SearcherAlgo<E,T> {
 private:
  double getWeightedCost(State<T> *came_from, State<T> *vertex) {
    return came_from->getCostToGet() + vertex->getCost();
  }
};


template <class E ,class T> class Astar : public SearcherAlgo<E,T> {
 private:
  double getWeightedCost(State<T> *came_from, State<T> *vertex) {
    double vertex_x = vertex->getState().first;
    double vertex_y = vertex->getState().second;
    double goal_x = this->goal_state->getState().first;
    double goal_y = this->goal_state->getState().second;
    double aerial_dis = sqrt(pow(vertex_x - goal_x,2) + pow(vertex_y -goal_y,2));
    return came_from->getCostToGet() + vertex->getCost() +aerial_dis ;
  }
};
template <class E ,class T> class DFS : public Searcher<E,T>{
 private:
  int nodesEvaluated=0;
 public:

  int getNumberOfNodesEvaluated() {
    return nodesEvaluated;
  }

  Solution <E> search(Searchable <T> *searchable) {

    std::set <State<T>*> visited_nodes;
    std::stack <State<T>*> route; // route to target
    std::vector <State<T>*> adj_nodes;

    State <T>* current_node = searchable->getInitialState();
    nodesEvaluated = 1;
    while (!searchable->isGoalState(current_node)) {
      adj_nodes = searchable->getAllPossibleStates(current_node);

      bool found_non_visited = false;
      for (auto node : adj_nodes) {
        if (visited_nodes.find(node) == visited_nodes.end()) {
          nodesEvaluated++;
          if(searchable->isGoalState(node)) {
            node->setFrom(current_node);
            return Solution<E>(node);
          }
          node->setFrom(current_node);
          route.push(current_node);
          visited_nodes.insert(node);
          current_node = node;
          found_non_visited = true;
          break;
        }
      }

      if (found_non_visited) {
        continue;
      }

      if (route.empty()) {
        return nullptr;
      }
      current_node = route.top();
      route.pop();
    }
  }
};
#endif //SEMSTERPROJECT_SEARCHER_H
