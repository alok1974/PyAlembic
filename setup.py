# monkey patch for _msvccompiler
import distutils.msvc9compiler
def find_vcvarsall(version):
    return "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat"
distutils.msvc9compiler.find_vcvarsall=find_vcvarsall


import distutils.tests
from distutils.core import setup, Extension
import os

VCPKG_DIR=os.environ['VCPKG_DIR']
INCLUDE_DIRS=[
        VCPKG_DIR+'/installed/x64-windows/include',
        VCPKG_DIR+'/installed/x64-windows/include/openexr'
        ]
LIBRARY_DIRS=[
        VCPKG_DIR+'/installed/x64-windows/lib'
        ]
CFLAGS=[
        '/wd4275',
        '/EHsc',
        ]

iex = Extension('iex', 
                sources = ['PyIex/iexmodule.cpp'],
                include_dirs = INCLUDE_DIRS,
                library_dirs= LIBRARY_DIRS,
                libraries=[
                    'Iex-2_2',
                    ],
                extra_compile_args=CFLAGS
                )

imath = Extension('imath', 
                sources = [
                    'PyImath/imathmodule.cpp',
                    'PyImath/PyImath.cpp',
                    'PyImath/PyImathBasicTypes.cpp',
                    'PyImath/PyImathBox.cpp',
                    'PyImath/PyImathBox2Array.cpp',
                    'PyImath/PyImathBox3Array.cpp',
                    'PyImath/PyImathColor3.cpp',
                    'PyImath/PyImathColor4.cpp',
                    'PyImath/PyImathEuler.cpp',
                    'PyImath/PyImathFixedArray.cpp',
                    'PyImath/PyImathFixedVArray.cpp',
                    'PyImath/PyImathFrustum.cpp',
                    'PyImath/PyImathFun.cpp',
                    'PyImath/PyImathLine.cpp',
                    'PyImath/PyImathMatrix33.cpp',
                    'PyImath/PyImathMatrix44.cpp',
                    'PyImath/PyImathPlane.cpp',
                    'PyImath/PyImathQuat.cpp',
                    'PyImath/PyImathRandom.cpp',
                    'PyImath/PyImathShear.cpp',
                    'PyImath/PyImathStringArray.cpp',
                    'PyImath/PyImathStringTable.cpp',
                    'PyImath/PyImathTask.cpp',
                    'PyImath/PyImathUtil.cpp',
                    'PyImath/PyImathVec2fd.cpp',
                    'PyImath/PyImathVec2si.cpp',
                    'PyImath/PyImathVec3fd.cpp',
                    'PyImath/PyImathVec3si.cpp',
                    'PyImath/PyImathVec3siArray.cpp',
                    'PyImath/PyImathVec4fd.cpp',
                    'PyImath/PyImathVec4si.cpp',
                    'PyImath/PyImathVec4siArray.cpp',
                    ],
                include_dirs = INCLUDE_DIRS+['PyImath'],
                library_dirs= LIBRARY_DIRS,
                libraries=[
                    'Iex-2_2',
                    'IexMath-2_2',
                    'Imath-2_2',
                    ],
                extra_compile_args=CFLAGS,
                define_macros=[
                    ('PYIMATH_EXPORTS', '1')
                    ]
                )


from distutils.cmd import Command
class TestCommand(Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        import sys, subprocess

        raise SystemExit(
                subprocess.call([sys.executable,
                    'tests'
                    ]))


setup (name = 'pyalembic',
       version = '1.0',
       description = 'python binding of Alembic',
       ext_modules = [iex, imath],
       cmdclass={
           'test': TestCommand
           }
       )

