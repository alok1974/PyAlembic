# monkey patch for _msvccompiler
import distutils.msvc9compiler
def find_vcvarsall(version):
    return "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat"
distutils.msvc9compiler.find_vcvarsall=find_vcvarsall


import distutils.tests
from distutils.core import setup, Extension
import os

iex = Extension('iex', 
                sources = ['PyIex/iexmodule.cpp'],
                include_dirs = [
                    os.environ['VCPKG_DIR']+'/installed/x64-windows/include',
                    os.environ['VCPKG_DIR']+'/installed/x64-windows/include/openexr'
                    ],
                library_dirs=[
                    os.environ['VCPKG_DIR']+'/installed/x64-windows/lib'
                    ],
                libraries=[
                    'Iex-2_2',
                    ]
                )

setup (name = 'pyalembic',
       version = '1.0',
       description = 'python binding of Alembic',
       ext_modules = [iex],
       test_suite = 'tests'
       )
