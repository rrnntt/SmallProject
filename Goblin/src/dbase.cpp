#include "Goblin/dbase.h"

#include <fstream>
#include <stdlib.h>
//#include "../Troll1/mio.h"

namespace Goblin
{

using namespace std;

vector<db_field_type_register_record> db_field_type_register::types;

db_field_type_register db_basic_field::types;

void assign_int(void* p,const string nm,const string val){
  *(int*)p = atoi(val.c_str());
}

void assign_float(void* p,const string nm,const string val){
  *(float*)p = atof(val.c_str());
}

void assign_double(void* p,const string nm,const string val){
  *(double*)p = atof(val.c_str());
}

void assign_string(void* p,const string nm,const string val){
  *(string*)p = val;
}

db_field_type_register::db_field_type_register(){
    register_type("i",allocate_field<int>,assign_int,print_field<int>);
    register_type("f",allocate_field<float>,assign_float,print_field<float>);
    register_type("d",allocate_field<double>,assign_double,print_field<double>);
    register_type("s",allocate_field<string>,assign_string,print_field<string>);
//  cerr<<"OK "<<types.size()<<'\n';
}


db_basic_field* db_field_type_register::allocate(string name,size_t cap){
    db_basic_field* p;
//    cerr<<"alloc "<<name<<' '<<cap<<'\n';
    for(size_t typ=0;typ<types.size();typ++)
      if (name == types[typ].name) {
       p = types[typ].allocator(cap);
       p->type = name;
       return p;
      };
    return 0;
}

void db_field_type_register::assign(db_field_type& tt,void* p,const string m,const string v){
  for(size_t typ=0;typ<types.size();typ++)
    if (tt == types[typ].name) {
       types[typ].assign(p,m,v);
       return;
    };
}

void db_field_type_register::print(db_field_type& tt,ostream& s,void* p){
  for(size_t typ=0;typ<types.size();typ++)
    if (tt == types[typ].name) {
       types[typ].print(s,p);
       return;
    };
}

//===========================================================================\\

dbase::dbase(){
  first();
}
dbase::~dbase(){
}

void dbase::newField(db_field_type typ,string name){
  vector<db_field_type_register_record>& types = db_field_type_register::types;
  db_basic_field *p = db_basic_field::types.allocate(typ,capacity());
  if (p){
    p->name = name;
    fields.push_back(p);
    for(size_t tt=0;tt<types.size();tt++)
      if (types[tt].name==typ) {
        p->assign = types[tt].assign;
        p->print = types[tt].print;
        break;
      };
  };
}

void dbase::deleteField(db_field_type type,string name){
  vector<db_basic_field*>::iterator i;
  for(i=fields.begin();i<fields.end();i++)
 	  if ((*i)->type == type && (*i)->name == name) {
 	    delete *i;
 	    fields.erase(i);
 	    break;
 	  };
}

void dbase::deleteFields(){
  vector<db_basic_field*>::iterator i;
  if (!fields.size()) return;
  i = fields.end();
  for(i = fields.end();fields.size();i = fields.end()){
    --i;
    delete *i;
    fields.erase(i);
  };
}

size_t dbase::addRow(){
  for(size_t i=0;i<fields.size();i++) fields[i]->add();
  return size()-1;
}

void dbase::insertRow(size_t j){
  for(size_t i=0;i<fields.size();i++) fields[i]->insert(j);
}

void dbase::eraseRow(size_t j){
  for(size_t i=0;i<fields.size();i++) fields[i]->erase(j);
}

void dbase::eraseRows(vector<int> &indx){
  for(size_t i=0;i<fields.size();i++) fields[i]->erase(indx);
}

/** No descriptions */
void dbase::loadFromFile(const string fn){
  vector<fmt_record> fmt,fld;
//  vector<fmt_record>::iterator f;
  fmt_record* f;
  ifstream from(fn.c_str());
  string str,pname,value,value0="";
  size_t i,j1,j2,n;
  db_basic_field* field;

//  for(size_t i=0;i<fields.size();i++)
//    cerr<<fields[i]->type <<'|'<<fields[i]->name<<'|'<<'\n';

  while(getline(from,str)){
    if ((i=str.find('\t'))!=string::npos) str[i] = ' ';
    if (str[0] == '#'){  // reading parameters
       if (str.substr(1) == "end") break;
       if ((i = str.find('=')) != string::npos){
          j1 = str.find_first_not_of(' ',1);
          pname = str.substr(j1,i-j1);
          if (pname.find("fmt") != string::npos){
             j1 = str.find('%');
             while( j1 != string::npos){
               if ( (j2 = str.find('%',++j1)) == string::npos)
                  fmt.push_back(fmt_record(str.substr(j1)));
               else
                  fmt.push_back(fmt_record(str.substr(j1,j2-j1)));
               j1 = j2;
               f = &fmt.back();
               if (db_field_type_register::registed(f->type)){
                 if (!existsField(f->type,f->name)) newField(f->type,f->name);
               }else
                 f->skip(f->field,f->bra,f->ket);
             };
          }else if (pname.find("fld") != string::npos){
             j1 = str.find('%');
             while( j1 != string::npos){
               if ( (j2 = str.find('%',++j1)) == string::npos)
                  fld.push_back(fmt_record(str.substr(j1)));
               else
                  fld.push_back(fmt_record(str.substr(j1,j2-j1)));
               j1 = j2;
               f = &fld.back();
               if (db_field_type_register::registed(f->type)){
                 if (!existsField(f->type,f->name)) newField(f->type,f->name);
               }else
                 f->skip(f->field,f->bra,f->ket);
             };
          }else
            params[pname] = str.substr(i+1);
       };
    }else{  // reading data
       j1 = 0;
       addRow();
       n = size()-1;
//       cerr<<"line "<<n<<' '<<str<<'\n';
       for(i=0;i<fmt.size();i++){
         if (fmt[i].field<0){   // do not read
         }else if (fmt[i].field>0){   // read exact number of characters
           if (j1 + fmt[i].field > str.size()) value = "";
           else
             value = str.substr(j1,fmt[i].field);
           j1 +=  fmt[i].field;
         }else if (fmt[i].bra > 0) {// read expression in brakets
           j1 = str.find_first_of(fmt[i].bra,j1);
           j2 = str.find_first_of(fmt[i].ket,j1+1);
           if (j2 > str.size()) value = "";
           else
             value = str.substr(j1+1,j2-j1-1);
           j1 = j2 + 1;
         }else{ // read the word bounded by space characters
           j1 = str.find_first_not_of(' ',j1);
           j2 = str.find_first_of(' ',j1+1);
           if (j2 == string::npos) j2 = str.size();
           if (j2 > str.size()) value = "";
           else
             value = str.substr(j1,j2-j1);
           j1 = j2;
         };
//         cerr<<value<<" j1="<<j1<<" j2="<<j2<<'\n';
         if (fmt[i].type == "x") continue;
         field = getField(fmt[i].type,fmt[i].name);
         if (fmt[i].value.empty())
           db_field_type_register::assign(fmt[i].type,field->data_addr(n),fmt[i].member.c_str(),value.c_str());
         else
           if (fmt[i].value[0]=='$'){
             db_field_type_register::assign(fmt[i].type,field->data_addr(n),fmt[i].member.c_str(),value0.c_str());
             value = value0;
           }else
             db_field_type_register::assign(fmt[i].type,field->data_addr(n),fmt[i].member.c_str(),fmt[i].value.c_str());

         value0 = value;
       };
    };
  };


  if (fld.size())
  for(size_t i=0;i<size();i++)
  for(size_t j=0;j<fld.size();j++){
     field = getField(fld[j].type,fld[j].name);
     db_field_type_register::assign(fld[j].type,field->data_addr(i),fld[j].member.c_str(),fld[j].value.c_str());
  };
  this->apply_params();

/*  cerr<<"param count="<<params.size()<<'\n';
  for(map<string,string>::iterator p=params.begin();p!=params.end();p++)
    cerr<<p->first<<'='<<p->second<<'\n';
  cerr<<"field count="<<fields.size()<<'\n';
  for(i=0;i<fields.size();i++)
    cerr<<"type="<<fields[i]->type<<" name="<<fields[i]->name<<'\n';
  cerr<<"fmt count="<<fmt.size()<<'\n';
  for(i=0;i<fmt.size();i++){
    cerr<<"----------------------\n";
    cerr<<"type="<<fmt[i].type<<'\n';
    cerr<<"name="<<fmt[i].name<<'\n';
    cerr<<"member="<<fmt[i].member<<'\n';
    cerr<<"field="<<fmt[i].field<<'\n';
    cerr<<"braket="<<fmt[i].bra<<fmt[i].ket<<'\n';
  };*/
}

//  %f:height %f[10]:line %vjkg['']:q    %vjkg[()]:q.ka
fmt_record::fmt_record(string str):type(),name(),member(),value(),field(),bra(),ket(){
   string first,second;
   size_t i0 = str.find_first_not_of(' ');
   if ( i0 == string::npos )  { skip();  return; };
   if ( str[i0] == '%' ) i0++;
   size_t ii = str.find(':');
   first = str.substr(i0,ii-i0);
   i0 = first.find('[');
   size_t i1 = first.find_last_of(']');
   if (i0 != string::npos && i1 != string::npos){
     size_t n = i1-i0-1;
     field = atoi(first.substr(i0+1,n).c_str());
     if (field == 0 && n == 2) {
       bra = first[i0+1];
       ket = first[i0+2];
     };
     type = first.substr(0,i0);
   }else
     type = first;
   if ( ii == string::npos )  { skip(field,bra,ket);  return; };
   second = str.substr(ii+1);
   i0 = second.find('.');
   if (i0 != string::npos){
     name = second.substr(0,i0);
     member = second.substr(i0+1);
     i1 = member.find('/');
     if (i1 != string::npos) {
       value = member.substr(i1+1);
       member.erase(i1);
     };
   }else{
     name = second;
     i1 = name.find('/');
     if (i1 != string::npos) {
       value = name.substr(i1+1);
       name.erase(i1);
     };
   };

   i1 = member.find(' ');
   if (i1 != string::npos)  member.erase(i1);
   i1 = name.find(' ');
   if (i1 != string::npos)  name.erase(i1);
}

void fmt_record::skip(size_t n,char b,char k){
  type = "x";
  name = "";
  member = "";
  value = "";
  field = n;
  bra = b;
  ket = k;
}

bool dbase::existsField(db_field_type typ, string name){
  for(size_t i=0;i<fields.size();i++)
    if (fields[i]->type == typ && fields[i]->name == name) return true;
  return false;
}

void dbase::saveToFile(const string fn,vector<int> *flt_){
  vector<int> *flt = flt_;
  ofstream file(fn.c_str());
  file.precision(12);
//  file<<"# program=goblin\n";

  for(size_t i=0;i<fields.size();i++){
    file<<"# fmt=%"<<fields[i]->type;
    if (fields[i]->type != db_int && fields[i]->type != db_float && fields[i]->type != db_double)  file<<"[\"\"]";
    file<<':'<<fields[i]->name<<'\n';
  };

  for(map<string,string>::iterator p=params.begin();p!=params.end();p++)
    file<<"# "<<p->first<<'='<<p->second<<'\n';

  if (flt && flt->size() != size()) flt = 0;

  for(size_t i=0;i<size();i++){
    if (flt && (*flt)[i] == 0) continue;
    for(size_t j=0;j<fields.size();j++){
      if (fields[j]->type != db_int && fields[j]->type != db_float && fields[j]->type != db_double){
        file<<'\"';
        db_field_type_register::print(fields[j]->type,file,fields[j]->data_addr(i));
        file<<"\" ";
      }else
        db_field_type_register::print(fields[j]->type,file,fields[j]->data_addr(i));
    };
      file<<'\n';
  };
}

void dbase::apply_params(){
}

/** clears contents of the database */
void dbase::clear(){
  for(size_t j=0;j<fields.size();j++) fields[j]->clear();
}
/** divides typnam into type and name of a field */
void dbase::type_name(const string typnam,string& typ,string& nam){
  string str = typnam;
  if (str[0] == '%') str.erase(0,1);
  size_t i;
  i = str.find(':');
  if (i == string::npos || i == 0){
    typ = "";
    nam = "";
    return;
  };
  typ = str.substr(0,i);
  nam = str.substr(i+1);
}

void dbase::moveField(size_t from,size_t to){
  if (from==to || from>=fields_size() || to>=fields_size()) return;
  db_basic_field *f = fields[from];
  if (from<to){
    for(size_t i=from;i<to;i++) fields[i] = fields[i+1];
  }else{
    for(size_t i=from;i>to;i--) fields[i] = fields[i-1];
  };
  fields[to] = f;
}

} // Goblin
