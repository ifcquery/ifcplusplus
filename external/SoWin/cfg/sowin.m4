############################################################################
# Usage:
#   SIM_AC_HAVE_SOWIN_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the SoWin development system.  If it is found,
#   the set of variables listed below are set up as described and made
#   available to the configure script.
#
#   The $sim_ac_sowin_desired variable can be set to false externally to
#   make SoWin default to be excluded.
#
# Autoconf Variables:
# > $sim_ac_sowin_desired     true | false (defaults to true)
# < $sim_ac_sowin_avail       true | false
# < $sim_ac_sowin_cppflags    (extra flags the preprocessor needs)
# < $sim_ac_sowin_ldflags     (extra flags the linker needs)
# < $sim_ac_sowin_libs        (link library flags the linker needs)
# < $sim_ac_sowin_datadir     (location of SoWin data files)
# < $sim_ac_sowin_version     (the libSoWin version)
#
# Authors:
#   Lars J. Aas, <larsa@sim.no>
#   Morten Eriksen, <mortene@sim.no>
#
# TODO:
#

AC_DEFUN([SIM_AC_HAVE_SOWIN_IFELSE], [
AC_PREREQ([2.14a])

# official variables
sim_ac_sowin_avail=false
sim_ac_sowin_cppflags=
sim_ac_sowin_ldflags=
sim_ac_sowin_libs=
sim_ac_sowin_datadir=
sim_ac_sowin_version=

# internal variables
: ${sim_ac_sowin_desired=true}
sim_ac_sowin_extrapath=

AC_ARG_WITH([sowin], AC_HELP_STRING([--without-sowin], [disable use of SoWin]))
AC_ARG_WITH([sowin], AC_HELP_STRING([--with-sowin], [enable use of SoWin]))
AC_ARG_WITH([sowin],
  AC_HELP_STRING([--with-sowin=DIR], [give prefix location of SoWin]),
  [ case $withval in
    no)  sim_ac_sowin_desired=false ;;
    yes) sim_ac_sowin_desired=true ;;
    *)   sim_ac_sowin_desired=true
         sim_ac_sowin_extrapath=$withval ;;
    esac],
  [])

if $sim_ac_sowin_desired; then
  sim_ac_path=$PATH
  test -z "$sim_ac_sowin_extrapath" ||   ## search in --with-sowin path
    sim_ac_path=$sim_ac_sowin_extrapath/bin:$sim_ac_path
  test x"$prefix" = xNONE ||          ## search in --prefix path
    sim_ac_path=$sim_ac_path:$prefix/bin

  AC_PATH_PROG(sim_ac_sowin_configcmd, sowin-config, false, $sim_ac_path)
  if $sim_ac_sowin_configcmd; then
    sim_ac_sowin_cppflags=`$sim_ac_sowin_configcmd --cppflags`
    sim_ac_sowin_ldflags=`$sim_ac_sowin_configcmd --ldflags`
    sim_ac_sowin_libs=`$sim_ac_sowin_configcmd --libs`
    sim_ac_sowin_datadir=`$sim_ac_sowin_configcmd --datadir`
    sim_ac_sowin_version=`$sim_ac_sowin_configcmd --version`
    AC_CACHE_CHECK(
      [whether libSoWin is available],
      sim_cv_sowin_avail,
      [sim_ac_save_cppflags=$CPPFLAGS
      sim_ac_save_ldflags=$LDFLAGS
      sim_ac_save_libs=$LIBS
      CPPFLAGS="$CPPFLAGS $sim_ac_sowin_cppflags"
      LDFLAGS="$LDFLAGS $sim_ac_sowin_ldflags"
      LIBS="$sim_ac_sowin_libs $LIBS"
      AC_LANG_PUSH(C++)
      AC_TRY_LINK(
        [#include <Inventor/Win/SoWin.h>],
        [(void)SoWin::init((const char *)0L);],
        [sim_cv_sowin_avail=true],
        [sim_cv_sowin_avail=false])
      AC_LANG_POP
      CPPFLAGS=$sim_ac_save_cppflags
      LDFLAGS=$sim_ac_save_ldflags
      LIBS=$sim_ac_save_libs
    ])
    sim_ac_sowin_avail=$sim_cv_sowin_avail
  else
    locations=`IFS=:; for p in $sim_ac_path; do echo " -> $p/sowin-config"; done`
    AC_MSG_WARN([cannot find 'sowin-config' at any of these locations:
$locations])
  fi
fi

if $sim_ac_sowin_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # SIM_AC_HAVE_SOWIN_IFELSE()

