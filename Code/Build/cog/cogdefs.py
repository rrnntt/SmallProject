import cog

namespace = 'MyNamespace'

def start_namespace(namespace):
    cog.outl('namespace '+namespace+'\n{')

def end_namespace(namespace):
    cog.outl('} // ' + namespace.upper())

def class_template(namespace, classname):
    start_namespace(namespace)
    end_namespace(namespace)

