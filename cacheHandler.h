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

 class FileCacheManager:public CacheManager<std::string,std::string> {
 private:
  std::map<std::string,std::string> solution_map;
  std::mutex mutex_solution_map;
 public:
  bool isProblemExist(std::string problem) {
    this->mutex_solution_map.lock();
    bool b= (bool)solution_map.count(problem);
    this->mutex_solution_map.unlock();
    return b;
  }
  void save(std::string problem ,std::string solution) {
    this->mutex_solution_map.lock();
    this->solution_map[problem] = solution;
    this->mutex_solution_map.unlock();
  }
  std::string getSolution(std::string problem) {
    std::string solution;
    this->mutex_solution_map.lock();
    if(this->isProblemExist(problem)) {
      solution = this->solution_map[problem];
      this->mutex_solution_map.unlock();
      return solution;
    } else {
      this->mutex_solution_map.unlock();
      return nullptr;
    }
  }
};
#endif //SEMSTERPROJECT_CACHEHANDLER_H
