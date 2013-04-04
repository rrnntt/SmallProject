#include "QtAPI/FunctionBrowser.h"
#include "QtAPI/SelectFunctionDialog.h"

#include "Numeric/FunctionFactory.h"
#include "Numeric/CompositeFunction.h"
//#include "MantidAPI/ITableWorkspace.h"
//#include "MantidAPI/IPeakFunction.h"
//#include "MantidAPI/IBackgroundFunction.h"
//#include "MantidAPI/AlgorithmManager.h"
//#include "MantidAPI/FrameworkManager.h"
//#include "MantidAPI/MatrixWorkspace.h"
//#include "MantidAPI/WorkspaceGroup.h"
//#include "MantidAPI/TableRow.h"
#include "Numeric/ParameterTie.h"
#include "Numeric/IConstraint.h"
//#include "MantidAPI/ConstraintFactory.h"
#include "Kernel/EParser.h"
//#include "MantidKernel/ConfigService.h"
//#include "MantidKernel/LibraryManager.h"

//#include "MantidAPI/CostFunctionFactory.h"
//#include "MantidAPI/ICostFunction.h"

//#include "MantidQtMantidWidgets/UserFunctionDialog.h"

#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
// Suppress a warning coming out of code that isn't ours
#if defined(__INTEL_COMPILER)
  #pragma warning disable 1125
#elif defined(__GNUC__)
  #if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 6 )
    #pragma GCC diagnostic push
  #endif
  #pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
#include "qteditorfactory.h"
#include "StringDialogEditorFactory.h"
#include "DoubleEditorFactory.h"
#if defined(__INTEL_COMPILER)
  #pragma warning enable 1125
#elif defined(__GNUC__)
  #if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 6 )
    #pragma GCC diagnostic pop
  #endif
#endif

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QSettings>
#include <QFileInfo>
#include <QApplication>
#include <QClipboard>
#include <QSignalMapper>
#include <QMetaMethod>
#include <QTreeWidget>

#include <algorithm>
#include <boost/lexical_cast.hpp>

namespace QtAPI
{

namespace
{

class FormulaDialogEditor: public StringDialogEditor
{
public:
  FormulaDialogEditor(QtProperty *property, QWidget *parent)
    :StringDialogEditor(property,parent){}
protected slots:
  void runDialog()
  {
//    MantidQt::MantidWidgets::UserFunctionDialog *dlg = new MantidQt::MantidWidgets::UserFunctionDialog((QWidget*)parent(),getText());
//    if (dlg->exec() == QDialog::Accepted)
//    {
//      setText(dlg->getFormula());
//      updateProperty();
//    };
  }
};

class FormulaDialogEditorFactory: public StringDialogEditorFactory
{
public:
  FormulaDialogEditorFactory(QObject* parent):StringDialogEditorFactory(parent){}
protected:
  using QtAbstractEditorFactoryBase::createEditor; // Avoid Intel compiler warning
  QWidget *createEditor(QtStringPropertyManager *manager, QtProperty *property,QWidget *parent)
  {
    (void) manager; //Avoid unused warning
    return new FormulaDialogEditor(property,parent);
  }
};

}

/**
 * Constructor
 * @param parent :: The parent widget.
 */
FunctionBrowser::FunctionBrowser(QWidget *parent)
:QWidget(parent)
{
  // create m_browser
  createBrowser();
  createActions();

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(m_browser);
  layout->setContentsMargins(0,0,0,0);

}

/**
 * Destructor
 */
FunctionBrowser::~FunctionBrowser()
{
}


/**
 * Create the Qt property browser and set up property managers.
 */
void FunctionBrowser::createBrowser()
{
  /* Create property managers: they create, own properties, get and set values  */
  m_functionManager = new QtGroupPropertyManager(this);
  m_parameterManager = new QtDoublePropertyManager(this);
  m_attributeStringManager = new QtStringPropertyManager(this);
  m_attributeDoubleManager = new QtDoublePropertyManager(this);
  m_attributeIntManager = new QtIntPropertyManager(this);
  m_attributeBoolManager = new QtBoolPropertyManager(this);
  m_indexManager = new QtStringPropertyManager(this);
  m_tieManager = new QtStringPropertyManager(this);
  m_constraintManager = new QtStringPropertyManager(this);
  m_formulaManager = new QtStringPropertyManager(this);

  //m_filenameManager = new QtStringPropertyManager(this);
  //m_formulaManager = new QtStringPropertyManager(this);

  // create editor factories
  QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
  DoubleEditorFactory *doubleEditorFactory = new DoubleEditorFactory(this);
  QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
  QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
  //StringDialogEditorFactory* stringDialogEditFactory = new StringDialogEditorFactory(this);
  FormulaDialogEditorFactory* formulaDialogEditFactory = new FormulaDialogEditorFactory(this);

  m_browser = new QtTreePropertyBrowser();
  // assign factories to property managers
  m_browser->setFactoryForManager(m_parameterManager, doubleEditorFactory);
  m_browser->setFactoryForManager(m_attributeStringManager, lineEditFactory);
  m_browser->setFactoryForManager(m_attributeDoubleManager, doubleEditorFactory);
  m_browser->setFactoryForManager(m_attributeIntManager, spinBoxFactory);
  m_browser->setFactoryForManager(m_attributeBoolManager, checkBoxFactory);
  m_browser->setFactoryForManager(m_indexManager, lineEditFactory);
  m_browser->setFactoryForManager(m_tieManager, lineEditFactory);
  m_browser->setFactoryForManager(m_constraintManager, lineEditFactory);
  m_browser->setFactoryForManager(m_formulaManager, formulaDialogEditFactory);

  m_browser->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_browser, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(popupMenu(const QPoint &)));
  //connect(m_browser, SIGNAL(currentItemChanged(QtBrowserItem*)), this, SLOT(currentItemChanged(QtBrowserItem*)));

  connect(m_attributeStringManager,SIGNAL(propertyChanged(QtProperty*)),this,SLOT(attributeChanged(QtProperty*)));
  connect(m_attributeDoubleManager,SIGNAL(propertyChanged(QtProperty*)),this,SLOT(attributeChanged(QtProperty*)));
  connect(m_attributeIntManager,SIGNAL(propertyChanged(QtProperty*)),this,SLOT(attributeChanged(QtProperty*)));
  connect(m_attributeBoolManager,SIGNAL(propertyChanged(QtProperty*)),this,SLOT(attributeChanged(QtProperty*)));
  connect(m_formulaManager,SIGNAL(propertyChanged(QtProperty*)),this,SLOT(attributeChanged(QtProperty*)));
}

/**
 * Create and connect actions
 */
void FunctionBrowser::createActions()
{
  m_actionAddFunction = new QAction("Add function",this);
  connect(m_actionAddFunction,SIGNAL(triggered()),this,SLOT(addFunction()));

  m_actionRemoveFunction = new QAction("Remove function",this);
  connect(m_actionRemoveFunction,SIGNAL(triggered()),this,SLOT(removeFunction()));

  m_actionFixParameter = new QAction("Fix",this);
  connect(m_actionFixParameter,SIGNAL(triggered()),this,SLOT(fixParameter()));

  m_actionRemoveTie = new QAction("Remove tie",this);
  connect(m_actionRemoveTie,SIGNAL(triggered()),this,SLOT(removeTie()));

  m_actionAddTie = new QAction("Add tie",this);
  connect(m_actionAddTie,SIGNAL(triggered()),this,SLOT(addTie()));

  m_actionFromClipboard = new QAction("Copy from clipboard",this);
  connect(m_actionFromClipboard,SIGNAL(triggered()),this,SLOT(copyFromClipboard()));

  m_actionToClipboard = new QAction("Copy to clipboard",this);
  connect(m_actionToClipboard,SIGNAL(triggered()),this,SLOT(copyToClipboard()));

  m_actionConstraints = new QAction("Custom",this);
  connect(m_actionConstraints,SIGNAL(triggered()),this,SLOT(addConstraints()));

  m_actionConstraints10 = new QAction("10%",this);
  connect(m_actionConstraints10,SIGNAL(triggered()),this,SLOT(addConstraints10()));

  m_actionConstraints50 = new QAction("50%",this);
  connect(m_actionConstraints50,SIGNAL(triggered()),this,SLOT(addConstraints50()));

  m_actionRemoveConstraints = new QAction("Remove constraints",this);
  connect(m_actionRemoveConstraints,SIGNAL(triggered()),this,SLOT(removeConstraints()));

  m_actionRemoveConstraint = new QAction("Remove",this);
  connect(m_actionRemoveConstraint,SIGNAL(triggered()),this,SLOT(removeConstraint()));
}

/**
 * Clear the contents
 */
void FunctionBrowser::clear()
{
  m_browser->clear();
  m_properties.clear();
}

/**
 * Set the function in the browser
 * @param funStr :: FunctionFactory function creation string
 */
void FunctionBrowser::setFunction(const QString& funStr)
{
  if ( funStr.isEmpty() ) return;
  try
  {
    auto fun = Numeric::FunctionFactory::instance().createFitFunction( funStr.toStdString() );
    if ( !fun ) return;
    this->setFunction(fun);
  }
  catch(...)
  {
    // error in the input string
  }
}

/**
 * Set the function in the browser
 * @param fun :: A function
 */
void FunctionBrowser::setFunction(Numeric::IFunction_sptr fun)
{
  clear();
  addFunction(NULL,fun);
}

/**
 * Add a sub-property to a parent property
 * @param parent :: The parent property
 * @param subproperty :: New sub-property
 */
FunctionBrowser::AProperty FunctionBrowser::addProperty(QtProperty* parent, QtProperty* subproperty)
{
  AProperty ap;
  ap.prop = subproperty;
  if (parent == NULL)
  {
    ap.item = m_browser->addProperty(subproperty);
  }
  else
  {
    parent->addSubProperty(subproperty);
    auto items = m_browser->items(subproperty);
    if (items.isEmpty())
    {
      throw std::runtime_error("Unexpected error in FunctionBrowser [1]");
    }
    ap.item = items[0];
  }
  ap.parent = parent;
  m_properties[subproperty] = ap;
  return ap;
}

/**
 * Remove and delete property
 * @param prop :: Property to remove.
 */
void FunctionBrowser::removeProperty(QtProperty *prop)
{
  auto p = m_properties.find(prop);
  if (p == m_properties.end()) return;
  AProperty ap = *p;

  // remove references to the children
  auto children = prop->subProperties();
  foreach(QtProperty* child,children)
  {
    m_properties.remove(child);
  }
  m_properties.erase(p);

  if ( isFunction(prop) )
  {
    m_ties.remove(prop);
  }

  if ( isTie(prop) )
  {// 
    for(auto it = m_ties.begin(); it != m_ties.end(); ++it)
    {
      if (it.value().tieProp == prop)
      {
        m_ties.erase(it);
        break;
      }
    }
  }

  if ( isConstraint(prop) )
  {
    for(auto it = m_constraints.begin(); it != m_constraints.end(); ++it)
    {
      auto& cp = it.value();
      if ( cp.lower == prop )
      {
        if ( !cp.upper )
        {
          m_constraints.erase(it);
        }
        else
        {
          cp.lower = NULL;
        }
        break;
      }
      else if ( cp.upper == prop )
      {
        if ( !cp.lower )
        {
          m_constraints.erase(it);
        }
        else
        {
          cp.upper = NULL;
        }
        break;
      }
    }
  }

  // remove property from Qt browser
  if (ap.parent)
  {
    ap.parent->removeSubProperty(prop);
  }
  else
  {
    m_browser->removeProperty(prop);
  }
  delete prop;
}

/**
 * Add a function property
 * @param parent :: Parent function property or NULL
 * @param funName :: Function name
 * @return :: A set AProperty struct
 */
FunctionBrowser::AProperty FunctionBrowser::addFunctionProperty(QtProperty* parent, QString funName)
{
  // check that parent is a function property
  if (parent && dynamic_cast<QtAbstractPropertyManager*>(m_functionManager) != parent->propertyManager())
  {
    throw std::runtime_error("Unexpected error in FunctionBrowser [2]");
  }
  QtProperty* prop = m_functionManager->addProperty(funName);
  return addProperty(parent, prop);
}

/**
 * Add a parameter property
 * @param parent :: Parent function property
 * @param paramName :: Parameter name
 * @param paramValue :: Parameter value
 */
FunctionBrowser::AProperty FunctionBrowser::addParameterProperty(QtProperty* parent, QString paramName, double paramValue)
{
  // check that parent is a function property
  if (!parent || dynamic_cast<QtAbstractPropertyManager*>(m_functionManager) != parent->propertyManager())
  {
    throw std::runtime_error("Unexpected error in FunctionBrowser [3]");
  }
  QtProperty* prop = m_parameterManager->addProperty(paramName);
  m_parameterManager->setValue(prop,paramValue);
  return addProperty(parent,prop);
}

/**
 * Set a function.
 * @param prop :: Property of the function or NULL
 * @param fun :: A function
 */
void FunctionBrowser::setFunction(QtProperty* prop, Numeric::IFunction_sptr fun)
{
  auto children = prop->subProperties();
  foreach(QtProperty* child, children)
  {
    removeProperty(child);
  }
  addAttributeAndParameterProperties(prop, fun);
}

/**
 * Add a function.
 * @param prop :: Property of the parent composite function or NULL
 * @param fun :: FunctionFactory function creation string
 */
void FunctionBrowser::addFunction(QtProperty* prop, Numeric::IFunction_sptr fun)
{
  //Mantid::API::IFunction_sptr fun = Mantid::API::FunctionFactory::Instance().createInitialized(funStr.toStdString());
  if ( !prop )
  {
    AProperty ap = addFunctionProperty(NULL,QString::fromStdString(fun->name()));
    setFunction(ap.prop, fun);
  }
  else
  {
    Numeric::IFunction_sptr parentFun =  getFunction(prop);
    if ( !parentFun ) return;
    auto cf = boost::dynamic_pointer_cast<Numeric::CompositeFunction>(parentFun);
    if ( !cf )
    {
      throw std::runtime_error("FunctionBrowser: CompositeFunction is expected for addFunction");
    }
    cf->addFunction(fun);
    setFunction(prop, cf);
  }
}

/**
 * Attribute visitor to create a QtProperty. Depending on the attribute type
 * the appropriate apply() method is used.
 */
class CreateAttributePropertyForFunctionBrowser: public Numeric::IFunction::ConstAttributeVisitor<FunctionBrowser::AProperty>
{
public:
  CreateAttributePropertyForFunctionBrowser(FunctionBrowser* browser, QtProperty* parent,QString attName)
    :m_browser(browser),m_parent(parent),m_attName(attName)
  {
    // check that parent is a function property
    if (!m_parent || dynamic_cast<QtAbstractPropertyManager*>(m_browser->m_functionManager) != m_parent->propertyManager())
    {
      throw std::runtime_error("Unexpected error in FunctionBrowser [4]");
    }
  }
protected:
  /// Create string property
  FunctionBrowser::AProperty apply(const std::string& str)const
  {
    QtProperty* prop = NULL;
    if ( m_attName == "Formula" )
    {
      prop = m_browser->m_formulaManager->addProperty(m_attName);
      m_browser->m_formulaManager->setValue(prop, QString::fromStdString(str));
    }
    else
    {
      prop = m_browser->m_attributeStringManager->addProperty(m_attName);
      m_browser->m_attributeStringManager->setValue(prop, QString::fromStdString(str));
    }
    return m_browser->addProperty(m_parent,prop);
  }
  /// Create double property
  FunctionBrowser::AProperty apply(const double& d)const
  {
    QtProperty* prop = m_browser->m_attributeDoubleManager->addProperty(m_attName);
    m_browser->m_attributeDoubleManager->setValue(prop, d);
    return m_browser->addProperty(m_parent,prop);
  }
  /// Create int property
  FunctionBrowser::AProperty apply(const int& i)const
  {
    QtProperty* prop = m_browser->m_attributeIntManager->addProperty(m_attName);
    m_browser->m_attributeIntManager->setValue(prop, i);
    return m_browser->addProperty(m_parent,prop);
  }
  /// Create bool property
  FunctionBrowser::AProperty apply(const bool& b)const
  {
    QtProperty* prop = m_browser->m_attributeBoolManager->addProperty(m_attName);
    m_browser->m_attributeBoolManager->setValue(prop, b);
    return m_browser->addProperty(m_parent,prop);
  }
private:
  FunctionBrowser* m_browser;
  QtProperty* m_parent;
  QString m_attName;
};

/**
 * Attribute visitor to set an attribute from a QtProperty. Depending on the attribute type
 * the appropriate apply() method is used.
 */
class SetAttributeFromProperty: public Numeric::IFunction::AttributeVisitor<>
{
public:
  SetAttributeFromProperty(FunctionBrowser* browser, QtProperty* prop)
    :m_browser(browser),m_prop(prop)
  {
  }
protected:
  /// Set string attribute
  void apply(std::string& str)const
  {
    QString attName = m_prop->propertyName();
    if ( attName == "Formula" )
    {
      str = m_browser->m_formulaManager->value(m_prop).toStdString();
    }
    else
    {
      str = m_browser->m_attributeStringManager->value(m_prop).toStdString();
    }
  }
  /// Set double attribute
  void apply(double& d)const
  {
    d = m_browser->m_attributeDoubleManager->value(m_prop);
  }
  /// Set int attribute
  void apply(int& i)const
  {
    i = m_browser->m_attributeIntManager->value(m_prop);
  }
  /// Set bool attribute
  void apply(bool& b)const
  {
    b = m_browser->m_attributeBoolManager->value(m_prop);
  }
private:
  FunctionBrowser* m_browser;
  QtProperty* m_prop;
};

/**
 * Add a attribute property
 * @param parent :: Parent function property
 * @param attName :: Attribute name
 * @param att :: Attribute value
 */
FunctionBrowser::AProperty FunctionBrowser::addAttributeProperty(QtProperty* parent, QString attName, const Numeric::IFunction::Attribute& att)
{
  CreateAttributePropertyForFunctionBrowser cap(this,parent,attName);
  return att.apply(cap);
}

/**
 * Add attribute and parameter properties to a function property. For a composite function
 *  adds all member functions' properties
 * @param prop :: A function property
 * @param fun :: Shared pointer to a created function
 */
void FunctionBrowser::addAttributeAndParameterProperties(QtProperty* prop, Numeric::IFunction_sptr fun)
{
  // add the function index property
  addIndexProperty(prop);

  // add attribute properties
  auto attributeNames = fun->getAttributeNames();
  for(auto att = attributeNames.begin(); att != attributeNames.end(); ++att)
  {
    QString attName = QString::fromStdString(*att);
    addAttributeProperty(prop, attName, fun->getAttribute(*att));
  }

  auto cf = boost::dynamic_pointer_cast<Numeric::CompositeFunction>(fun);
  if (cf)
  {// if composite add members
    for(size_t i = 0; i < cf->nFunctions(); ++i)
    {
      AProperty ap = addFunctionProperty(prop,QString::fromStdString(cf->getFunction(i)->name()));
      addAttributeAndParameterProperties(ap.prop,cf->getFunction(i));
    }
  }
  else
  {// if simple add parameters
    for(size_t i = 0; i < fun->nParams(); ++i)
    {
      QString name = QString::fromStdString(fun->parameterName(i));
      double value = fun->getParameter(i);
      AProperty ap = addParameterProperty(prop, name, value);
      // if parameter has a tie
      if (fun->isFixed(i))
      {
        auto tie = fun->getTie(i);
        if (tie)
        {
          addTieProperty(ap.prop, QString::fromStdString(tie->asString()));
        }
      }
      auto c = fun->getConstraint(i);
      if ( c )
      {
        addConstraintProperties( ap.prop, QString::fromStdString( c->asString() ) );
      }
    }
  }
}

/**
 * Add property showing function's index in the composite function
 * @param prop :: A function property
 * @return :: AProperty struct for added property. If all fields are NULL - property wasn't added
 *  because it is the top function
 */
FunctionBrowser::AProperty FunctionBrowser::addIndexProperty(QtProperty* prop)
{
  AProperty ap;
  ap.item = NULL;
  ap.parent = NULL;
  ap.prop = NULL;
  if (!prop) return ap;
  if (!isFunction(prop)) return ap;
  if ( !m_properties[prop].parent ) return ap;

  QString index = "fff";
  QtProperty* ip = m_indexManager->addProperty("Index");
  ip->setEnabled(false);
  m_indexManager->setValue(ip,index);
  return addProperty(prop,ip);

}

/**
 * Update function index properties 
 * @param prop :: A function property
 * @param index :: The parent function's index
 */
void FunctionBrowser::updateFunctionIndices(QtProperty* prop, QString index)
{
  if ( prop == NULL )
  {
    auto top = m_browser->properties();
    if (top.isEmpty()) return;
    prop = top[0];
  }
  auto children = prop->subProperties();
  size_t i = 0;
  foreach(QtProperty* child, children)
  {
    if (isFunction(child))
    {
      updateFunctionIndices(child,index + "f" + QString::number(i) + ".");
      ++i;
    }
    else if (isIndex(child))
    {
      m_indexManager->setValue(child,index);
    }
  }
}

/**
 * Get property of the overall function.
 */
FunctionBrowser::AProperty FunctionBrowser::getFunctionProperty()
{
  auto props = m_browser->properties();
  if ( props.isEmpty() )
  {
    AProperty ap;
    ap.item = NULL;
    ap.parent = NULL;
    ap.prop = NULL;
    return ap;
  }
  QtProperty* prop = props[0];
  return m_properties[prop];
}


/**
 * Check if property is a function group
 * @param prop :: Property to check
 */
bool FunctionBrowser::isFunction(QtProperty* prop) const
{
  return prop && dynamic_cast<QtAbstractPropertyManager*>(m_functionManager) == prop->propertyManager();
}

/**
 * Check if property is a function attribute
 * @param prop :: Property to check
 */
bool FunctionBrowser::isStringAttribute(QtProperty* prop) const
{
  return prop && (
    dynamic_cast<QtAbstractPropertyManager*>(m_attributeStringManager) == prop->propertyManager() ||
    dynamic_cast<QtAbstractPropertyManager*>(m_formulaManager) == prop->propertyManager()
    );
}

/**
 * Check if property is a function attribute
 * @param prop :: Property to check
 */
bool FunctionBrowser::isDoubleAttribute(QtProperty* prop) const
{
  return prop && dynamic_cast<QtAbstractPropertyManager*>(m_attributeDoubleManager) == prop->propertyManager();
}

/**
 * Check if property is a function attribute
 * @param prop :: Property to check
 */
bool FunctionBrowser::isIntAttribute(QtProperty* prop) const
{
  return prop && dynamic_cast<QtAbstractPropertyManager*>(m_attributeIntManager) == prop->propertyManager();
}

/**
 * Check if property is a function attribute
 * @param prop :: Property to check
 */
bool FunctionBrowser::isAttribute(QtProperty* prop) const
{
  return isStringAttribute(prop) || isDoubleAttribute(prop) || isIntAttribute(prop);
}

/**
 * Check if property is a function parameter
 * @param prop :: Property to check
 */
bool FunctionBrowser::isParameter(QtProperty* prop) const
{
  return prop && dynamic_cast<QtAbstractPropertyManager*>(m_parameterManager) == prop->propertyManager();
}

/**
 * Get parameter value as a string
 * @param prop :: A parameter property
 */
double FunctionBrowser::getParameter(QtProperty* prop) const
{
  return m_parameterManager->value(prop);
}

/**
 * Check if a property is an index
 * @param prop :: A property
 */
bool FunctionBrowser::isIndex(QtProperty* prop) const
{
  return prop && dynamic_cast<QtAbstractPropertyManager*>(m_indexManager) == prop->propertyManager();
}

/**
 * Get the function index for a property
 * @param prop :: A property
 */
QString FunctionBrowser::getIndex(QtProperty* prop) const
{
  if ( !prop ) return "";
  if (isFunction(prop))
  {
    auto props = prop->subProperties();
    if (props.isEmpty()) return "";
    for(auto it = props.begin(); it != props.end(); ++it)
    {
      if ( isIndex(*it) )
      {
        return m_indexManager->value(*it);
      }
    }
    return "";
  }

  auto ap = m_properties[prop];
  return getIndex(ap.parent); 
}


/**
 * Add a tie property
 * @param prop :: Parent parameter property
 * @param tie :: A tie string
 */
FunctionBrowser::AProperty FunctionBrowser::addTieProperty(QtProperty* prop, QString tie)
{
  if ( !prop )
  {
    throw std::runtime_error("FunctionBrowser: null property pointer");
  }
  AProperty ap;
  ap.item = NULL;
  ap.prop = NULL;
  ap.parent = NULL;

  if ( !isParameter(prop) ) return ap;

  Kernel::EParser expr;
  expr.parse(tie.toStdString());
  // Do parameter names include composite function index
  bool isComposite = false;
  auto vars = expr.getVariables();
  for(auto var = vars.begin(); var != vars.end(); ++var)
  {
    // nesting level of a particular variable
    int n = static_cast<int>(std::count(var->begin(),var->end(),'.'));
    if ( n != 0 )
    {
      isComposite = true;
    }
  }

  // check that tie has form <paramName>=<expression>
  if ( expr.name() != "=" )
  {// prepend "<paramName>="
    if ( !isComposite )
    {
      tie.prepend(prop->propertyName() + "=");
    }
    else
    {
      QString index = getIndex(prop);
      tie.prepend(index + prop->propertyName() + "=");
    }
  }

  // find the property of the function
  QtProperty *funProp = isComposite? getFunctionProperty().prop : m_properties[prop].parent;

  QtProperty* tieProp = m_tieManager->addProperty("Tie");
  m_tieManager->setValue(tieProp, tie);
  ap = addProperty(prop,tieProp);

  ATie atie;
  atie.paramProp = prop;
  atie.tieProp = tieProp;
  //atie.tie = tie;
  m_ties.insert(funProp,atie);

  return ap;
}

/**
 * Check if a parameter property has a tie
 * @param prop :: A parameter property
 */
bool FunctionBrowser::hasTie(QtProperty* prop) const
{
  if ( !prop ) return false;
  auto children = prop->subProperties();
  foreach(QtProperty* child, children)
  {
    if ( child->propertyName() == "Tie" )
    {
      return true;
    }
  }
  return false;
}

/**
 * Check if a property is a tie
 * @param prop :: A property
 */
bool FunctionBrowser::isTie(QtProperty* prop) const
{
  return prop && dynamic_cast<QtAbstractPropertyManager*>(m_tieManager) == prop->propertyManager();
}


/**
 * Get a tie for a parameter
 * @param prop :: A parameter property
 */
std::string FunctionBrowser::getTie(QtProperty* prop) const
{
  if ( !prop ) return "";
  auto children = prop->subProperties();
  foreach(QtProperty* child, children)
  {
    if ( child->propertyName() == "Tie" )
    {
      return m_tieManager->value(child).toStdString();
    }
  }
  return "";
}

/** 
 * Add a constraint property
 * @param prop :: Parent parameter property
 * @param constraint :: A constraint string
 */
QList<FunctionBrowser::AProperty> FunctionBrowser::addConstraintProperties(QtProperty* prop, QString constraint)
{
  if ( !isParameter(prop) ) return QList<FunctionBrowser::AProperty>();
  QString lowerBoundStr = "";
  QString upperBoundStr = "";
  Kernel::EParser expr;
  expr.parse(constraint.toStdString());
  if ( expr.name() != "==" ) return QList<FunctionBrowser::AProperty>();
  if ( expr.size() == 3 )
  {// lower < param < upper
    try
    {
      // check that the first and third terms are numbers
      double d1 = boost::lexical_cast<double>( expr[0].name() ); (void)d1;
      double d2 = boost::lexical_cast<double>( expr[2].name() ); (void)d2;
      if ( expr[1].operator_name() == "<" && expr[2].operator_name() == "<" )
      {
        lowerBoundStr = QString::fromStdString( expr[0].name() );
        upperBoundStr = QString::fromStdString( expr[2].name() );
      }
      else // assume that the operators are ">"
      {
        lowerBoundStr = QString::fromStdString( expr[2].name() );
        upperBoundStr = QString::fromStdString( expr[0].name() );
      }
    }
    catch(...)
    {// error in constraint
      return QList<FunctionBrowser::AProperty>();
    }
  }
  else if ( expr.size() == 2 )
  {// lower < param or param > lower etc
    size_t paramPos = 0;
    try // find position of the parameter name in expression
    {
      double d = boost::lexical_cast<double>( expr[1].name() );(void)d;
    }
    catch(...)
    {
      paramPos = 1;
    }
    std::string op = expr[1].operator_name();
    if ( paramPos == 0 )
    {// parameter goes first
      if ( op == "<" )
      {// param < number
        upperBoundStr = QString::fromStdString( expr[1].name() );
      }
      else
      {// param > number
        lowerBoundStr = QString::fromStdString( expr[1].name() );
      }
    }
    else
    {// parameter is second
      if ( op == "<" )
      {// number < param
        lowerBoundStr = QString::fromStdString( expr[0].name() );
      }
      else
      {// number > param
        upperBoundStr = QString::fromStdString( expr[0].name() );
      }
    }
  }

  // add properties
  QList<FunctionBrowser::AProperty> plist;
  AConstraint ac;
  //ac.constraint = constraint;
  ac.paramProp = prop;
  ac.lower = ac.upper = NULL;
  if ( !lowerBoundStr.isEmpty() )
  {
    auto ap = addProperty( prop, m_constraintManager->addProperty("LowerBound") );
    plist << ap;
    ac.lower = ap.prop;
    m_constraintManager->setValue( ac.lower, lowerBoundStr );
  }
  if ( !upperBoundStr.isEmpty() )
  {
    auto ap = addProperty( prop, m_constraintManager->addProperty("UpperBound") );
    plist << ap;
    ac.upper = ap.prop;
    m_constraintManager->setValue( ac.upper, upperBoundStr );
  }
  if ( ac.lower || ac.upper )
  {
    m_constraints.insert(m_properties[prop].parent,ac);
  }

  return plist;
}

/**
 * Check if a property is a constraint
 * @param prop :: Property to check.
 */
bool FunctionBrowser::isConstraint(QtProperty* prop) const
{
  return prop && dynamic_cast<QtAbstractPropertyManager*>(m_constraintManager) == prop->propertyManager();
}

/**
 * Check if a parameter property has a constraint
 * @param prop :: A parameter property.
 */
bool FunctionBrowser::hasConstraint(QtProperty* prop) const
{
  return hasLowerBound(prop) || hasUpperBound(prop);
}

/**
 * Check if a parameter property has a lower bound
 * @param prop :: A parameter property.
 */
bool FunctionBrowser::hasLowerBound(QtProperty* prop) const
{
  if ( !isParameter(prop) ) return false;
  auto props = prop->subProperties();
  if ( props.isEmpty() ) return false;
  foreach(QtProperty* p, props)
  {
    if ( dynamic_cast<QtAbstractPropertyManager*>(m_constraintManager) == p->propertyManager() &&
      p->propertyName() == "LowerBound" ) return true;
  }
  return false;
}

/**
 * Check if a parameter property has a upper bound
 * @param prop :: A parameter property.
 */
bool FunctionBrowser::hasUpperBound(QtProperty* prop) const
{
  if ( !isParameter(prop) ) return false;
  auto props = prop->subProperties();
  if ( props.isEmpty() ) return false;
  foreach(QtProperty* p, props)
  {
    if ( dynamic_cast<QtAbstractPropertyManager*>(m_constraintManager) == p->propertyManager() &&
      p->propertyName() == "UpperBound" ) return true;
  }
  return false;
}



/**
 * Show a pop up menu.
 */
void FunctionBrowser::popupMenu(const QPoint &)
{
  auto item = m_browser->currentItem();
  if (!item)
  {
    QMenu context(this);
    context.addAction(m_actionAddFunction);
    if ( !QApplication::clipboard()->text().isEmpty() )
    {
      context.addAction(m_actionFromClipboard);
    }
    if ( !m_browser->properties().isEmpty() )
    {
      context.addAction(m_actionToClipboard);
    }
    context.exec(QCursor::pos());
    return;
  }
  QtProperty* prop = item->property();
  if (isFunction(prop))
  {// functions
    QMenu context(this);
    Numeric::IFunction_sptr fun = Numeric::FunctionFactory::instance().createFitFunction(prop->propertyName().toStdString());
    auto cf = boost::dynamic_pointer_cast<Numeric::CompositeFunction>(fun);
    if (cf || m_properties[prop].parent == NULL)
    {
      context.addAction(m_actionAddFunction);
    }
    context.addAction(m_actionRemoveFunction);
    if ( !QApplication::clipboard()->text().isEmpty() )
    {
      context.addAction(m_actionFromClipboard);
    }
    if ( !m_browser->properties().isEmpty() )
    {
      context.addAction(m_actionToClipboard);
    }
    context.exec(QCursor::pos());
  }
  else if (isParameter(prop))
  {// parameters
    QMenu context(this);
    if ( hasTie(prop) )
    {
      context.addAction(m_actionRemoveTie);
    }
    else
    {
      context.addAction(m_actionFixParameter);
      context.addAction(m_actionAddTie);
    }
    bool hasLower = hasLowerBound(prop);
    bool hasUpper = hasUpperBound(prop);
    if ( !hasLower && !hasUpper )
    {
      QMenu *constraintMenu = new QMenu("Constraints",this);
      constraintMenu->addAction(m_actionConstraints10);
      constraintMenu->addAction(m_actionConstraints50);
      constraintMenu->addAction(m_actionConstraints);
      context.addMenu(constraintMenu);
    }
    else
    {
      context.addAction(m_actionRemoveConstraints);
    }
    context.exec(QCursor::pos());
  }
  else if ( isConstraint(prop) )
  {// constraints
    QMenu context(this);
    context.addAction(m_actionRemoveConstraint);
    context.exec(QCursor::pos());
  }
}

/** 
 * Add a function to currently selected composite function property
 */
void FunctionBrowser::addFunction()
{
  QString newFunction;

  auto item = m_browser->currentItem();
  QtProperty* prop = NULL;
  if ( item )
  {
    prop = item->property();
    if (!isFunction(prop)) return;
  }

  // check if the browser is empty
  if ( !prop )
  {
    auto top = m_browser->properties();
    if ( !top.isEmpty() )
    {
      prop = top[0];
      if (!isFunction(prop)) return;
    }
  }

  // Get new function type 
  SelectFunctionDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted)
  {
    newFunction = QString::fromStdString(dlg.getSelection().function);
  }
  else
  {
    return;
  }

  if (newFunction.isEmpty()) return;

  // create new function
  auto f = Numeric::FunctionFactory::instance().createFitFunction(newFunction.toStdString());
  //newFunction = QString::fromStdString(f->asString());

  if (prop)
  {// there are other functions defined
    Numeric::IFunction_sptr fun = Numeric::FunctionFactory::instance().createFitFunction(prop->propertyName().toStdString());
    auto cf = boost::dynamic_pointer_cast<Numeric::CompositeFunction>(fun);
    if (cf)
    {
      addFunction(prop,f);
    }
    else
    {
      cf.reset(new Numeric::CompositeFunction);
      auto f0 = getFunction(prop);
      if ( f0 )
      {
        cf->addFunction(f0);
      }
      cf->addFunction(f);
      setFunction(cf);
    }
  }
  else
  {// the browser is empty - add first function
    addFunction(NULL,f);
  }
  updateFunctionIndices();
}

/**
 * Return the function 
 * @param prop :: Function property 
 */
Numeric::IFunction_sptr FunctionBrowser::getFunction(QtProperty* prop)
{
  if (prop == NULL)
  {// get overall function
    auto props = m_browser->properties();
    if (props.isEmpty()) return Numeric::IFunction_sptr();
    prop = props[0];
  }
  if (!isFunction(prop)) return Numeric::IFunction_sptr();

  // construct the function 
  auto fun = Numeric::FunctionFactory::instance().createFitFunction(prop->propertyName().toStdString());
  auto cf = boost::dynamic_pointer_cast<Numeric::CompositeFunction>(fun);
  if (cf)
  {
    auto children = prop->subProperties();
    foreach(QtProperty* child, children)
    {
      if (isFunction(child))
      {
        auto f = getFunction(child);
        // if f is null ignore that function
        if ( f )
        {
          cf->addFunction(f);
        }
      }
    }
  }
  else
  {
    // loop over the children properties and set parameters and attributes
    auto children = prop->subProperties();
    foreach(QtProperty* child, children)
    {
      if (isAttribute(child))
      {
        std::string attName = child->propertyName().toStdString();
        SetAttributeFromProperty setter(this,child);
        Numeric::IFunction::Attribute attr = fun->getAttribute(attName);
        attr.apply(setter);
        try
        {
          fun->setAttribute(attName,attr);
        }
        catch(std::exception& e)
        {
          if ( attName == "Formula" && attr.asString().empty() && children.size() == 1 )
          {
            fun->setAttributeValue("Formula","x");
          }
          else
          {
            QMessageBox::warning(this,"Mantid - Warning","Attribute " + child->propertyName() + 
              " cannot be set for function " + prop->propertyName() + ":\n" + e.what() );
            return Numeric::IFunction_sptr();
          }
        }
      }
      else if (isParameter(child))
      {
        fun->setParameter(child->propertyName().toStdString(), getParameter(child));
      }
    }
  }

  // add ties
  {
    auto from = m_ties.lowerBound(prop);
    auto to = m_ties.upperBound(prop);
    QList<QtProperty*> filedTies; // ties can become invalid after some editing
    for(auto it = from; it != to; ++it)
    {
      try
      {
        QString tie = m_tieManager->value(it.value().tieProp);
        fun->addTies(tie.toStdString());
      }
      catch(...)
      {
        //std::cerr << "tie failed " << it.value().tie.toStdString() << std::endl;
        filedTies << it.value().tieProp;
      }
    }
    // remove failed ties from the browser
    foreach(QtProperty* p, filedTies)
    {
      removeProperty(p);
    }
  }

  // add constraints
  {
    auto from = m_constraints.lowerBound(prop);
    auto to = m_constraints.upperBound(prop);
    for(auto it = from; it != to; ++it)
    {
      try
      {
        QString constraint;
        auto cp = it.value();
        if ( cp.lower )
        {
          constraint += m_constraintManager->value(cp.lower) + "<" + cp.paramProp->propertyName();
        }
        else
        {
          constraint += cp.paramProp->propertyName();
        }
        if ( cp.upper )
        {
          constraint += "<" + m_constraintManager->value(cp.upper) ;
        }
        fun->addConstraints(constraint.toStdString());
      }
      catch(...)
      {
      }
    }
  }

  return fun;
}

/**
 * Return FunctionFactory function string
 */
QString FunctionBrowser::getFunctionString()
{
  auto fun = getFunction();
  if ( !fun ) return "";
  return QString::fromStdString( fun->asString() );
}

/**
 * Remove the function under currently selected property
 */
void FunctionBrowser::removeFunction()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isFunction(prop)) return;
  removeProperty(prop);
  updateFunctionIndices();
}

/**
 * Fix currently selected parameter
 */
void FunctionBrowser::fixParameter()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isParameter(prop)) return;
  QString tie = QString::number(getParameter(prop));
  auto ap = addTieProperty(prop, tie);
  if (ap.prop)
  {
    ap.prop->setEnabled(false);
  }
}

/**
 * Unfix currently selected parameter
 */
void FunctionBrowser::removeTie()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isParameter(prop)) return;
  auto children = prop->subProperties();
  foreach(QtProperty* child, children)
  {
    if ( child->propertyName() == "Tie" )
    {
      removeProperty(child);
      return;
    }
  }
}

/**
 * Add a custom tie to currently selected parameter
 */
void FunctionBrowser::addTie()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isParameter(prop)) return;

  bool ok;
  QString tie = QInputDialog::getText(this, "Add a tie", "Tie:", QLineEdit::Normal, "", &ok);
  if ( ok && !tie.isEmpty() )
  {
    addTieProperty(prop, tie);
  }
}

/**
 * Copy function from the clipboard
 */
void FunctionBrowser::copyFromClipboard()
{
  QString funStr = QApplication::clipboard()->text();
  if ( funStr.isEmpty() ) return;
  try
  {
    auto fun = Numeric::FunctionFactory::instance().createFitFunction( funStr.toStdString() );
    if ( !fun ) return;
    this->setFunction(fun);
  }
  catch(...)
  {
    // text in the clipboard isn't a function definition
    QMessageBox::warning(this,"MantidPlot - Warning", "Text in the clipboard isn't a function definition"
      " or contains errors.");
  }
}

/**
 * Copy function to the clipboard
 */
void FunctionBrowser::copyToClipboard()
{
  auto fun = getFunction();
  if ( fun ) 
  {
    QApplication::clipboard()->setText( QString::fromStdString(fun->asString()) );
  }
}

/**
 * Add both constraints to current parameter
 */
void FunctionBrowser::addConstraints()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isParameter(prop)) return;
  addConstraintProperties(prop,"0<"+prop->propertyName()+"<0");
}

/**
 * Add both constraints to current parameter
 */
void FunctionBrowser::addConstraints10()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isParameter(prop)) return;
  double val = getParameter(prop);
  addConstraintProperties(prop,QString::number(val*0.9)+"<"+prop->propertyName()+"<"+QString::number(val*1.1));
}

/**
 * Add both constraints to current parameter
 */
void FunctionBrowser::addConstraints50()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isParameter(prop)) return;
  double val = getParameter(prop);
  addConstraintProperties(prop,QString::number(val*0.5)+"<"+prop->propertyName()+"<"+QString::number(val*1.5));
}

/**
 * Remove both constraints from current parameter
 */
void FunctionBrowser::removeConstraints()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isParameter(prop)) return;
  auto props = prop->subProperties();
  foreach(QtProperty* p, props)
  {
    if ( isConstraint(p) )
    {
      removeProperty( p );
    }
  }
}

/**
 * Remove one constraint from current parameter
 */
void FunctionBrowser::removeConstraint()
{
  auto item = m_browser->currentItem();
  if ( !item ) return;
  QtProperty* prop = item->property();
  if (!isConstraint(prop)) return;
  removeProperty( prop );
}

/**
 * Slot connected to all function attribute managers. Update the corresponding function.
 * @param prop :: An attribute property that was changed
 */
void FunctionBrowser::attributeChanged(QtProperty* prop)
{
  auto funProp = m_properties[prop].parent;
  if ( !funProp ) return;
  auto fun = Numeric::FunctionFactory::instance().createFitFunction(funProp->propertyName().toStdString());

  std::string attName = prop->propertyName().toStdString();
  SetAttributeFromProperty setter(this,prop);
  Numeric::IFunction::Attribute attr = fun->getAttribute(attName);
  attr.apply(setter);
  fun->setAttribute(attName,attr);

  setFunction(funProp, fun);
  updateFunctionIndices();
}


} // QtAPI
