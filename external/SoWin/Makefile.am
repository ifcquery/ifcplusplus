############################################################################
## Makefile.am

include src/Inventor/Win/common/Makefile.common

############################################################################
##

SUFFIXES = .cpp .h .m4 .in .o .lo .Plo .TPlo

############################################################################
## 

if BUILD_HTMLPAGES
HTMLDIR = html
else
HTMLDIR =
endif

if BUILD_HTMLHELP
HTMLHELPDIR = htmlhelp
else
HTMLHELPDIR =
endif

if BUILD_LIBRARY
# Material editor(s) not part of the public API yet, so there's
# no point yet in installing the material data.
#LIBDIR = data src
LIBDIR = src
else
LIBDIR =
endif

DOCDIRS = man $(HTMLDIR) $(HTMLHELPDIR)

SUBDIRS = . $(LIBDIR) $(DOCDIRS)

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	sowin@SOWIN_MAJOR_VERSION@.dsp \
	sowin@SOWIN_MAJOR_VERSION@.dsw \
	$(SoGui_BuiltMetaFiles)

DISTCLEANFILES = \
	libtool \
	$(SoGui_BuiltFiles) \
	$(SoGui_BuiltMetaFiles)

if BUILD_LIBRARY

bin_SCRIPTS = so@gui@-config

m4datadir = $(datadir)/aclocal
# Don't replace "win" with "@gui@" here, it will pukk up
# for ``make dist''.
dist_m4data_DATA = cfg/sowin.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = so@gui@-@CONFIG@.cfg

endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA= SoWin.pc

EXTRA_DIST = \
	SoWin.pc.in \
	cfg/sowin.m4 \
	cfg/gendsp.pl.in \
	cfg/wrapmsvc.exe \
	cfg/csubst.exe \
	docs/ChangeLog.v1.0.0 \
	docs/ChangeLog.v1.2.0 \
	docs/ChangeLog.v1.3.0 \
	docs/ChangeLog.v1.4.0 \
	docs/ChangeLog.v1.5.0 \
	docs/announcement-1_1_0.txt \
	docs/announcement-1_2_0.txt \
	docs/announcement-1_3_0.txt \
	docs/announcement-1_4_0.txt \
	docs/announcement-1_5_0.txt \
	packaging/windows/Makefile.in \
	packaging/windows/bfheader.nsi.in \
	packaging/windows/footer.nsi.in \
	packaging/windows/heading.nsi.in \
	packaging/windows/middle.nsi.in \
	packaging/windows/post.nsi.in \
	packaging/windows/sowin-sdk.html.in \
	$(SoGuiCommonDistFiles) \
	build/misc/config-wrapper.h \
	build/misc/create-directories.bat \
	build/misc/generate.sh \
	build/misc/install-sdk.bat \
	build/misc/install-headers.bat \
	build/misc/uninstall-headers.bat \
	build/msvc6/src/Inventor/Win/SoWinBasic.h \
	build/msvc6/src/config-debug.h \
	build/msvc6/src/config-release.h \
	build/msvc6/src/config.h \
	build/msvc6/src/sowindefs.h \
	build/msvc6/sowin1.dsw \
	build/msvc6/sowin1.dsp \
	build/msvc6/sowin1_install.dsp \
	build/msvc6/sowin1_uninstall.dsp \
	build/msvc7/src/Inventor/Win/SoWinBasic.h \
	build/msvc7/src/config.h \
	build/msvc7/src/config-debug.h \
	build/msvc7/src/config-release.h \
	build/msvc7/src/sowindefs.h \
	build/msvc7/sowin1.sln \
	build/msvc7/sowin1.vcproj \
	build/msvc7/sowin1_install.vcproj \
	build/msvc7/sowin1_uninstall.vcproj \
	build/msvc8/src/Inventor/Win/SoWinBasic.h \
	build/msvc8/src/config.h \
	build/msvc8/src/config-debug.h \
	build/msvc8/src/config-release.h \
	build/msvc8/src/sowindefs.h \
	build/msvc8/sowin1.sln \
	build/msvc8/sowin1.vcproj \
	build/msvc8/sowin1_install.vcproj \
	build/msvc8/sowin1_uninstall.vcproj

# Target which is present to make it simple to only generate all the source
# files from the common sources.

built-sources: $(BUILT_SOURCES)

############################################################################

all-local: built-sources

install-exec-local: built-sources $(srcdir)/cfg/wrapmsvc.exe
	@$(NORMAL_INSTALL)
	@if @BUILD_WITH_MSVC@; then \
	  echo " $(mkinstalldirs) $(DESTDIR)$(bindir)"; \
	  $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	  p=wrapmsvc.exe; \
	  echo " $(INSTALL_PROGRAM) $(top_srcdir)/cfg/$$p $(DESTDIR)$(bindir)/$$p"; \
	  $(INSTALL_PROGRAM) "$(top_srcdir)/cfg/$$p" "$(DESTDIR)$(bindir)/$$p"; \
	fi

install-data-local: built-sources

uninstall-local:
	@rm -f $(DESTDIR)$(bindir)/wrapmsvc.exe

# **************************************************************************
# misc rules for automatic Microsoft Windows packaging.
# main: `windows-packages'

windows-files:
	@if test -d windows; then :; else \
	  echo " mkdir windows"; \
	  mkdir windows; \
	fi
	@for file in $(top_srcdir)/packaging/windows/*.in; do \
	  basename=`echo $$file | sed 's%^.*/%%g; s%\.in$$%%'`; \
	  echo " config.status: generating windows/$$basename"; \
	  ./config.status --file=-:- >"windows/$$basename" <"$$file"; \
	done

windows-packages: windows-files
	@( cd windows; $(MAKE) VERSION=$(VERSION) )

# EOF ######################################################################
