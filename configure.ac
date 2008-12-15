#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([csmile],[0.0.1],[aalonso00@gmail.com])
AM_INIT_AUTOMAKE([dist-bzip2 foreign])
AC_CONFIG_SRCDIR([src/csmile-gtk-ui.h])
AC_CONFIG_HEADER([config.h])


AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
#AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB(gthread-2.0, g_thread_init, gstreamer-0.10)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

AM_MAINTAINER_MODE

if test "x$GCC" = "xyes"; then CFLAGS="$CFLAGS -Wall -pedantic"; fi

# Checks for library functions.

dnl #####################################
dnl ### Basic dependencies - Required ###
dnl #####################################
gobject_modules="gobject-2.0 >= 2.6.0
		    glib-2.0 >= 2.6.0
		    gtk+-2.0 >= 2.6.0
            gdk-2.0 >= 2.6.0
            gstreamer-0.10 >= 0.10
            gstreamer-interfaces-0.10 >= 0.10"

PKG_CHECK_MODULES([GOBJECT], [$gobject_modules])

#GTK_DOC_CHECK([1.4])		  

AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT
