
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
//#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <iostream>
#include <string>

typedef std::string::const_iterator iterator_type;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct fun2
{
    std::string name;
    std::string arg1;
    std::string arg2;
};

BOOST_FUSION_ADAPT_STRUCT(
    fun2,
    (std::string, name)
    (std::string, arg1)
    (std::string, arg2)
)

struct test_parser : qi::grammar<std::string::const_iterator, fun2()>
{
  test_parser() : test_parser::base_type(start)
  {
    using qi::eps;
    using qi::lit;
    using qi::_val;
    using qi::_1;
    using ascii::char_;

    name_string %= qi::lexeme[ +(qi::alnum) ];
    start %= name_string >> '(' >> name_string >> ','  >> name_string >> ')' ;
  }

  qi::rule<std::string::const_iterator, fun2()> start;
  qi::rule<std::string::const_iterator, std::string()> name_string;
};

int test_spirit()
{


    test_parser my_parser; // Our grammar

    std::string str = "dist(x1,y2)";
    fun2 result;

    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();

    bool r = parse(iter, end, my_parser, result);

    if (r && iter == end)
    {
      std::cout << "-------------------------\n";
      std::cout << "Parsing succeeded\n";
      std::cout << "result = " << result.name <<'(' << result.arg1<<','<<result.arg2 <<')'<< std::endl;
      std::cout << "-------------------------\n";
    }
    else
    {
      std::string rest(iter, end);
      std::cout << "-------------------------\n";
      std::cout << "Parsing failed\n";
      std::cout << "stopped at: \": " << rest << "\"\n";
      std::cout << "-------------------------\n";
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Another parser
///////////////////////////////////////////////////////////////////////////////



//struct mini_xml;
//
//typedef
//    boost::variant<
//        boost::recursive_wrapper<mini_xml>
//      , std::string
//    >
//mini_xml_node;

struct mini_xml
{
    std::string name;                           // tag name
    std::vector<mini_xml> children;        // children
};

BOOST_FUSION_ADAPT_STRUCT(
    mini_xml,
    (std::string, name)
    (std::vector<mini_xml>, children)
)

struct mini_xml_parser : qi::grammar<std::string::const_iterator, mini_xml(), qi::locals<std::string>, ascii::space_type>
{
  mini_xml_parser() : mini_xml_parser::base_type(xml)
  {
    using qi::eps;
    using qi::lit;
    using qi::_val;
    using qi::_1;
    using ascii::char_;
    using namespace qi::labels;

    start_tag =
                    '<'
                >>  !lit('/')
                >>  qi::lexeme[+(char_ - '>')       [_val += _1]]
                >>  '>'
            ;

            end_tag =
                    "</"
                >>  ascii::string(_r1)
                >>  '>'
            ;

    xml %=
                start_tag[_a = _1]
            >>  *xml
            >>  end_tag(_a)
        ;
  }

  qi::rule<std::string::const_iterator, mini_xml(), qi::locals<std::string>, ascii::space_type> xml;
  qi::rule<std::string::const_iterator, std::string(), ascii::space_type> start_tag;
  qi::rule<std::string::const_iterator, void(std::string), ascii::space_type> end_tag;
};

int test_mini_xml()
{
  mini_xml xml;
  mini_xml_parser xml_parser;
  std::string code = "<a> <b></b><c></c> </a>";
  std::string::const_iterator iter = code.begin();
  std::string::const_iterator end = code.end();
  bool r = qi::phrase_parse(iter, end, xml_parser, ascii::space, xml);
  if (r && iter == end)
  {
    std::cerr << "OK" << std::endl;
    std::cerr << xml.name << std::endl;
    std::cerr << xml.children[0].name << ' ' << xml.children[1].name << std::endl;
  }
  else
  {
    std::cerr << "Failed" << std::endl;
    std::cerr << *iter << std::endl;
  }
  return 0;
}

