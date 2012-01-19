#include <string>
#include <sstream>
#include <fstream>
#include "../Troll1/utils.h"
#include "../Troll1/mio.h"
#include "../Troll1/gdata.h"
#include "text.h"

using namespace std;

text::text():object(),ifirst(),ipage(20),cx(),cy(),cwidth(7),cheight(15){
  g_height = 1;
  sx0=sy0=sx1=sy1=0;
  sel = none;
}

bool text::load(string fn){
  ifstream fil(fn.c_str());
  if (!fil) return false;
  string str;
  size_t i=0;
  while(getline(fil,str)){
    txt.push_back(str);
    i++;
  };
  if (!txt.size()) txt.push_back("");
  return true;
}

bool text::save(){
  if (write()) return save(fname);
  return false;
}

bool text::save(string fn){
  ofstream fil(fn.c_str());
  if (!fil) return false;
  for(vector<string>::iterator s=txt.begin();s!=txt.end();s++)
    fil<<*s<<endl;
  return true;
}

cmd_res text::cmd(string str){
  cmd_res ok,bad(false),repaint;
  repaint.repaint(true);
  istringstream istr(str);
  string com;
  istr>>com;
  string rest;
  rest = str.substr(str.find(com)+com.size());

  size_t i = com.find('.');
  if (i != string::npos){
    com[i] = ' ';
    istringstream is(com);
    string who,itscom;
    is>>who>>itscom;
//    if (who == "lpar") return lparams().cmd(itscom+' '+rest);
    return bad;
  };
  if (ifirst >= txt.size()) ifirst = txt.size()-1;
  size_t cn = ifirst+cy;
  if (!txt.size()) txt.push_back("");
  if (cn >= txt.size()) cy = 0;
  string &ctxt = txt[cn];
  if (ifirst+cy < txt.size() && cx > txt[ifirst+cy].size()) cx = txt[ifirst+cy].size();
  
  if (com=="tabs"){
    for(vector<string>::iterator t=txt.begin();t!=txt.end();t++)
      for(size_t i=0;i<(*t).size();i++)
        if ((*t)[i] == '\t') (*t)[i] = ' ';
    return repaint;
  };
  
  if (com=="inss"){
    if (cx < ctxt.size()) ctxt.insert(cx," ");
    else
      ctxt += ' ';
    cx++;
    return repaint;
  };
  
  if (com=="insc"){
    string c;
    istr>>c;
    if (c.empty()) return ok;
    if (cx < ctxt.size()) ctxt.insert(cx,c);
    else
      ctxt += c;
    cx++;
    return repaint;
  };
  
  if (com=="newl"){
    size_t i=0,j=ifirst+cy;
    for(vector<string>::iterator t=txt.begin();t!=txt.end();t++,i++)
        if (i == j) {
          vector<string>::iterator tt = t++;
          txt.insert(t,tt->substr(cx));
          tt->erase(cx);
          cmd("ldn");cmd("lhome");
        };
    return repaint;
  };
  
  if (com=="bk"){
    if (cx == 0){
      if (cn == 0) return ok;
      cx = txt[cn-1].size();
      if (cy == 0) cmd("up");
      else
        cy--;
      txt[cn-1] += ctxt;
      size_t i=0;
      for(vector<string>::iterator t=txt.begin();t!=txt.end();t++,i++)
        if (i == cn) txt.erase(t);
    }else if (cx <= ctxt.size())
       ctxt.erase(--cx,1);
    return repaint;
  };
  
  if (com=="del"){
    size_t i,j;
    if (cx < txt[ifirst+cy].size()) txt[ifirst+cy].erase(cx,1);
    else if (cy < txt.size()-1){
      txt[ifirst+cy] += txt[ifirst+cy+1];
      i=0;j=ifirst+cy+1;
      for(vector<string>::iterator t=txt.begin();t!=txt.end();t++,i++)
        if (i == j) txt.erase(t);
    };
    return repaint;
  };
  
  if (com=="lr"){
    cx++;
    if (ifirst+cy < txt.size() && cx > txt[ifirst+cy].size()) cx = txt[ifirst+cy].size();
    return repaint;
  };
  
  if (com=="ll"){
    if (cx > 0) cx--;
    return repaint;
  };
  
  if (com=="home"){
    ifirst = 0;
    return repaint;
  };
  
  if (com=="end"){
    if (ipage < txt.size()-1) ifirst = txt.size()-ipage;
    else
      ifirst = 0;
    return repaint;
  };
  
  if (com=="lhome"){
    cx = 0;
    return repaint;
  };
  
  if (com=="lend"){
    cx = txt[ifirst+cy].size();
    return repaint;
  };
  
  if (com=="pgdn"){
    ifirst += ipage;
    if (ifirst >= txt.size()) ifirst = txt.size() - 1;
    if (ifirst+cy < txt.size() && cx > txt[ifirst+cy].size()) cx = txt[ifirst+cy].size();
    return repaint;
  };
  
  if (com=="pgup"){
    if (ifirst >= ipage) ifirst -= ipage;
    else 
      ifirst = 0;
    if (ifirst+cy < txt.size() && cx > txt[ifirst+cy].size()) cx = txt[ifirst+cy].size();
    return repaint;
  };
  
  if (com=="dn"){
    ifirst += 1;
    if (ifirst >= txt.size()) ifirst = txt.size() - 1;
    if (ifirst+cy < txt.size() && cx > txt[ifirst+cy].size()) cx = txt[ifirst+cy].size();
    return repaint;
  };
  
  if (com=="lup"){
    if (cy > 0) cy--;
    else {
      cy = 0;
      cmd("up");
    };
    if (ifirst+cy < txt.size() && cx > txt[ifirst+cy].size()) cx = txt[ifirst+cy].size();
    return repaint;
  };
  
  if (com=="ldn"){
    cy += 1;
    if (ifirst+cy >= txt.size()) cy = txt.size() - 1 - ifirst;
    if (cy*cheight > g_height) {
      cy = ipage;
      cmd("dn");
    };
    if (ifirst+cy < txt.size() && cx > txt[ifirst+cy].size()) cx = txt[ifirst+cy].size();
    return repaint;
  };
  
  if (com=="up"){
    if (ifirst > 0) ifirst -= 1;
    else 
      ifirst = 0;
    if (ifirst+cy < txt.size() && cx > txt[ifirst+cy].size()) cx = txt[ifirst+cy].size();
    return repaint;
  };
  
  if (com=="wr"){
    write(true);
    return ok;
  };
  
  if (com=="rd"){
    write(false);
    return ok;
  };
  
  if (com=="save"){
    if (!save()) return bad;
    return ok;
  };
  
  if (com=="saveAs"){
    string fn;
    istr>>fn;
    if (!save(fn)) return bad;
    return ok;
  };
  
  if (com=="load"){
    string fn;
    istr>>fn;
    if (!load(fn)) return bad;
    return ok;
  };
  
  if (com=="keypress"){
    int key;
    char shft,ch;
    string in_type;
    string s;
    istr>>key>>in_type>>shft;
    if (in_type == "def"){
      switch (key){
        case key_PageUp: return cmd("pgup");
        case key_PageDown: return cmd("pgdn");
        case key_Up: return (shft=='c')?cmd("up"):cmd("lup");
        case key_Down: return (shft=='c')?cmd("dn"):cmd("ldn");
        case key_Right: return cmd("lr");
        case key_Left: return cmd("ll");
        case key_Home: return (shft=='c')?cmd("home"):cmd("lhome");
        case key_End: return (shft=='c')?cmd("end"):cmd("lend");
        case key_Delete: return cmd("del");
        case key_BackSpace: return cmd("bk");
        case key_Enter: return cmd("newl");
        case key_Space: 
               if (shft == 's'){
                  bool OK=false;
                  string ss = mio.in(&OK);
                  return OK?(owner->cmd(ss)):ok;
               }else
                 return cmd("inss");
        case key_A: return (shft=='s')?cmd("insc A"):cmd("insc a");
        case key_B: return (shft=='s')?cmd("insc B"):cmd("insc b");
        case key_C: return (shft=='s')?cmd("insc C"):cmd("insc c");
        case key_D: return (shft=='s')?cmd("insc D"):cmd("insc d");
        case key_E: return (shft=='s')?cmd("insc E"):cmd("insc e");
        case key_F: return (shft=='s')?cmd("insc F"):cmd("insc f");
        case key_G: return (shft=='s')?cmd("insc G"):cmd("insc g");
        case key_H: return (shft=='s')?cmd("insc H"):cmd("insc h");
        case key_I: return (shft=='s')?cmd("insc I"):cmd("insc i");
        case key_J: return (shft=='s')?cmd("insc J"):cmd("insc j");
        case key_K: return (shft=='s')?cmd("insc K"):cmd("insc k");
        case key_L: return (shft=='s')?cmd("insc L"):cmd("insc l");
        case key_M: return (shft=='s')?cmd("insc M"):cmd("insc m");
        case key_N: return (shft=='s')?cmd("insc N"):cmd("insc n");
        case key_O: return (shft=='s')?cmd("insc O"):cmd("insc o");
        case key_P: return (shft=='s')?cmd("insc P"):cmd("insc p");
        case key_Q: return (shft=='s')?cmd("insc Q"):cmd("insc q");
        case key_R: return (shft=='s')?cmd("insc R"):cmd("insc r");
        case key_S: return (shft=='s')?cmd("insc S"):cmd("insc s");
        case key_T: return (shft=='s')?cmd("insc T"):cmd("insc t");
        case key_U: return (shft=='s')?cmd("insc U"):cmd("insc u");
        case key_V: return (shft=='s')?cmd("insc V"):cmd("insc v");
        case key_W: return (shft=='s')?cmd("insc W"):cmd("insc w");
        case key_X: return (shft=='s')?cmd("insc X"):cmd("insc x");
        case key_Y: return (shft=='s')?cmd("insc Y"):cmd("insc y");
        case key_Z: return (shft=='s')?cmd("insc Z"):cmd("insc z");
        case key_0: return (shft=='s')?cmd("insc )"):cmd("insc 0");
        case key_1: return (shft=='s')?cmd("insc !"):cmd("insc 1");
        case key_2: return (shft=='s')?cmd("insc \""):cmd("insc 2");
        case key_3: return (shft=='s')?cmd("insc £"):cmd("insc 3");
        case key_4: return (shft=='s')?cmd("insc $"):cmd("insc 4");
        case key_5: return (shft=='s')?cmd("insc %"):cmd("insc 5");
        case key_6: return (shft=='s')?cmd("insc ^"):cmd("insc 6");
        case key_7: return (shft=='s')?cmd("insc &"):cmd("insc 7");
        case key_8: return (shft=='s')?cmd("insc *"):cmd("insc 8");
        case key_9: return (shft=='s')?cmd("insc ("):cmd("insc 9");
        case key_Period: return (shft=='s')?cmd("insc >"):cmd("insc .");
        case key_Comma: return (shft=='s')?cmd("insc <"):cmd("insc ,");
//        case key_: return (shft=='s')?cmd("insc "):cmd("insc ");
      };
      return bad; 
    };
    return bad;
  };
  

  return object::cmd(str);
}

fun_res text::fun(string str){
  string s = str;
  string b,c;
  while( !(b = brakets(s)).empty() ){
    if (b == "$ERROR$") return b;
    c = fun(b)();
    s.replace(s.find('['),b.size()+2,c);
  };
  istringstream istr(s);
  ostringstream ostr;
  string nam;
  istr >> nam;
  if (nam.empty()) return fun_res();
  
  if (nam == "size"){
    ostr << 0;
    return ostr.str();
  };
  
  return object::fun(str);			      
}


bool text::draw(canvas& c){
  if (c.type == "graph" && c.out_type == "def"){
   graphics& g = *(graphics*)(&c);
   g.clip_clientRect();
   int x,y;
   g_height = g.client_height();
   for(size_t i=ifirst;i<txt.size();i++){
     y = (i-ifirst)*cheight;
     if (y > g_height) {
       ipage = i - ifirst - 1;
       break;
     };
     g.text(10,y,txt[i]);
   };
   g.pen(rgb(255,0,0));
   x = 10 + cx*cwidth;
   y = cy*cheight;
   g.moveTo(x,y);
   g.lineTo(x,y+cheight);
   return true;
  };
  return false;
}


cmd_res text::mouseClick(canvas* c,int x,int y,int shft){
  cmd_res bad(false),repaint;
  repaint.repaint(false);
  if (c->type != "graph") return bad;
  graphics& g = *(graphics*)c;
  if (shft == 1) {
    size_t cn = ifirst + cy;
    if (cn > sy0) {
      sy1 = cn; sx1 = cx;
    };
  };
  return repaint;
}



