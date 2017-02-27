from distutils.core import setup, Extension
from os.path import dirname, abspath

DIR = dirname(abspath(__file__))

ardHdlc = Extension(
    'ardHdlc',
    sources = [
        DIR + '/hdlc.c',
		DIR + '/python4ardHdlc.c'
    ],
    include_dirs = [
        DIR
    ],
)

setup(
	name = 'python4ardHdlc',
    ext_modules=[ardHdlc],
)
