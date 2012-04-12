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
      Singleton* s = API::Framework::instance().getSingleton("Numeric::FunctionFactory");
      if (s == nullptr)
      {
        FunctionFactory *f = new FunctionFactory();
        API::Framework::instance().registerSingleton("Numeric::FunctionFactory",f);
        return *f;
      }
      else
      {
        return *static_cast<FunctionFactory*>(s);
      }
    }

    FunctionFactory::FunctionFactory() : Kernel::DynamicFactory<IFunction>(), g_log(Kernel::Logger::get("Numeric::FunctionFactory"))
    {
      g_log.debug("Numeric::FunctionFactory created.");
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
        const std::string value = expr[expr.size()-1].str();
        for( size_t i = expr.size() - 1; i != 0; )
        {
          --i;
          fun->tie(expr[i].name(),value);
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
        std::string ini(input);
        std::for_each(ini.begin(),ini.end(),[](char& c){
          if (c == '\n') c = ' ';
        });
        expr.parse(ini);
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

      if (e.size() == 0)
      {
        return fun;
      }
      //else if (e[0].name() != ",")
      //{
      //  throw std::runtime_error("createFitFunction: wrong syntax");
      //}

      auto terms = e.terms();//e[0].terms();
      auto term = terms.begin();

      for(;term!=terms.end();++term)
      {// loop over function's parameters/attributes
        const Kernel::EParser& t = (**term);
        if (t.name() == "=")
        {
          const Kernel::EParser& left = t[0];
          const Kernel::EParser& right = t[1];
          std::string parName = left.name();
          std::string parValue = right.str();
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
          //throw std::runtime_error("Composite functions are not implemented yet for IFunction");
          CompositeFunction_sptr cfun = boost::dynamic_pointer_cast<CompositeFunction>(fun);
          if (!cfun)
          {
            throw std::runtime_error("Cannot add a function to a non-composite function "+fnName);
          }
          IFunction_sptr mem = createFitFunction(t);
          cfun->addFunction(mem);
        }
      }// for term

      return fun;
    }

} // namespace Numeric
