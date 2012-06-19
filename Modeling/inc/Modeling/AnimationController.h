#ifndef MODELING_ANIMATIONCONTROLLER_H
#define MODELING_ANIMATIONCONTROLLER_H

#include "Modeling/DllExport.h"
#include "Kernel/Singleton.h"

#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Bone>

namespace Modeling
{

class MODELING_EXPORT AnimationController: protected Kernel::Singleton
{
public:
  AnimationController();
  ~AnimationController(){}
  virtual std::string name() const {return "AnimationController";}
  static AnimationController& instance();
  typedef std::vector<std::string> AnimationMapVector;

  static bool setModel(osgAnimation::BasicAnimationManager* model);

  bool list();

  bool play();

  bool stop();

  bool next();

  bool previous();

  bool playByName(const std::string& name);

  const std::string& getCurrentAnimationName() const ;

  const AnimationMapVector& getAnimationMap() const ;

private:
  osg::ref_ptr<osgAnimation::BasicAnimationManager> _model;
  osgAnimation::AnimationMap     _map;
  AnimationMapVector             _amv;
  unsigned int                   _focus;

};

} // namespace Modeling

#endif // MODELING_ANIMATIONCONTROLLER_H
