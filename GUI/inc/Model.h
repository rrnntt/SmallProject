#ifndef MODEL_H
#define MODEL_H

#include <osg/Referenced>
#include <osg/Node>

class Model: public osg::Referenced
{
protected:
  class ModelNodeCallback : public osg::NodeCallback
  {
  public:
    void operator()(osg::Node* node, osg::NodeVisitor* nv);
  };
public:
  Model();
  osg::Node* getNode()const{return m_node;}
  void setUpdate(bool on = true);
  virtual void update(){}
  void load(const std::string& fname);
protected:
  osg::ref_ptr<osg::Node> m_node;
  bool m_updateOn;
  osg::NodeCallback* m_oldCallback;
};

#endif /*MODEL_H*/