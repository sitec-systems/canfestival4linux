# - Check for the presence of DL
#
# The following variables are set when DL is found:
#  HAVE_DL       = Set to true, if all components of DL
#                          have been found.
#  DL_INCLUDES   = Include path for the header files of DL
#  DL_LIBRARIES  = Link these to use DL

## -----------------------------------------------------------------------------
## Check for the header files

find_path (DL_INCLUDE_DIR dlfcn.h
  PATHS 
    /usr/local/include
    /usr/include
    "$ENV{SDKTARGETSYSROOT}/usr/include"
  )

## -----------------------------------------------------------------------------
## Check for the library

find_library (DL_LIBRARY dl
  PATHS 
    /usr/local/lib 
    /usr/lib 
    /lib
    "$ENV{SDKTARGETSYSROOT}/usr/lib"
  )

## -----------------------------------------------------------------------------
## Actions taken when all components have been found

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DL DEFAULT_MSG
    DL_INCLUDE_DIR
    DL_LIBRARY)
mark_as_advanced(DL_LIBRARIES DL_INCLUDES)
set(DL_LIBRARIES ${DL_LIBRARY})
set(DL_INCLUDE_DIRS ${DL_INCLUDE_DIR})

