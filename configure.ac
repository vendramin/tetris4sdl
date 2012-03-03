dnl Process this file with autoconf to produce a configure script.

AC_INIT(tetris, 0.1)
AC_CONFIG_SRCDIR([./main.c])
AC_PREREQ(2.50)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(tetris, 0.1)
AM_CONFIG_HEADER(config.h)

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC
#AM_PROG_LIBTOOL

dnl Checks for programs.


dnl Checks for libraries.

dnl Check for SDL
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION, :,
  AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

dnl Check for SDL_ttf library
AC_CHECK_LIB(SDL_ttf, TTF_Init, ,
  AC_MSG_ERROR([Unable to find the SDL_ttf library] ))

dnl Checks for header files.


dnl Checks for typedefs, structures, and compiler characteristics.


dnl Checks for library functions.


dnl Checks for Additional stuffs.



dnl Set PACKAGE SOURCE DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`

dnl Set PACKAGE PREFIX
if test "x${prefix}" = "xNONE"; then
  packageprefix=${ac_default_prefix}
else
  packageprefix=${prefix}
fi

dnl Set PACKAGE DATA & DOC DIR
packagedatadir=share/${PACKAGE}
packagedocdir=doc/${PACKAGE}

AC_OUTPUT([Makefile])
