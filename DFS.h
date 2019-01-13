
#ifndef SEMSTERPROJECT_DFS_H
#define SEMSTERPROJECT_DFS_H
#include <cstddef>
#include <vector>
#include <string>
#include <stack>
#include "searchable.h"
#include "searcher.h"
using namespace std;

template<class T>  class DFS : public Searcher<T> {

private:
    State<T> initial_state;
    vector<State<T>> visited;
    int num_of_nodes;
    bool is_fisrt_iteration = true;
public:
    DFS();
    int getNumberOfNodesEvaluated() {
        return this->num_of_nodes;
    }

    bool isVisited(State<T> state) {
        for(const auto & iter:this->visited){
            if(state == iter) {
                return true;
            }
        }
        return false;
    }

    Solution<State<T>> search(Searchable<T> searchable){
        if(this->is_fisrt_iteration) {
            this->initial_state = searchable.getInitialState();
            this->is_fisrt_iteration = false;
        }
        if(searchable.isGoalState(this->initial_state)) {
            return this->visited;
        }

        this->visited.push_back(this->initial_state);
        this->num_of_nodes++;
        std::vector<State<T>> adj_of_v = searchable.getAllPossibleStates(this->initial_state);
        bool is_path_exist = false;
        for (const auto & iter:adj_of_v) {
            if(!isVisited(iter)) {
                this->initial_state = iter;
                search(searchable);
                is_path_exist = true;
            }
        }
        if(!is_path_exist){
            return -1;
        }
    }
};
/*
class DFS : public Searcher<std::pair<int,int>> {

private:
    State<std::pair<int,int>> initial_state;
    vector<State<std::pair<int,int>>> visited;
    int num_of_nodes;
    bool is_fisrt_iteration = true;
public:
    int getNumberOfNodesEvaluated() {
        return this->num_of_nodes;
    }

    bool isVisited(State<std::pair<int,int>> state) {
        for(const auto & iter:this->visited){
            if(state == iter) {
                return true;
            }
        }
        return false;
    }

    vector<State<std::pair<int,int>>> search(MatrixProblem *searchable){
        if(this->is_fisrt_iteration) {
            this->initial_state = searchable->getInitialState();
            this->is_fisrt_iteration = false;
        }
        if(searchable->isGoalState(this->initial_state)) {
            return this->visited;
        }

        this->visited.push_back(this->initial_state);
        this->num_of_nodes++;
        std::vector<State<std::pair<int,int>>> adj_of_v = searchable->getAllPossibleStates(this->initial_state);
        bool is_path_exist = false;
        for (const auto & iter:adj_of_v) {
            if(!isVisited(iter)) {
                this->initial_state = iter;
                search(searchable);
                is_path_exist = true;
            }
        }
        if(!is_path_exist){
           // return -1;
        }
    }
};
*/
#endif //SEMSTERPROJECT_DFS_H
