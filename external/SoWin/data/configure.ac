# **************************************************************************
# data/configure.in

AC_PREREQ(2.50)
AC_INIT([SoAnyData], [1.0.0], [coin-support@coin3d.org])

AC_CONFIG_SRCDIR(materials/Makefile.am)
AC_CONFIG_AUX_DIR(cfg)

# **************************************************************************
# Miscellaneous options and initializations.

AM_INIT_AUTOMAKE(data, 1.0.0)

# File to cram results from the configure tests into.
AM_CONFIG_HEADER(config.h)

# Turn off default maintainer make-rules -- use ./bootstrap instead.
AM_MAINTAINER_MODE

# **************************************************************************
# Search and set up necessary stuff to use the GUI library.

files=0
for file in ../so*-*.cfg; do
  files=`expr $files + 1`
done
if test $files -gt 1; then
  rm -f ../so*-*.cfg
  AC_MSG_ERROR([
  The configure script will now abort because the configure directory was
  not clean from configuring/building with multiple --with-alternate options
  without cleaning out the directory in-between.  The problematic duplicate
  config files has been removed.  Please repeat the configure command -
  you will get past this point on the next run.  Sorry for the inconvenience.])
fi

sim_ac_configfile=../so*-*.cfg
if test -f $sim_ac_configfile; then
  . $sim_ac_configfile;
else
  AC_MSG_ERROR([found no config file in ..])
fi

AC_SUBST([GUI], [$AC_SUBST_GUI])
AC_SUBST([Gui], [$AC_SUBST_Gui])
AC_SUBST([gui], [$AC_SUBST_gui])

if test x"${sim_ac_framework+set}" = x"set" && $sim_ac_framework; then
  AM_CONDITIONAL([MAC_FRAMEWORK], [true])
else
  AM_CONDITIONAL([MAC_FRAMEWORK], [false])
fi
AC_SUBST([MAC_FRAMEWORK_NAME])
AC_SUBST([MAC_FRAMEWORK_PREFIX])
AC_SUBST([MAC_FRAMEWORK_VERSION])

# **************************************************************************
# All files configure must automatically generate.

AC_OUTPUT([
	Makefile
	bin/Makefile
	materials/Makefile
	materials/artdeco/Makefile
	materials/autumn/Makefile
	materials/glass/Makefile
	materials/metal/Makefile
	materials/neon/Makefile
	materials/rococo/Makefile
	materials/santafe/Makefile
	materials/sheen/Makefile
	materials/silky/Makefile
	materials/spring/Makefile
	materials/summer/Makefile
	materials/tropical/Makefile
	materials/winter/Makefile
])

# **************************************************************************
