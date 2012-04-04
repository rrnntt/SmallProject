#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/CompositeFunction.h"
#include "Numeric/MultiDomainFunction.h"
#include "Numeric/ConstraintFactory.h"
#include "Numeric/IConstraint.h"
#include "Kernel/EParser.h"
#include "API/Framework.h"

#include <sstream>

namespace Numeric
{

    FunctionFactory& FunctionFactory::instance()
    {
      Singleton* s = API::Framework::instance().getSingleton("FunctionFactory");
      if (s == nullptr)
      {
        FunctionFactory *f = new FunctionFactory();
        API::Framework::instance().registerSingleton("FunctionFactory",f);
        return *f;
      }
      else
      {
        return *static_cast<FunctionFactory*>(s);
      }
    }

    FunctionFactory::FunctionFactory() : Kernel::DynamicFactory<IFunction>(), g_log(Kernel::Logger::get("FunctionFactory"))
    {
      g_log.debug("FunctionFactory created.");
    }

    IFunction_sptr FunctionFactory::createFunction(const std::string& type) const
    {
      IFunction_sptr fun = IFunction_sptr(create(type));
      return fun;
    }



    /**Creates an instance of a function
     * @param input :: An input string which defines the function and initial values for the parameters.
     * Parameters of different functions are separated by ';'. Parameters of the same function
     * are separated by ','. parameterName=value pairs are used to set a parameter value. For each function
     * "name" parameter must be set to a function name. E.g.
     * input = "name=LinearBackground,A0=0,A1=1; name = Gaussian, PeakCentre=10.,Sigma=1"
     * @return A pointer to the created function
     */
    IFunction_sptr FunctionFactory::createInitialized(const std::string& input) const
    {
      Kernel::EParser expr;
      try
      {
        expr.parse(input);
      }
      catch(...)
      {
        inputError(input);
      }

      const Kernel::EParser& e = expr;//.bracketsRemoved();
      std::map<std::string,std::string> parentAttributes;
      if (e.name() == ";")
      {
        IFunction_sptr fun = createComposite(e,parentAttributes);
        if (!fun) inputError();
        return fun;
      }

      return createSimple(e,parentAttributes);

    }

    /** 
     * Create a function from an expression.
     * @param expr :: The input expression
     * @return A pointer to the created function
     */
    IFunction_sptr FunctionFactory::createSimple(const Kernel::EParser& expr, std::map<std::string,std::string>& parentAttributes)const
    {
      if (expr.name() == "=" && expr.size() > 1)
      {
        return createFunction(expr.terms()[1]->name());
      }

      if (expr.name() != "," || expr.size() == 0)
      {
        inputError(expr.str());
      }

      auto terms = expr.terms();
      auto term = *terms.begin();

      if (term->name() != "=") inputError(expr.str());
      if (term[0].name() != "name" && term[0].name() != "composite")
      {
        throw std::invalid_argument("Function name must be defined before its parameters");
      }
      std::string fnName = term[1].name();

      IFunction_sptr fun = createFunction(fnName);
      for(auto t = terms.begin() + 1; t != terms.end(); ++t)
      {// loop over function's parameters/attributes
        term = *t;
        if (term->name() != "=") inputError(expr.str());
        std::string parName = term[0].name();
        std::string parValue = term[1].str();
        if (fun->hasAttribute(parName))
        {// set attribute
          if (parValue.size() > 1 && parValue[0] == '"')
          {// remove the double quotes
            parValue = parValue.substr(1,parValue.size()-2);
          }
          IFunction::Attribute att = fun->getAttribute(parName);
          att.fromString(parValue);
          fun->setAttribute(parName,att);
        }
        else if (parName.size() >= 10 && parName.substr(0,10) == "constraint")
        {// or it can be a list of constraints
          addConstraints(fun,(*term)[1]);
        }
        else if (parName == "ties")
        {
          addTies(fun,(*term)[1]);
        }
        else if (!parName.empty() && parName[0] == '$')
        {
          parName.erase(0,1);
          parentAttributes[parName] = parValue;
        }
        else
        {// set initial parameter value
          fun->setParameter(parName,atof(parValue.c_str()));
        }
      }// for term

      return fun;
    }

    /** 
     * Create a composite function from an expression.
     * @param expr :: The input expression
     * @return A pointer to the created function
     */
    CompositeFunction_ptr FunctionFactory::createComposite(const Kernel::EParser& expr, std::map<std::string,std::string>& parentAttributes)const
    {
      if (expr.name() != ";") inputError(expr.str());

      if (expr.size() == 0)
      {
        return CompositeFunction_ptr();
      }

      auto terms = expr.terms();
      auto it = terms.begin();
      const Kernel::EParser& term = *it;//->bracketsRemoved();

      CompositeFunction_ptr cfun;
      if (term.name() == "=")
      {
        if (term[0]->name() == "composite")
        {
          cfun = boost::dynamic_pointer_cast<CompositeFunction>(createFunction(term[1]->name()));
          if (!cfun) inputError(expr.str());
          ++it;
        }
        else if (term[0]->name() == "name")
        {
          cfun = boost::dynamic_pointer_cast<CompositeFunction>(createFunction("CompositeFunction"));
          if (!cfun) inputError(expr.str());
        }
        else
        {
          inputError(expr.str());
        }
      }
      else if (term.name() == ",")
      {
        auto firstTerm = term.terms().begin();
        if ((*firstTerm)->name() == "=")
        {
          if ((*firstTerm)[0].name() == "composite")
          {
            cfun = boost::dynamic_pointer_cast<CompositeFunction>(createSimple(term,parentAttributes));
            if (!cfun) inputError(expr.str());
            ++it;
          }
          else if ((*firstTerm)[0].name() == "name")
          {
            cfun = boost::dynamic_pointer_cast<CompositeFunction>(createFunction("CompositeFunction"));
            if (!cfun) inputError(expr.str());
          }
          else
          {
            inputError(expr.str());
          }
        }
      }
      else if (term.name() == ";")
      {
        cfun = boost::dynamic_pointer_cast<CompositeFunction>(createFunction("CompositeFunction"));
        if (!cfun) inputError(expr.str());
      }
      else
      {
        inputError(expr.str());
      }

      for(;it!=terms.end();++it)
      {
        const Kernel::EParser& term = *it;//->bracketsRemoved();
        IFunction_sptr fun;
        std::map<std::string,std::string> pAttributes;
        if (term.name() == ";")
        {
          fun = createComposite(term,pAttributes);
          if (!fun) continue;
        }
        else
        {
          std::string parName = term[0]->name();
          std::string parValue = term[1]->str();
          if (term[0]->name().size() >= 10 && term[0]->name().substr(0,10) == "constraint")
          {
            addConstraints(cfun,term[1]);
            continue;
          }
          else if (term[0]->name() == "ties")
          {
            addTies(cfun,term[1]);
            continue;
          }
          else
          {
            fun = createSimple(term,pAttributes);
          }
        }
        cfun->addFunction(fun);
        size_t i = cfun->nFunctions() - 1;
        for(auto att = pAttributes.begin(); att != pAttributes.end(); ++att)
        {
          cfun->setLocalAttributeValue(i,att->first,att->second);
        }
      }

      return cfun;
    }

    /// Throw an exception
    void FunctionFactory::inputError(const std::string& str)const
    {
      std::string msg("Error in input string to FunctionFactory");
      if (!str.empty())
      {
        msg += "\n" + str;
      }
      throw std::invalid_argument(msg);
    }

    /** 
     * Add constraints to the created function
     * @param fun :: The function
     * @param expr :: The constraint expression. The expression name must be either a single constraint
     *    expression such as "0 < Sigma < 1" or a list of constraint expressions separated by commas ','
     *    and enclosed in brackets "(...)" .
     */
    void FunctionFactory::addConstraints(IFunction_sptr fun,const Kernel::EParser& expr)const
    {
      if (expr.name() == ",")
      {
        for(size_t i=0;i<expr.size();i++)
        {
          addConstraint(fun,expr[i]);
        }
      }
      else
      {
        addConstraint(fun,expr);
      }
    }

    /** 
     * Add a constraints to the function
     * @param fun :: The function
     * @param expr :: The constraint expression.
     */
    void FunctionFactory::addConstraint(IFunction_sptr fun,const Kernel::EParser& expr)const
    {
      IConstraint* c = ConstraintFactory::instance().createInitialized(fun.get(),expr);
      fun->addConstraint(c);
    }

    /**
     * @param fun :: The function
     * @param expr :: The tie expression: either parName = TieString or a list
     *   of name = string pairs
     */
    void FunctionFactory::addTies(IFunction_sptr fun,const Kernel::EParser& expr)const
    {
      if (expr.name() == "=")
      {
        addTie(fun,expr);
      }
      else if (expr.name() == ",")
      {
        for(size_t i=0;i<expr.size();i++)
        {
          addTie(fun,expr[i]);
        }
      }
    }

    /**
     * @param fun :: The function
     * @param expr :: The tie expression: parName = TieString
     */
    void FunctionFactory::addTie(IFunction_sptr fun,const Kernel::EParser& expr)const
    {
      if (expr.size() > 1)
      {// if size > 2 it is interpreted as setting a tie (last expr.term) to multiple parameters, e.g 
        // f1.alpha = f2.alpha = f3.alpha = f0.beta^2/2
        const std::string value = expr[expr.size()-1]->str();
        for( size_t i = expr.size() - 1; i != 0; )
        {
          --i;
          fun->tie(expr[i]->name(),value);
        }
      }
    }

    /**
      * Create a fitting function from a string.
      * @param input :: The input string, has a form of a function call: funName(attr1=val,param1=val,...,ties=(param3=2*param1,...),constraints=(p2>0,...))
      */
    IFunction_sptr FunctionFactory::createFitFunction(const std::string& input) const
    {
      Kernel::EParser expr;
      try
      {
        expr.parse(input);
      }
      catch(...)
      {
        inputError(input);
      }
      return createFitFunction(expr);
    }

    /**
      * Create a fitting function from an expression.
      * @param expr :: The input expression made by parsing the input string to createFitFunction(const std::string& input)
      */
    IFunction_sptr FunctionFactory::createFitFunction(const Kernel::EParser& expr) const
    {
      const Kernel::EParser& e = expr;//.bracketsRemoved();

      std::string fnName = e.name();

      IFunction_sptr fun( create(fnName) );
      if (!fun)
      {
        throw std::runtime_error("Cannot create function "+fnName);
      }

      auto terms = e.terms();
      auto term = terms.begin();

      for(;term!=terms.end();++term)
      {// loop over function's parameters/attributes
        if ((*term)->name() == "=")
        {
          std::string parName = (*term)[0].name();
          std::string parValue = (*term)[1].str();
          if (fun->hasAttribute(parName))
          {// set attribute
            if (parValue.size() > 1 && parValue[0] == '"')
            {// remove the double quotes
              parValue = parValue.substr(1,parValue.size()-2);
            }
            IFunction::Attribute att = fun->getAttribute(parName);
            att.fromString(parValue);
            fun->setAttribute(parName,att);
          }
          else if (parName.size() >= 10 && parName.substr(0,10) == "constraint")
          {// or it can be a list of constraints
            addConstraints(fun,(*term)[1]);
          }
          else if (parName == "ties")
          {
            addTies(fun,(*term)[1]);
          }
          else
          {// set initial parameter value
            fun->setParameter(parName,atof(parValue.c_str()));
          }
        }
        else // if the term isn't a name=value pair it could be a member function of a composite function
        {
          throw std::runtime_error("Composite functions are not implemented yet for IFunction");
          //CompositeFunction* cfun = dynamic_cast<CompositeFunction*>(fun);
          //if (!cfun)
          //{
          //  throw std::runtime_error("Cannot add a function to a non-composite function "+fnName);
          //}
          //IFunction* mem = createFitFunction(*term);
          //cfun->addFunction(mem);
        }
      }// for term

      return fun;
    }

} // namespace Numeric
