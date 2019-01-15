//
// Created by ron on 13/01/19.
//

#ifndef SEMSTERPROJECT_CACHEHANDLER_H
#define SEMSTERPROJECT_CACHEHANDLER_H
#include "searcher.h"
#include <map>
#include <mutex>
template <class Solution ,class Problem> class CacheManager {
public:
  virtual bool isProblemExist(Problem)=0;
  virtual void save(Problem ,Solution) =0;
  virtual Solution getSolution(Problem)=0;
};

template <class E ,class T>class FileCacheManager:public CacheManager<Solution<E>*,Searchable<T>*> {
 private:
  std::map<Searchable<T>*,Solution<E>*> solution_map;
  std::mutex mutex_solution_map;
 public:
  bool isProblemExist(Searchable<T> * searchable) {
    this->mutex_solution_map.lock();
    bool b= (bool)solution_map.count(searchable);
    this->mutex_solution_map.unlock();
    return b;
  }
  void save(Searchable<T> * searchable,Solution<E>* solution) {
    this->mutex_solution_map.lock();
    this->solution_map[searchable] = solution;
    this->mutex_solution_map.unlock();
  }
  Solution<E>* getSolution(Searchable<T> * searchable) {
    Solution<E>* s;
    this->mutex_solution_map.lock();
    if(this->isProblemExist(searchable)) {
      s = this->solution_map[searchable];
      this->mutex_solution_map.unlock();
      return s;
    } else {
      this->mutex_solution_map.unlock();
      return nullptr;
    }
  }
};
#endif //SEMSTERPROJECT_CACHEHANDLER_H
