import sys
import os
import os.path
import shutil

'''

Creates a CMake project for the GSL library.
Tested with gsl-1.9

IMPORTANT!

Disable language extension in MSVC properties->C++->Language both in Debug and Release
If you see failing in rowcol.c for complex then extensions were not disabled (__STDC__ is undefined or 0).

utils/memcpy.c doesn't compile in release
config.h.in must be copied to the _cmake directory (next to CMakeLists.txt) and renamed to config.h
some math function defs may need to be removed from it (hypot, ldexp, frexp).  
add #define inline __inline at the bottom of the file.

'''

if len(sys.argv) < 2:
	print 'usage: ',os.path.basename(sys.argv[0]),'<gsl directory>'
	exit(1)

gsl_dir = sys.argv[1]

if not os.path.isdir(gsl_dir):
	print gsl_dir,'is not a directory'
	exit(1)

'''
Create a directory for build files
'''
cmake_dir = gsl_dir + '_cmake'
if os.path.exists(cmake_dir):
	if os.path.isdir(cmake_dir):
		shutil.rmtree(cmake_dir)
	else:
		print cmake_dir,'exists and it\'s not a directory'
		exit(1)
os.mkdir(cmake_dir)

include_dir = os.path.join(cmake_dir,'include/gsl')
os.mkdir(os.path.join(cmake_dir,'include'))
os.mkdir(include_dir)

src = [] # source files
inc = ['config.h'] # include files
# source include files which aren't picked up automatically
src_inc = ['bitreverse.c','compare_source.c','c_main.c','hc_main.c','real_main.c','signals_source.c','find2d.c',
'rowcol_source.c',
'submatrix_source.c',
'real_unpack.c',
'lmpar.c',
'dogleg.c',
'beta_inc.c',
'',
]

for root, dirs, files in os.walk(gsl_dir):
	if root != os.path.basename(gsl_dir):
		#print root
		dir = os.path.basename(root)
		for file in files:
			if file.endswith('.h'):
				if file.startswith('gsl_'):
					shutil.copy(os.path.join(root,file),os.path.join(cmake_dir,'include','gsl',file))
					inc_file = os.path.join('include','gsl',file).replace('\\','/')
					inc.append(inc_file)
				else:
					shutil.copy(os.path.join(root,file),os.path.join(cmake_dir,dir,file))
					inc_file = os.path.join(dir,file).replace('\\','/')
					inc.append(inc_file)
			elif file.endswith('.c') and not file.startswith('test'):
				shutil.copy(os.path.join(root,file),os.path.join(cmake_dir,dir,file))
				if file.startswith('fp-'):
					continue
				f = open(os.path.join(root,file),'r')
				lines = f.readlines()
				to_src = False
				if file not in src_inc:
					for line in lines:
						if 'include' in line:
							to_src = True
							break
				if to_src:
					src_file = os.path.join(dir,file).replace('\\','/')
					src.append(src_file)
	else:
		#f = open(os.path.join(gsl_dir,'acconfig.h'),'r')
		#lines = f.readlines()
		#f.close()
		#lines[0] = '#define inline __inline'
		#f = open(os.path.join(cmake_dir,'config.h'),'w')
		#f.writelines(lines)
		#f.close()
		del dirs[dirs.index('doc')]
		del dirs[dirs.index('gsl')]
		#del dirs[dirs.index('test')]
		#del dirs[dirs.index('ieee-utils')]
		for dir in dirs:
			if dir != 'doc' and dir != 'gsl':
				os.mkdir(os.path.join(cmake_dir,dir))
		for file in files:
			if file.endswith('.h'):
				inc_file = os.path.join('include','gsl',file).replace('\\','/')
				inc.append(inc_file)
				shutil.copy(os.path.join(root,file),os.path.join(cmake_dir,'include','gsl',file))

cmakelists = open(os.path.join(cmake_dir,'CMakeLists.txt'),'w')
cmakelists.write('project(gsl)\n')
cmakelists.write('set( INC_FILES '+' '.join(inc)+' )\n')
cmakelists.write('set( SRC_FILES '+' '.join(src)+' )\n')
cmakelists.write('add_library( gsl ${SRC_FILES} ${INC_FILES} )\n')
cmakelists.write('include_directories(. include include/gsl utils)\n')
cmakelists.write('add_definitions(-DHAVE_DECL_HYPOT -DHAVE_DECL_LDEXP -DHAVE_DECL_FREXP)\n')
# disable language extensions
cmakelists.write('set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /Za )\n')
cmakelists.write('set( CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} /Za )\n')
cmakelists.write('set( CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} /Za )\n')
cmakelists.write('set( CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_MINSIZEREL} /Za )\n')
cmakelists.write('set( CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Za )\n')
cmakelists.close()
