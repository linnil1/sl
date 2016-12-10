from setuptools import setup, Extension

def readme():
    with open('README.rst') as f:
        return f.read()

module1 = Extension('slpyc',
                    sources = ['slpy/slpyc.c','slpy/sl.c'])

setup (name = 'slpy',
       packages=['slpy'],
       version = '1.5.2',
       license='MIT',
       author = 'linnil1',
       author_email = 'linnil1.886@gmail.com',
       url = 'https://github.com/linnil1/sl',
      classifiers=[ 'Programming Language :: Python :: 3.5'],
       description = 'SL for python',
       keywords="sl",
       long_description = '''
Turn sl.c for python use and
use generator to make it look simple
but it is really ugly methods
''',
       entry_points = {
           'console_scripts': ['slpy=slpy.command_line:main'], 
       },
       zip_safe=True,
       ext_modules = [module1])
