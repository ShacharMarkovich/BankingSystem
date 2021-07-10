# how far down are we?
# if you're down one dir, this should be ../, two would be
# ../../, etc
path_to_top := ../../

# what's our name, so can invoke ourselves recursively?
mfile := xplat.mk

# define the targets and their components for this makefile

# if we build a static library, name it here (stem only)
LLIB =
# and then name the objects that go in it
LIBOBJ =

# if we have prebuilt libs to install, name them here, they can either
# be a single version in this folder, or a per-buildtype version in a
# folder whose name matches the buildtype under the current folder
INSTALLLIBS =
# if they come from somewhere besides ./lib/${buildtype}/, define it
# here.  Needs to end in a "/" if not empty
# PREBUILTLIBPATH =

# if we have local headers to install, name them here
INSTALLHEADERS = epid_issuer.h    \
                 epid_member.h    \
                 epid_verifier.h  \
                 epid_types.h     \
                 epid_constants.h \
                 epid_prng.h      \
                 epid_macros.h \
                 epid_errors.h

# if they don't come from ./, tell us whence
INCFROM = 
# if they should be installed in a subfolder under the main includes,
# tell us here
INCSUB =
# if there are extra include directories we should add to our
# compiles, name them here
EXTRAINCDIRS =

# if we have a binary to build, name it here
BIN =
# and tell us what objects go into it
BINOBJ =

# if we depend on non-system installed libraries to build our
# binaries, name them here, name-stems only
REQLIB =

# if we need extra system libraries (beyond libc) to build our binary,
# name them here, name-stems only.  Note that it may more sense to
# define the system libraries you'd like to add in the project-local
# os-specific defs files, since the names are never going to be the
# same, platform to platform, right?
SYSLIBS =

# if we have subdirectories that we need to build, name them here
SUBDIRS = 

# that's it for targets.  If we have any definition overrides
# we need that aren't project-wide, do them here

include ${path_to_top}_build/xplat/xplatdefs.mk

# if we have any definition elaborations to do that aren't
# project-wide, do them here.  For example:
# CCOPTS += whatever

# if we want to replace the default rules but don't need to
# project-wide, do it here.  Be sure to look at xplatmasterrules.mk
# and define the guard variable for the rule so it survives

include ${path_to_top}_build/xplat/xplatdefaultrule.mk

# if we have any rules replacements that we need that aren't
# project-wide, do them here.  Be sure to look at xplatmasterrules.mk
# and define the guard variables for the rules you provide, so they
# survive.

include ${path_to_top}_build/xplat/xplatrules.mk

