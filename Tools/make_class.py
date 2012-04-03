import sys
import os
import os.path
import shutil
import file_parser
from file_parser import parse_file

'''
Creates a class template for SmallProject
'''

if len(sys.argv) < 3:
	print sys.argv
	print 'usage: ',os.path.basename(sys.argv[0]),'<namespace name>','<class name>', '[singleton|factory <type>|algorithm]'
	exit(1)

namespace_name = sys.argv[1]
class_name = sys.argv[2]

is_singleton = False
is_factory = False
factory_type = ''
is_algorithm = False
if len(sys.argv) > 3:
	if sys.argv[3] == 'singleton':
		is_singleton = True
	if sys.argv[3] == 'factory':
		is_singleton = True
		is_factory = True
		if len(sys.argv) > 4:
			factory_type = sys.argv[4]
	if sys.argv[3] == 'algorithm':
		is_algorithm = True

# upper case names
NAMESPACE_NAME = namespace_name.upper()
CLASS_NAME = class_name.upper()

dictionary = {'Namespace':namespace_name,
				'NAMESPACE':NAMESPACE_NAME,
				'ClassName':class_name,
				'CLASSNAME':CLASS_NAME
}

# define the output file names
header_file_name = class_name + '.h'
cpp_file_name = class_name + '.cpp'
test_file_name = ''
if namespace_name != 'GUI':
	test_file_name = class_name + 'Test.cpp'

parse_file('ClassHTemplate.txt',header_file_name,dictionary)
parse_file('ClassCPPTemplate.txt',cpp_file_name,dictionary)
if len(test_file_name) > 0:
	parse_file('ClassTestTemplate.txt',test_file_name,dictionary)

