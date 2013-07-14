#include "Numeric/TiesFunction.h"
#include "Numeric/IConstraint.h"
#include "Numeric/ParameterTie.h"

namespace Numeric
{

/**
 * Destructor.
 */
TiesFunction::~TiesFunction()
{
  for(std::vector<ParameterTie*>::iterator it = m_ties.begin();it != m_ties.end(); ++it)
  {
    delete *it;
  }
  m_ties.clear();
  for(std::vector<IConstraint*>::iterator it = m_constraints.begin();it!= m_constraints.end();++it)
  {
    delete *it;
  }
  m_constraints.clear();
}

/**
 * Attaches a tie to this TiesFunction. The attached tie is owned by the TiesFunction.
 * @param tie :: A pointer to a new tie
 */
void TiesFunction::addTie(ParameterTie* tie)
{
  size_t iPar = tie->getIndex();
  bool found = false;
  for(std::vector<ParameterTie*>::size_type i=0;i<m_ties.size();i++)
  {
    if (m_ties[i]->getIndex() == iPar) 
    {
      found = true;
      delete m_ties[i];
      m_ties[i] = tie;
      break;
    }
  }
  if (!found)
  {
    m_ties.push_back(tie);
  }
}

/**
 * Apply the ties.
 */
void TiesFunction::applyTies()
{
  for(std::vector<ParameterTie*>::iterator tie=m_ties.begin();tie!=m_ties.end();++tie)
  {
    (**tie).eval();
  }
}

/**
 * Used to find ParameterTie for a parameter i
 */
class ReferenceEqual
{
  const size_t m_i;///< index to find
public:
  /** Constructor
   */
  ReferenceEqual(size_t i):m_i(i){}
  /**Bracket operator
   * @param p :: the parameter you are looking for
   * @return True if found
   */
  bool operator()(ParameterReference* p)
  {
    return p->getIndex() == m_i;
  }
};

/** Removes i-th parameter's tie if it is tied or does nothing.
 * @param i :: The index of the tied parameter.
 * @return True if successfull
 */
bool TiesFunction::removeTie(size_t i)
{
  if (i >= nParams())
  {
    throw std::out_of_range("TiesFunction parameter index out of range.");
  }
  std::vector<ParameterTie*>::iterator it = std::find_if(m_ties.begin(),m_ties.end(),ReferenceEqual(i));
  if (it != m_ties.end())
  {
    delete *it;
    m_ties.erase(it);
    unfix(i);
    return true;
  }
  return false;
}

/** Get tie of parameter number i
 * @param i :: The index of a declared parameter.
 * @return A pointer to the tie
 */
ParameterTie* TiesFunction::getTie(size_t i)const
{
  if (i >= nParams())
  {
    throw std::out_of_range("TiesFunction parameter index out of range.");
  }
  std::vector<ParameterTie*>::const_iterator it = std::find_if(m_ties.begin(),m_ties.end(),ReferenceEqual(i));
  if (it != m_ties.end())
  {
    return *it;
  }
  return NULL;
}

/** Remove all ties
 */
void TiesFunction::clearTies()
{
  for(std::vector<ParameterTie*>::iterator it = m_ties.begin();it != m_ties.end(); ++it)
  {
    size_t i = getParameterIndex(**it);
    unfix(i);
    delete *it;
  }
  m_ties.clear();
}

/** Add a constraint
 *  @param ic :: Pointer to a constraint.
 */
void TiesFunction::addConstraint(IConstraint* ic)
{
  size_t iPar = ic->getIndex();
  bool found = false;
  for(std::vector<IConstraint*>::size_type i=0;i<m_constraints.size();i++)
  {
    if (m_constraints[i]->getIndex() == iPar) 
    {
      found = true;
      delete m_constraints[i];
      m_constraints[i] = ic;
      break;
    }
  }
  if (!found)
  {
    m_constraints.push_back(ic);
  }
}

/** Get constraint of parameter number i
 * @param i :: The index of a declared parameter.
 * @return A pointer to the constraint or NULL
 */
IConstraint* TiesFunction::getConstraint(size_t i)const
{
  if (i >= nParams())
  {
    throw std::out_of_range("TiesFunction parameter index out of range.");
  }
  std::vector<IConstraint*>::const_iterator it = std::find_if(m_constraints.begin(),m_constraints.end(),ReferenceEqual(i));
  if (it != m_constraints.end())
  {
    return *it;
  }
  return NULL;
}

/** Remove a constraint
 * @param parName :: The name of a parameter which constarint to remove.
 */
void TiesFunction::removeConstraint(const std::string& parName)
{
  size_t iPar = parameterIndex(parName);
  for(std::vector<IConstraint*>::iterator it=m_constraints.begin();it!=m_constraints.end();++it)
  {
    if (iPar == (**it).getIndex())
    {
      delete *it;
      m_constraints.erase(it);
      break;
    }
  }
}

void TiesFunction::setParametersToSatisfyConstraints()
{
  for (size_t i = 0; i < m_constraints.size(); i++)
  {
    m_constraints[i]->setParamToSatisfyConstraint();
  }
}


} // Numeric
