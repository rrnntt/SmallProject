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
	elif sys.argv[3] == 'factory':
		is_singleton = True
		is_factory = True
		if len(sys.argv) > 4:
			factory_type = sys.argv[4]
	elif sys.argv[3] == 'algorithm':
		is_algorithm = True
	else:
		raise Exception('Cannot create ' + sys.argv[3])

# upper case names
NAMESPACE_NAME = namespace_name.upper()
CLASS_NAME = class_name.upper()

dictionary = {'Namespace':namespace_name,
				'NAMESPACE':NAMESPACE_NAME,
				'ClassName':class_name,
				'CLASSNAME':CLASS_NAME,
				'classname':CLASS_NAME.lower()
}

# define the output file names
header_folder = 'test'
cpp_folder = 'test'
test_folder = 'test'
header_file_name = header_folder+'/'+class_name + '.h'
cpp_file_name = cpp_folder+'/'+class_name + '.cpp'
test_file_name = ''
if namespace_name != 'GUI':
	test_file_name = test_folder+'/'+class_name + 'Test.cpp'
	
#define template names
header_template = ''
cpp_template = ''
if is_factory:
	header_template = 'FactoryHTemplate.txt'
	cpp_template = 'SingletonCPPTemplate.txt'
	dictionary['FactoryType'] = factory_type
	dictionary['FACTORYTYPE'] = factory_type.upper()
	dictionary['factorytype'] = factory_type.lower()
elif is_singleton:
	header_template = 'SingletonHTemplate.txt'
	cpp_template = 'SingletonCPPTemplate.txt'
else:
	header_template = 'ClassHTemplate.txt'
	cpp_template = 'ClassCPPTemplate.txt'

parse_file(header_template,header_file_name,dictionary)
parse_file(cpp_template,cpp_file_name,dictionary)
if len(test_file_name) > 0:
	parse_file('ClassTestTemplate.txt',test_file_name,dictionary)

