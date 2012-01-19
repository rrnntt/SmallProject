#ifndef GOBLIN_DBASE_H
#define GOBLIN_DBASE_H

#include "Goblin/DllExport.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>

namespace Goblin
{

using namespace std;

typedef string db_field_type;

const db_field_type db_int     = "i";
const db_field_type db_float   = "f";
const db_field_type db_double  = "d";
const db_field_type db_pointer = "p";
const db_field_type db_string  = "s";

class GOBLIN_EXPORT db_field_type_register;

class GOBLIN_EXPORT db_basic_field{
public:
  static db_field_type_register types;
  db_basic_field():assign(0),print(0){}
  virtual ~db_basic_field(){}
  db_basic_field(db_field_type typ){type = typ;}
  db_field_type type;
  string name;
  void (*assign)(void*,const string,const string);
  void (*print)(ostream&,void*);
  virtual void insert(size_t i){}
  virtual void erase(size_t i){}
  virtual void erase(vector<int> &indx){}
  virtual size_t add(){return 0;}
  virtual size_t size(){return 0;}
  virtual size_t capacity(){return 0;}
  virtual void*  data_addr(size_t i){return 0;}
  virtual void  sort(vector<size_t>& indx){}
  virtual void  clear(){}
};

template<class T>
class db_field:public db_basic_field{
public:
  db_field(){}
  db_field(db_field_type typ):db_basic_field(typ){}
  vector<T> data;
  virtual void insert(size_t i){
    size_t j = 0;
    for(typename vector<T>::iterator t=data.begin();t!=data.end();t++,j++)
       if (i == j){
          data.insert(t,T());
          break;
       };
  }
  virtual void erase(size_t i){
    size_t j = 0;
    for(typename vector<T>::iterator t=data.begin();t!=data.end();t++,j++)
       if (i == j){
          data.erase(t);
          break;
       };
  }
  virtual void erase(vector<int> &indx){
    size_t j = 0;
    if (size() != indx.size()) return;
    for(typename vector<T>::iterator t=data.begin();t!=data.end();j++)
       if (!indx[j]){
          data.erase(t);
       }else t++;
  }
  virtual size_t add(){data.push_back(T());return size()-1;}
  virtual size_t size(){return data.size();}
  virtual size_t capacity(){return data.capacity();}
  virtual void*  data_addr(size_t i){if (i<data.size()) return &data[i];else return 0;}
  virtual void  sort(vector<size_t>& indx){
    vector<T> tmp(data.size());
    for(size_t i=0;i<data.size();i++) tmp[i] = data[indx[i]];
    for(size_t i=0;i<data.size();i++) data[i] = tmp[i];
  };
  virtual void  clear(){data.clear();}
};

template<class T>
db_basic_field* allocate_field(size_t cap){
  db_field<T> *p = new db_field<T>();
  if (p && cap) {
//    p->data.reserve(cap);
    p->data.resize(cap);
    for(size_t i=0;i<cap;i++) p->data[i] = T();
  };
  return p;
}

template<class T>
void print_field(ostream& s,void* p){
  s<<*(T*)p<<' ';
}


class GOBLIN_EXPORT db_field_type_register_record{
public:
  db_field_type_register_record(string nam,db_basic_field *(*allo)(size_t cap),void (*a)(void*,const string,const string),
                                void (*p)(ostream&,void*)){
    name = nam;
    allocator = allo;
    assign = a;
    print = p;
  }
  string name;
  db_basic_field *(*allocator)(size_t cap);
  void (*assign)(void*,const string,const string);
  void (*print)(ostream&,void*);
};

class GOBLIN_EXPORT db_field_type_register{
  public:
  static vector<db_field_type_register_record> types;

  db_field_type_register();
  db_basic_field* allocate(string name,size_t cap);
  static void assign(db_field_type& tt,void* p,const string m,const string v);
  static void print(db_field_type& tt,ostream& s,void* p);
  static void register_type(string nam,db_basic_field *(*allo)(size_t cap),void (*a)(void*,const string,const string),
                            void (*p)(ostream&,void*)){
    types.push_back(db_field_type_register_record(nam,allo,a,p));
  }

  static bool registed(db_field_type typ){
//    cerr<<'['<<typ<<"]\n";
    for(size_t i=0;i<types.size();i++)
      if (types[i].name == typ) return true;
//    cerr<<"false\n";
    return false;
  }

};

struct GOBLIN_EXPORT fmt_record{
  db_field_type type;
  string name;
  string member;
  string value;
  int field;
  char bra,ket;
  fmt_record():type(),name(),member(),field(),bra(),ket(){}
  fmt_record(string str);//  %f:height %f[10]:line %vjkg['']:q    %vjkg[()]:q.ka
  void skip(size_t n=0,char b=0,char k=0);
};

class GOBLIN_EXPORT dbase {
public:
  vector<db_basic_field*> fields;
  map<string,string> params;
	dbase();
	virtual ~dbase();
	
	db_basic_field *getField(db_field_type type,string str){
	  for(size_t i=0;i<fields.size();i++)
   	  if (fields[i]->type == type && fields[i]->name == str)
   	      return fields[i];
    return 0;
	}
	
	template<class T>
	db_field<T> *getField(string type,string str){
	  string st(type),ss(str);
	  for(size_t i=0;i<fields.size();i++)
   	  if (fields[i]->type == st && fields[i]->name == ss)
   	      return (db_field<T>*)fields[i];
    return 0;
	}

	template<class T>
	db_field<T> *getFieldNew(string type,string name){
	  db_field<T> *p = getField<T>(type,name);
	  if (!p) {
	    newField(type,name);
	    p = getField<T>(type,name);
	    p->data.resize(size());
	  };
	  return p;
	}

			
	size_t size(){return fields.size()?fields[0]->size():0;}
	size_t capacity(){return fields.size()?fields[0]->capacity():0;}
	size_t fields_size(){return fields.size();}
	size_t curr_;
	size_t first(){return curr_ = 0;}
	size_t curr()const{return curr_;}
	void setCurr(size_t i){if (i < size()) curr_ = i;}
	size_t next(){return curr_<size()-1?++curr_:curr_;}
	size_t prev(){return curr_>0?--curr_:curr_;}
	
	void newField(db_field_type typ,string name);
  void deleteField(db_field_type typ,string name);
  void deleteFields();

  size_t addRow();
  void insertRow(size_t i);
  void eraseRow(size_t i);
  void eraseRows(vector<int> &indx);
  void loadFromFile(const string fn);
  bool existsField(db_field_type typ, string name);
  void saveToFile(const string fn,vector<int> *flt=0);
  virtual void apply_params();

  template<class T>
  void sort_rows(string type,string name){ // arguments: type and name of the key field
     multimap<T,size_t> b;
     vector<size_t> indx(size());
     db_field<T> *p = getField<T>(type,name);
     if (!p) return;
     size_t i=0;
     for(i=0;i<size();i++){
        b.insert(make_pair(p->data[i],i));
     };
     typename multimap<T,size_t>::iterator ib;
     i = 0;
     for(ib=b.begin();ib!=b.end();ib++,i++){
        indx[i] = ib->second;
     };
     for(i=0;i<fields.size();i++) {
       fields[i]->sort(indx);
     };
  };
  void clear();
  void type_name(const string typnam,string& typ,string& nam);
  void moveField(size_t,size_t);

};

} // Goblin

#endif // GOBLIN_DBASE
