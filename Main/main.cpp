#include "Kernel/Framework.h"
#include "test_spirit.h"
#include "test_parser.h"
#include "test_problem.h"

#include "Formula/Expression.h"

int main( int argc, char** argv )
{
  Kernel::Framework::instance().parseCommandLine(argc,argv);
  test_problem();
  return 0;
}
