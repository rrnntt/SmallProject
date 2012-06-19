#include "Modeling/AnimationController.h"
#include "Kernel/Framework.h"

namespace Modeling
{
AnimationController::AnimationController():
_model(0),
_focus(0)
{
}

AnimationController& AnimationController::instance()
{
  Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("AnimationController");
  if (s == nullptr)
  {
    AnimationController *f = new AnimationController();
    Kernel::Framework::instance().registerSingleton("AnimationController",f);
    return *f;
  }
  else
  {
    return *static_cast<AnimationController*>(s);
  }
}

bool AnimationController::setModel(osgAnimation::BasicAnimationManager* model)
{
  AnimationController& self = instance();
  self._model = model;
  for (auto it = self._model->getAnimationList().begin(); it != self._model->getAnimationList().end(); it++)
  {
    self._map[(*it)->getName()] = *it;
  }

  for(auto it = self._map.begin(); it != self._map.end(); it++)
  {
    self._amv.push_back(it->first);
  }

  return true;
}

bool AnimationController::list()
{
    std::cout << "Animation List:" << std::endl;
    for(osgAnimation::AnimationMap::iterator it = _map.begin(); it != _map.end(); it++)
        std::cout << it->first << std::endl;
    return true;
}

bool AnimationController::play()
{
    if(_focus < _amv.size())
    {
        std::cout << "Play " << _amv[_focus] << std::endl;
        _model->playAnimation(_map[_amv[_focus]].get());
        return true;
    }

    return false;
}

bool AnimationController::stop()
{
    if(_focus < _amv.size())
    {
        std::cout << "Stop " << _amv[_focus] << std::endl;
        _model->stopAnimation(_map[_amv[_focus]].get());
        return true;
    }
    return false;
}

bool AnimationController::next()
{
    _focus = (_focus + 1) % _map.size();
    std::cout << "Current now is " << _amv[_focus] << std::endl;
    return true;
}

bool AnimationController::previous()
{
    _focus = (_map.size() + _focus - 1) % _map.size();
    std::cout << "Current now is " << _amv[_focus] << std::endl;
    return true;
}

bool AnimationController::playByName(const std::string& name)
{
    for(unsigned int i = 0; i < _amv.size(); i++) if(_amv[i] == name) _focus = i;
    _model->playAnimation(_map[name].get());
    return true;
}

const std::string& AnimationController::getCurrentAnimationName() const
{
    return _amv[_focus];
}

const AnimationController::AnimationMapVector& AnimationController::getAnimationMap() const
{
    return _amv;
}

} // namespace Modeling
