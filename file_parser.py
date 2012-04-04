#import sys
import os
import os.path
#import shutil

def replace_templates(s,d):
	'''
	Replace templates in string s with values. The values are taken from dictionary d.
	Templates must have form ${NAME}. d contains pairs 'NAME':'VALUE', i.e. without ${} wrapping
	'''
	ss = s
	for name,value in d.iteritems():
		tmpl = '${'+name+'}'
		ss = ss.replace(tmpl,value)
	return ss

def parse_file(infile,outfile,d):
	'''
	Replaces templates in file infile with values in d and saves result in outfile.
	infile and outfile are file names.
	'''
	if infile == outfile:
		raise Exception('Do not override the template file!')

	if os.path.exists(outfile):
		print 'File ',outfile,' already exists.\nDo you want to overwrite it (yes/no)?'
		ans = 'no'
		try:
			ans = raw_input('>')
		except:
			pass
		if ans != 'yes':
			print 'Action canceled'
			return

	ifil = open(infile,'r')
	s = ifil.read()
	ifil.close()
	ofil = open(outfile,'w')
	ofil.write(replace_templates(s,d))
	ofil.close()

if __name__ == "__main__":
	d = {'a':'b','b':'a'}

	s = '${a} is b, ${b} is a'

	print s
	print replace_templates(s,d)

	parse_file('templ1.txt','class1.hxx',{'class_name':'Derived','parent_class':'Base'})
