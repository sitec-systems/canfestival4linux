# - Check for the presence of RT
#
# The following variables are set when RT is found:
#  HAVE_RT       = Set to true, if all components of RT
#                          have been found.
#  RT_INCLUDES   = Include path for the header files of RT
#  RT_LIBRARIES  = Link these to use RT

## -----------------------------------------------------------------------------
## Check for the header files

find_path (RT_INCLUDE_DIR time.h
  PATHS 
    /usr/local/include
    /usr/include
    "$ENV{SDKTARGETSYSROOT}/usr/include"
  )

## -----------------------------------------------------------------------------
## Check for the library

find_library (RT_LIBRARY rt
  PATHS 
    /usr/local/lib 
    /usr/lib 
    /lib
    "$ENV{SDKTARGETSYSROOT}/usr/lib"
  )

## -----------------------------------------------------------------------------
## Actions taken when all components have been found

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RT DEFAULT_MSG
    RT_INCLUDE_DIR
    RT_LIBRARY)
mark_as_advanced(RT_LIBRARIES RT_INCLUDES)
set(RT_LIBRARIES ${RT_LIBRARY})
set(RT_INCLUDE_DIRS ${RT_INCLUDE_DIR})
