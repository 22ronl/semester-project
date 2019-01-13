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


template<T> class SolverSearcher : public Solver {
private:
    Searcher<T> s;

};
#endif //SEMSTERPROJECT_SOLVER_H
