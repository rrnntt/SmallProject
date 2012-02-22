#include "Modeling/Model.h"
#include "Modeling/AnimationController.h"

#include <osgDB/ReadFile>

#include <osg/Texture2D>
#include <osg/TexEnv>

#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Bone>

#include <iostream>

namespace Modeling
{

Model::Model():
m_node(),m_updateOn(false),m_oldCallback(NULL)
{
}

/**
 * Switches on the update handler (update()) which will run for each frame
 */
void Model::setUpdate(bool on)
{
  if (on)
  {
    if (m_node)
    {
      if (m_node->getUpdateCallback())
      {
        m_oldCallback = m_node->getUpdateCallback();
      }
      m_node->setUserData( this );
      m_node->setUpdateCallback(new ModelNodeCallback());
    }
  }
}

/**
 * The update node callback - a way osg handles model updates
 */
void Model::ModelNodeCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
  osg::ref_ptr<Model> model = 
    dynamic_cast<Model*> (node->getUserData() );
  if(model)
  {
    if (model->m_oldCallback) (*model->m_oldCallback)(node,nv);
    model->update();
    //std::cerr << nv->getTraversalNumber() << ' ' << nv->getFrameStamp()->getSimulationTime() << std::endl;
  }
  traverse(node, nv); 
}

struct AnimationManagerFinder : public osg::NodeVisitor
{
  osg::ref_ptr<osgAnimation::BasicAnimationManager> _am;
  AnimationManagerFinder() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}
  void apply(osg::Node& node)
  {
    if (_am.valid())
        return;
    if (node.getUpdateCallback())
    {
        osgAnimation::AnimationManagerBase* b = dynamic_cast<osgAnimation::AnimationManagerBase*>(node.getUpdateCallback());
        if (b)
        {
            _am = new osgAnimation::BasicAnimationManager(*b);
            return;
        }
    }
    traverse(node);
  }
};

/**
  * Load geometry (and animation) from a file.
  * @param fname :: A file name.
  */
void Model::load(const std::string& fname)
{
  // find appropriate loading plugin and load the file
  m_node = osgDB::readNodeFile(fname);

  // Try to find some animation in the model
  AnimationManagerFinder finder;
  m_node->accept(finder);
  if (finder._am.valid())
  {
    // if animation found play it (?)
      m_node->setUpdateCallback(finder._am.get());
      AnimationController::setModel(finder._am.get());
      AnimationController::instance().play();
  } else {
      osg::notify(osg::WARN) << "no osgAnimation::AnimationManagerBase found in the subgraph, no animations available" << std::endl;
  }

  //osg::Texture2D *tex = new osg::Texture2D;
  //tex->setWrap( osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT );
  //tex->setWrap( osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT );
  //tex->setImage(osgDB::readImageFile("Images/tank.rgb"));
  //osg::StateSet *dstate = new osg::StateSet;
  //dstate->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON );
  //dstate->setTextureAttribute(0, new osg::TexEnv );
  //m_node->setStateSet( dstate );
}

} // namespace Modeling
