#include "gtest/gtest.h"
#include "Formula/EParser.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
bool parse_numbers(Iterator first, Iterator last)
{
  using qi::double_;
  using qi::phrase_parse;
  using ascii::space;

  bool r = phrase_parse(
    first,                          /*< start iterator >*/
    last,                           /*< end iterator >*/
    double_ >> *(',' >> double_),   /*< the parser >*/
    space                           /*< the skip-parser >*/
    );
  if (first != last) // fail if we did not get a full match
    return false;
  return r;
}

struct writer
{
  void print(int const& i) const
  {
    std::cerr << "writer " << i << std::endl;
  }
};

TEST(EParserTest, Simple) 
{
  EXPECT_EQ(1+1,2);
  std::cerr << "Hello!\n";
  std::string input("1,2,3,4");
  std::cerr << "parse " << parse_numbers(input.begin(),input.end()) << std::endl;

  writer w;
  input = "{14}";
  auto first = input.begin();
  auto last = input.end();
  qi::parse(first, last, '{' >> qi::int_[boost::bind(&writer::print, &w, _1)] >> '}');
}

namespace my1
{
    struct test_parser : qi::grammar<std::string::const_iterator, int()>
    {
        test_parser() : test_parser::base_type(start)
        {
            using qi::eps;
            using qi::lit;
            using qi::_val;
            using qi::_1;
            using ascii::char_;

            start = eps             [_val = 0]
            ;
        }

        qi::rule<std::string::const_iterator, int()> start;
    };
}
