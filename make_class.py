import sys
import os
import os.path
import shutil

'''
Creates a class template for SmallProject
'''

if len(sys.argv) < 3:
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


header_ifdef = namespace_name + '_' + class_name + '_H'
header_ifdef = header_ifdef.upper()
has_namespace = namespace_name != 'GUI'
has_dll_export = has_namespace
tab = '  '
if is_singleton or is_factory or is_algorithm:
	is_subclass = True
else:
	is_subclass = False

# start writing the class' header file
header = '#ifndef ' + header_ifdef +'\n#define ' + header_ifdef + '\n\n'

if has_dll_export:
	header += '#include "' + namespace_name + '/DllExport.h"\n'
	
if is_singleton:
	header += '#include "API/Singleton.h"\n'
if is_algorithm:
	header += '#include "API/Algorithm.h"\n'
if is_factory:
	header += '#include "Kernel/DynamicFactory.h"\n'
header += '\n'

if has_namespace:
	header += 'namespace ' + namespace_name + '\n{\n\n'

header += 'class '
if has_dll_export: # add dll export stuff
	header += namespace_name.upper() + '_EXPORT '

header += class_name 
if is_subclass:
	header += ': '
if is_singleton:
	header += 'protected API::Singleton'
if is_factory:
	header += ', public Kernel::DynamicFactory<' + factory_type + '>'
if is_algorithm:
	header += 'public API::Algorithm'
header += '\n{\n'
header += 'public:\n'
header += tab + class_name + '();\n'
header += tab + '~' + class_name + '(){}\n'
if is_singleton:
	header += tab + 'virtual std::string name() const {return "' + class_name + '";}\n'
	header += tab + 'static ' + class_name + '& instance();\n'
if is_algorithm:
	header += tab + 'virtual std::string name() const {return "' + class_name + '";}\n'
	header += 'protected:\n' + tab + 'virtual void exec();\n'
header += '};\n\n'

if has_namespace:
	header += '} // namespace ' + namespace_name + '\n'

if is_factory:
	type_split = factory_type.split('::')
	if len(type_split) > 0:
		type_name = type_split[len(type_split)-1]
	else:
		type_name = ''
	header += '\n#define DECLARE_' + type_name.upper() + '(type) \\\n'
	header += 'int declare_'+type_name.lower()+'_##type( ('+namespace_name+'::'+class_name+'::instance().subscribe<type>(#type),0));\n\n'

header += '#endif // ' + header_ifdef + '\n'
# end writing the class' header file


# start writing the source file
source = '#include "'
if has_namespace:
	source += namespace_name + '/'
source += class_name + '.h"\n'
if is_singleton:
	source += '#include "API/Framework.h"\n'
if is_algorithm:
	source += '#include "API/AlgorithmFactory.h"\n'

source += '\n'
if has_namespace:
	source += 'namespace ' + namespace_name + '\n{\n\n'

if is_algorithm:
	source += 'DECLARE_ALGORITHM(' + class_name + ');\n\n'

source += class_name + '::' + class_name + '()\n{\n}\n\n'

if is_algorithm:
	source += 'void ' + class_name + '::exec()\n{\n}\n\n'

if is_singleton:
	source += class_name + '& ' + class_name + '::' + 'instance()\n{\n'
	source += tab + 'Singleton* s = API::Framework::instance().getSingleton("' + class_name + '");\n'
	source += tab + 'if (s == nullptr)\n'+tab+'{\n'
	source += tab + tab + class_name + ' *f = new ' + class_name + '();\n'
	source += tab + tab + 'API::Framework::instance().registerSingleton("' + class_name + '",f);\n'
	source += tab + tab + 'return *f;\n'
	source += tab + '}\n'
	source += tab + 'else\n'
	source += tab + '{\n'
	source += tab + tab + 'return *static_cast<' + class_name + '*>(s);\n'
	source += tab + '}\n}\n\n'

if has_namespace:
	source += '} // namespace ' + namespace_name + '\n'
# end writing the source file

# start writing test file
if has_namespace:
	test = '#include "gtest/gtest.h"\n'
	test += '#include "' + namespace_name + '/' + class_name + '.h"\n\n'
	test += '#include <iostream>\n\n'
	test += 'TEST('+class_name+'Test, Test)\n{\n}\n\n'
# end writing test file

# saving the code in files
header_path = namespace_name + '/inc/'
if has_namespace:
	header_path += namespace_name + '/'
header_path += class_name + '.h'
source_path = namespace_name + '/src/' + class_name + '.cpp'
test_path = namespace_name + '/test/' + class_name + 'Test.cpp'

header_file = open(header_path,'w')
header_file.write(header)
header_file.close()

source_file = open(source_path,'w')
source_file.write(source)
source_file.close()

if has_namespace:
	try:
		test_file = open(test_path,'w')
		test_file.write(test)
		test_file.close()
	except:
		pass
