dnl Process this file with autoconf to produce a configure script.

# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT(oilm, 0.1, andreas.volz@tux-style.com)
AC_PREREQ(2.52)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ISC_POSIX

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST
AC_C___ATTRIBUTE__

define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

#AM_GNU_GETTEXT([external])
#AM_GNU_GETTEXT_VERSION(0.14)

MODULE="oilm"
AC_SUBST(MODULE)

PKG_CHECK_MODULES(E, [enlightenment eina])
release=$(pkg-config --variable=release enlightenment)
MODULE_ARCH="$host_os-$host_cpu-$release"
AC_SUBST(MODULE_ARCH)
AC_DEFINE_UNQUOTED(MODULE_ARCH, "$MODULE_ARCH", "Module architecture")

AC_PATH_PROG(EDJE_CC, "edje_cc", "", $PATH)


datadir="${e_modules}/${PACKAGE}"    
AC_ARG_ENABLE(homedir-install,
  AS_HELP_STRING([--enable-homedir-install], [Install module in homedir]),
  [ datadir="${HOME}/.e/e/modules/${PACKAGE}" ]
)

AC_OUTPUT([
Makefile
src/Makefile
module.desktop
], [
])
