from distutils.core import setup, Extension

module1 = Extension('slpy',
                    sources = ['slpy.c','sl.c'])

setup (name = 'slpy',
       version = '1.0',
       description = 'SL for python',
       author = 'linnil1',
       author_email = 'linnil1.886@gmail.com',
       url = 'https://github.com/linnil1/sl',
       long_description = '''
Turn sl.c for python use and
use generator to make it look simple
but it is really ugly methods
''',
       ext_modules = [module1])
