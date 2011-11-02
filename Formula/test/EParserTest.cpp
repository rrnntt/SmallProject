#include "gtest/gtest.h"
#include "Formula/EParser.h"

#include <boost/spirit/include/qi.hpp>

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


TEST(EParserTest, Simple) 
{
  EXPECT_EQ(1+1,2);
  std::cerr << "Hello!\n";
  std::string input("1,2,3,4");
  std::cerr << "parse " << parse_numbers(input.begin(),input.end()) << std::endl;
}
