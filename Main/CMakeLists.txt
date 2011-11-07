#
#  Main project
#
project(Main)

set( INC_FILES 
  test_spirit.h
  test_parser.h
   )
set( SRC_FILES 
  main.cpp
  test_spirit.cpp
  test_parser.cpp
  roman_numerals.cpp
  )
  
add_executable( Main ${SRC_FILES} ${INC_FILES} )
include_directories(inc 
../Kernel/inc
../Formula/inc
#../Numeric/inc
${THIRD_PARTY_INCLUDE_DIR}
)

target_link_libraries(Main
Numeric Kernel Formula
)
