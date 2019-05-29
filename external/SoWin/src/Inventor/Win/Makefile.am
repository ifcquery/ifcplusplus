############################################################################
## So@Gui@/src/Inventor/@Gui@/makefile.am

include common/Makefile.common

############################################################################
##

SUBDIRS = devices editors engines nodes viewers widgets

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_builddir)/data -I$(top_srcdir)/src

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	sowin@SOWIN_MAJOR_VERSION@@SUFFIX@.dll \
	sowin@SOWIN_MAJOR_VERSION@@SUFFIX@.pdb \
	sowin@SOWIN_MAJOR_VERSION@@SUFFIX@.exp \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

############################################################################

if BUILD_WITH_MSVC
lib_LIBRARIES = sowin@SOWIN_MAJOR_VERSION@@SUFFIX@.lib
else
lib_LTLIBRARIES = libSoWin@SUFFIX@.la
endif

if BUILD_IVF
IvfLstFile = ../../Ivf/ivf.lst
else
IvfLstFile =
endif

############################################################################
# libtool setup

libSoWin@SUFFIX@_la_SOURCES = \
	SoWin.cpp \
	SoWinComponent.cpp \
	SoWinComponentP.h \
	SoWinGLWidget.cpp \
	SoWinGLWidgetP.h \
	Win32API.cpp \
	Win32API.h

nodist_libSoWin@SUFFIX@_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles)
#	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

libSoWin@SUFFIX@_la_LIBADD = \
	devices/libSoWinDevices.la \
	editors/libSoWinEditors.la \
	engines/libSoGuiEngines.la \
	nodes/libSoGuiNodes.la \
	viewers/libSoWinViewers.la \
	widgets/libSoWinWidgets.la

libSoWin@SUFFIX@_la_LDFLAGS = \
        -no-undefined \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

############################################################################
# custom setup

sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_SOURCES = \
	SoWin.cpp \
	SoWinComponent.cpp \
	SoWinComponentP.h \
	SoWinGLWidget.cpp \
	SoWinGLWidgetP.h \
	Win32API.cpp \
	Win32API.h

nodist_sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_SOURCES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles)
#	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_LIBADD = \
	devices/devices.lst \
	editors/editors.lst \
	engines/engines.lst \
	nodes/nodes.lst \
	viewers/viewers.lst \
	widgets/widgets.lst \
	$(IvfLstFile)

############################################################################
# header installation

SoWinincdir = $(includedir)/Inventor/Win
SoWininc_HEADERS = \
	SoWinDef.h \
	SoWinRes.h \
	SoWinResource.h \
	SoWinBeginStrict.h \
	SoWinEndStrict.h

nodist_SoWininc_HEADERS = \
	SoWin.h \
	SoWinBasic.h \
	SoWinObject.h \
	SoWinCursor.h \
	SoWinComponent.h \
	SoWinGLWidget.h \
	SoWinRenderArea.h \
	SoWinColorEditor.h \
	SoWinMaterialEditor.h

install-SoWinincHEADERS: $(SoWininc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(SoWinincdir)
	@list='$(SoWininc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(SoWinincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

install-nodist_SoWinincHEADERS: $(nodist_SoWininc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(SoWinincdir)
	@list='$(nodist_SoWininc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(SoWinincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

############################################################################

sowin@SOWIN_MAJOR_VERSION@@SUFFIX@.lib: Makefile \
  $(sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
  $(sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_DEPENDENCIES)
	@basename="sowin@SOWIN_MAJOR_VERSION@@SUFFIX@"; \
	rm -f "$$basename.lib" "$$basename.dll"; \
	  echo "Linking $@..."; \
	  echo $(CC) $(LDFLAGS) $(LIBFLAGS) \
	    $(sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
	    $(sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_LIBADD) $(LIBS); \
	  $(CC) $(LDFLAGS) $(LIBFLAGS) \
	    $(sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
	    $(sowin@SOWIN_MAJOR_VERSION@@SUFFIX@_lib_LIBADD) $(LIBS);

############################################################################

if BUILD_WITH_MSVC
install-exec-local: sowin@SOWIN_MAJOR_VERSION@@SUFFIX@.lib
	@basename="sowin@SOWIN_MAJOR_VERSION@@SUFFIX@"; \
	if @BUILD_WITH_MSVC@; then \
	  for ext in dll pdb; do \
	    file="$$basename.$$ext"; \
	    if test -f "$$file"; then \
	      $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	      echo " $(INSTALL_PROGRAM)" "$$file" "$(DESTDIR)$(bindir)/$$file"; \
	      $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(bindir)/$$file" || exit 1; \
	    fi \
	  done; \
	else :; \
	fi
else
install-exec-local:
endif

install-data-local:
	@basename="sowin@SOWIN_MAJOR_VERSION@@SUFFIX@"; \
	if @BUILD_WITH_MSVC@; then \
	  for ext in lib; do \
	    file="$$basename.$$ext"; \
	    if test -f "$$file"; then \
	      $(mkinstalldirs) "$(DESTDIR)$(libdir)"; \
	      echo " $(INSTALL_DATA)" "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	      $(INSTALL_DATA) "$$file" "$(DESTDIR)$(libdir)/$$file" || exit 1; \
	    fi \
	  done; \
	else :; \
	fi

uninstall-local: uninstall-SoWinincHEADERS uninstall-nodist_SoWinincHEADERS
	@basename="sowin@SOWIN_MAJOR_VERSION@@SUFFIX@"; \
	for ext in dll pdb lib; do \
	  file="$(DESTDIR)$(bindir)/$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    echo " rm -f $$file"; \
	    rm -f "$$file"; \
	  fi \
	done; \
	baseincdir="$(DESTDIR)$(includedir)/Inventor/Win"; \
	for dir in \
	  "$$baseincdir/devices" \
	  "$$baseincdir/editors" \
	  "$$baseincdir/engines" \
	  "$$baseincdir/nodes" \
	  "$$baseincdir/viewers" \
	  "$$baseincdir/widgets" \
	  "$$baseincdir"; do \
	  if test -d "$$dir"; then \
	    echo " rmdir $$dir"; \
	    rmdir "$$dir" || exit 1; \
	  fi \
	done; \
	:

# EOF **********************************************************************
