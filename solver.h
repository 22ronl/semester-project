//
// Created by ron on 13/01/19.
//

#ifndef SEMSTERPROJECT_SOLVER_H
#define SEMSTERPROJECT_SOLVER_H

#include "searcher.h"

template <class Problem, class Solution>
class Solver {
public:
    //convert string to matrix.
    virtual Solution solve(Problem problem) = 0;
};


template<class Problem ,class Solution ,class E, class T > class SolverSearcher : public Solver<Problem, Solution> {
private:
    Searcher<E,T>* s;
public:
    SolverSearcher(Searcher<E,T>* searcher) {this->s =searcher;}
    Solution solve(Problem searchable) {
      return s->search(searchable);
    }
};
#endif //SEMSTERPROJECT_SOLVER_H
