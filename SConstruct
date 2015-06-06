## INFO ########################################################################
##                                                                            ##
##                                  MeaCulpa                                  ##
##                                  ========                                  ##
##                                                                            ##
##      Sophisticated, minimalistic and high-level error handling for C       ##
##                       Version: 0.1.4.165 (20150606)                        ##
##                              File: SConstruct                              ##
##                                                                            ##
##               For more information about the project, visit                ##
##                  <https://github.com/petervaro/MeaCulpa>.                  ##
##                       Copyright (C) 2015 Peter Varo                        ##
##                                                                            ##
##  This program is free software: you can redistribute it and/or modify it   ##
##   under the terms of the GNU General Public License as published by the    ##
##       Free Software Foundation, either version 3 of the License, or        ##
##                    (at your option) any later version.                     ##
##                                                                            ##
##    This program is distributed in the hope that it will be useful, but     ##
##         WITHOUT ANY WARRANTY; without even the implied warranty of         ##
##            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.            ##
##            See the GNU General Public License for more details.            ##
##                                                                            ##
##     You should have received a copy of the GNU General Public License      ##
##     along with this program, most likely a file in the root directory,     ##
##        called 'LICENSE'. If not, see <http://www.gnu.org/licenses>.        ##
##                                                                            ##
######################################################################## INFO ##

# Import python modules
from os.path import join
from shutil  import copy
from sys     import platform

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# Basic settings
debug      = False
input_dir  = 'MeaCulpa'
build_dir  = 'build'
output_dir = 'dist'
output_lib = 'MeaCulpa'
input_src  = ['MeaCulpa.c']


#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# Indicate the beginning of building
print '\n{:#^80}\n'.format(' SCONS BUILD ')

# C Compiler (gcc|clang)
CC = 'clang'

# C Compiler Flags
CCFLAGS = ['v',
           'O3',
           'Wall',
           'Wextra',
           'pedantic',
           'std=c11',
           'g' if debug else '']

# GCC Specific Compiler Flags
GCCFLAGS = ['fdiagnostics-color=always']

# CLANG Specific Compiler Flags
CLANGFLAGS = ['fcolor-diagnostics',
              'fmacro-backtrace-limit=0']

# Add compiler specific flags
CCFLAGS.extend(GCCFLAGS if CC == 'gcc' else CLANGFLAGS if CC == 'clang' else [])

# C Pre-Processor Path (Include)
CPPPATH = ['MeaCulpa',
           '/usr/include',
           '/usr/local/include']

# Library paths
LIBPATH = ['/usr/lib',
           '/usr/local/lib']


#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# MAINTENANCE
maintain_run = Environment().Command(target='maintain',
                                     source=None,
                                     action='python3 maintain.py')


#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# CONTAINER LIBRARY

# Create environment
environment = \
    Environment(CC=CC,
                CCFLAGS=['-' + flag for flag in CCFLAGS if flag],
                CPPPATH=CPPPATH,
                LIBPATH=LIBPATH)
# Specify output directory
environment.VariantDir(variant_dir=build_dir,
                       src_dir=input_dir)
# Create library
environment_to_lib = \
    environment.SharedLibrary(target=join(output_dir, output_lib),
                              source=[join(build_dir, f) for f in input_src])

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# TESTS

# Create environment
environment_test = \
    Environment(CC=CC,
                CCFLAGS=['-' + flag for flag in CCFLAGS if flag],
                CPPPATH=CPPPATH,
                LIBPATH=LIBPATH,
                LIBS=[output_lib])
# Specify output directory
environment_test.VariantDir(variant_dir=build_dir,
                            src_dir=input_dir)
# Create program
environment_test_out = \
    environment_test.Program(target=join(output_dir, 'test'),
                             source=[join(build_dir, 'test.c')])

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# DOC

# Define variables
CPPDEFINES = {
    # 'E1a': None,
    'E1b': None,
    # 'E2' : None,
}

# Create environment
environment_doc = \
    Environment(CC=CC,
                CCFLAGS=['-' + flag for flag in CCFLAGS if flag],
                CPPPATH=CPPPATH,
                CPPDEFINES=CPPDEFINES,
                LIBPATH=LIBPATH,
                LIBS=[output_lib])
# Specify output directory
environment_doc.VariantDir(variant_dir=build_dir,
                           src_dir=input_dir)
# Create program
environment_doc_out = \
    environment_doc.Program(target=join(output_dir, 'doc'),
                            source=[join(build_dir, 'doc.c')])

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# INSTALLATION
def install(*args, **kwargs):
    if platform in ('linux', 'linux2'):
        copy(join(input_dir,  'MeaCulpa.h'),
             join(CPPPATH[2], 'MeaCulpa.h'))
        copy(join(output_dir, 'lib' + output_lib + '.so'),
             join(LIBPATH[1], 'lib' + output_lib + '.so'))
        print "MeaCulpa has been installed"
    elif platform == 'darwin':
        copy(join(input_dir,  'MeaCulpa.h'),
             join(CPPPATH[2], 'MeaCulpa.h'))
        copy(join(output_dir, 'lib' + output_lib + '.dylib'),
             join(LIBPATH[1], 'lib' + output_lib + '.dylib'))
        print "MeaCulpa has been installed"
    elif platform == 'win32':
        print "Installation of MeaCulpa is not supported yet"
    else:
        print "Installation of MeaCulpa is not supported yet"

environment_install = Environment().Command(target='install',
                                            source=None,
                                            action=install)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# Before creating lib, run maintain script
environment.Depends(environment_to_lib, maintain_run)
# Before install, create lib
environment.Depends(environment_install, environment_to_lib)
# Before running tests, install lib
environment_test.Depends(environment_test_out, environment_install)
# Before running docs, run rests
environment_doc.Depends(environment_doc_out, environment_test_out)
