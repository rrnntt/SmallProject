#include "Modeling/Model.h"

#include <osgDB/ReadFile>

#include <osg/Texture2D>
#include <osg/TexEnv>

#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Bone>

#include <iostream>

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
    void apply(osg::Node& node) {
        if (_am.valid())
            return;
        if (node.getUpdateCallback()) {
            osgAnimation::AnimationManagerBase* b = dynamic_cast<osgAnimation::AnimationManagerBase*>(node.getUpdateCallback());
            if (b) {
                _am = new osgAnimation::BasicAnimationManager(*b);
                return;
            }
        }
        traverse(node);
    }
};

class AnimtkViewerModelController 
{
public:
    typedef std::vector<std::string> AnimationMapVector;

    static AnimtkViewerModelController& instance() 
    {
        static AnimtkViewerModelController avmc;
        return avmc;
    }

    static bool setModel(osgAnimation::BasicAnimationManager* model) 
    {
        AnimtkViewerModelController& self = instance();
        self._model = model;
        for (osgAnimation::AnimationList::const_iterator it = self._model->getAnimationList().begin(); it != self._model->getAnimationList().end(); it++)
            self._map[(*it)->getName()] = *it;

        for(osgAnimation::AnimationMap::iterator it = self._map.begin(); it != self._map.end(); it++)
            self._amv.push_back(it->first);

        return true;
    }

    bool list() 
    {
        std::cout << "Animation List:" << std::endl;
        for(osgAnimation::AnimationMap::iterator it = _map.begin(); it != _map.end(); it++)
            std::cout << it->first << std::endl;
        return true;
    }

    bool play() 
    {
        if(_focus < _amv.size()) 
        {
            std::cout << "Play " << _amv[_focus] << std::endl;
            _model->playAnimation(_map[_amv[_focus]].get());
            return true;
        }

        return false;
    }

    bool stop() 
    {
        if(_focus < _amv.size()) 
        {
            std::cout << "Stop " << _amv[_focus] << std::endl;
            _model->stopAnimation(_map[_amv[_focus]].get());
            return true;
        }
        return false;
    }    

    bool next() 
    {
        _focus = (_focus + 1) % _map.size();
        std::cout << "Current now is " << _amv[_focus] << std::endl;
        return true;
    }

    bool previous() 
    {
        _focus = (_map.size() + _focus - 1) % _map.size();
        std::cout << "Current now is " << _amv[_focus] << std::endl;
        return true;
    }

    bool playByName(const std::string& name) 
    {
        for(unsigned int i = 0; i < _amv.size(); i++) if(_amv[i] == name) _focus = i;
        _model->playAnimation(_map[name].get());
        return true;
    }

    const std::string& getCurrentAnimationName() const 
    {
        return _amv[_focus];
    }

    const AnimationMapVector& getAnimationMap() const 
    {
        return _amv;
    }

private:
    osg::ref_ptr<osgAnimation::BasicAnimationManager> _model;
    osgAnimation::AnimationMap                   _map;
    AnimationMapVector                     _amv;
    unsigned int                           _focus;

    AnimtkViewerModelController():
        _model(0),
        _focus(0) {}
};

void Model::load(const std::string& fname)
{
  m_node = osgDB::readNodeFile(fname);

    // Set our Singleton's model.
    AnimationManagerFinder finder;
    m_node->accept(finder);
    if (finder._am.valid()) {
        m_node->setUpdateCallback(finder._am.get());
        AnimtkViewerModelController::setModel(finder._am.get());
        AnimtkViewerModelController::instance().play();
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
