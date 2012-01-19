#include "../Troll1/object.h"
#include "../Troll1/gdata.h"
#include "../Troll1/mio.h"
#include "text.h"
#include "spline.h"
#include "splist.h"
#include "enlist.h"
#include "spectrum.h"

object* createObject(string type,string name){
  object* o = 0;
  if (type == "splist"){
    o = (object*) new splist;
    if (!o){
      cerr<<"Error creating "<<type<<'\n';
      return 0;
    };
    o->ext = "aln3";
  };

  if (type == "enlist"){
    o = (object*) new enlist;
    if (!o){
      cerr<<"Error creating "<<type<<'\n';
      return 0;
    };
    o->ext = "lev3";
  };

  if (type == "spectrum"){
    o = (object*) new spectrum;
    if (!o){
      cerr<<"Error creating "<<type<<'\n';
      return 0;
    };
    o->ext = "hdr";
  };
  
  if (type == "spline"){
    o = (object*) new spline;
    if (!o){
      cerr<<"Error creating "<<type<<'\n';
      return 0;
    };
    o->ext = "txt";
  };
  
  if (type == "spbase"){
    o = (object*) new spbase;
    if (!o){
      cerr<<"Error creating "<<type<<'\n';
      return 0;
    };
    o->ext = "txt";
  };
  
  if (type == "text"){
    o = (object*) new text;
    if (!o){
      cerr<<"Error creating "<<type<<'\n';
      return 0;
    };
    o->ext = "txt";
  };
  
/*  if (o) {
    mainAppForm->obj.add(type+':'+name);
    mainAppForm->obj.setText(type+':'+name);
  };*/
  return o;
}



