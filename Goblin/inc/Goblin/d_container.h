#ifndef D_CONTAINER_H
#define D_CONTAINER_H

#include <vector>

namespace Goblin
{

using namespace std;

template<class T>
class data_container{
  vector<T*> data;
public:
  typedef typename vector<T*>::iterator iter;
  typedef T* iterator;
  data_container(){
    curr_ = 0;
  }
  ~data_container(){ clear(); }
  size_t size(){return data.size();}
  void clear(){
    for(size_t i=0;i<data.size();i++) delete data[i];
    data.clear();
  }
  size_t curr_;
  T& add(){
    curr_ = size();
    T* s = new T;
    data.push_back(s);
    return *s;
  }
  void erase(size_t i){if (i<size()) {
     delete data[i];
     size_t j = 0;
     for(typename vector<T*>::iterator d=data.begin();d!=data.end();d++,j++)
       if (i == j){
         data.erase(d);
         break;
       }; 
  }}
  size_t curr(){return curr_;}
  void next(){ if (curr_ < data.size()-1) ++curr_;}
  void prev(){ if (curr_ > 0) --curr_;}
  void toFirst(){curr_ = 0;}
  void toLast(){curr_ = data.size()-1;}
  size_t end(){return size();}

  void curr(size_t i){ if (i<size()) curr_ = i;}
  void setCurr(size_t i){ curr(i);}
  void cycle(){
    if (data.size()<2) return;
    T* s = data[0];
    size_t n = data.size()-1;
    for(size_t i=0;i<n;i++) data[i] = data[i+1];
    data[n] = s;
    prev();
  };

  T& operator()(){ return *data[curr_]; }
  T& operator[](size_t i){return i<size()?*data[i]:*data[curr_];}
};

} // Goblin

#endif

