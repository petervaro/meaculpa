## INFO ##
## INFO ##

# Indicate the beginning of building
print '{:#^80}'.format(' SCONS BUILD ')

# Import python modules
from os.path   import join
from itertools import chain
from copy      import deepcopy


#------------------------------------------------------------------------------#
# If developer mode passed ($ scons devel=yes)
DEVEL = ARGUMENTS.get('devel', False)



# Set global variables
#------------------------------------------------------------------------------#
# C compiler
CC = ARGUMENTS.get('cc', 'clang')

# C compiler (generic) flags
CCFLAGS = ['v',
           'g',
           'O3',
           'Wall',
           'Wextra',
           'Wpedantic',
           'std=c11',]

# GCC specific c compiler flags
GCCFLAGS = ['fdiagnostics-color=always']
GCCFLAGS_DEVEL = []

# CLANG specific  compiler flags
CLANGFLAGS = ['fcolor-diagnostics',
              'fmacro-backtrace-limit=0']
CLANGFLAGS_DEVEL = ['Weverything']



# Set flags based on the above variables
#------------------------------------------------------------------------------#
if CC.startswith('clang'):
    if DEVEL:
        CLANGFLAGS.extend(CLANGFLAGS_DEVEL)
    flags = CLANGFLAGS
elif CC.startswith('gcc'):
    if DEVEL:
        GCCFLAGS.extend(GCCFLAGS_DEVEL)
    flags = GCCFLAGS
else:
    raise ValueError('CC is not supported: {}'.format(CC))

# Finalize flags
CCFLAGS = ['-' + flag for flag in chain(CCFLAGS, flags)]


# Helper functions
#------------------------------------------------------------------------------#
def path_prefixer(folder, files):
    return [join(folder, file) for file in files]



# Target: building different versions of meaculpa
#------------------------------------------------------------------------------#
def meaculpa(version=''):
    # Output and inputs
    INPUT_DIR  = '.'
    OUTPUT_DIR = join('build', version)
    TARGET     = join('libs', 'meaculpa')
    SOURCE     = [join('src', 'error.c'),
                  join('src', 'panic.c'),]

    # C pre-processor path (include)
    CPPPATH = [join(INPUT_DIR, 'include')]
    # Library path
    LIBPATH = ['/usr/lib',
               '/usr/local/lib']
    # Libraries
    LIBS    = ['jemalloc']

    # Define version
    ccflags = deepcopy(CCFLAGS)
    ccflags.append(('-DMC_' + version.upper()) if version else '')

    # Add suffix
    TARGET += ('_' + version) if version else ''

    # Create environment
    environment = Environment(CC      = CC,
                              CCFLAGS = ccflags,
                              CPPPATH = CPPPATH,
                              LIBPATH = LIBPATH,
                              LIBS    = LIBS)

    # Specify output directory
    environment.VariantDir(variant_dir = OUTPUT_DIR,
                           src_dir     = INPUT_DIR)

    # Build
    build = environment.SharedLibrary(target = TARGET,
                                      source = path_prefixer(OUTPUT_DIR,
                                                             SOURCE))

    # Name this build
    environment.Alias('meaculpa', build)
    Default(build)
    return build



# Target: building sample files
#------------------------------------------------------------------------------#
def samples():
    # Output and inputs
    INPUT_DIR  = 'samples'
    OUTPUT_DIR = join('build', 'samples')
    TARGET     = join('bins', 'sample')
    SOURCE     = ['example01.c']

    # C pre-processor path (include)
    CPPPATH = ['/usr/include',
               '/usr/local/include']
    # Library path
    LIBPATH = ['/usr/lib',
               '/usr/local/lib']
    # Libraries
    LIBS    = ['meaculpa_safe']

    # Define version
    ccflags = deepcopy(CCFLAGS)
    ccflags.append('-DMC_SAFE')

    # Create environment
    environment = Environment(CC      = CC,
                              CCFLAGS = ccflags,
                              CPPPATH = CPPPATH,
                              LIBPATH = LIBPATH,
                              LIBS    = LIBS)

    # Specify output directory
    environment.VariantDir(variant_dir = OUTPUT_DIR,
                           src_dir     = INPUT_DIR)

    # Build
    build = environment.Program(target = TARGET,
                                source = path_prefixer(OUTPUT_DIR,
                                                       SOURCE))

    # Name this build
    environment.Alias('samples', build)
    return build



#------------------------------------------------------------------------------#
# Create build options
meaculpa()
meaculpa('fast')
Depends(samples(), meaculpa('safe'))
# samples()
