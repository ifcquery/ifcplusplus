############################################################################
##

include ../common/Makefile.common

## Process this file with automake to generate Makefile.in.

if BUILD_WITH_MSVC
noinst_LIBRARIES = widgets.lst
else
noinst_LTLIBRARIES = libSoWinWidgets.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libSoWin build.

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS = 

## Handle built sourcecode files.

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

############################################################################
# libtool setup

libSoWinWidgets_la_SOURCES = \
	SoWinThumbWheel.cpp \
	SoWinThumbWheel.h \
	WinNativePopupMenu.cpp \
	WinNativePopupMenu.h \
	SoWinBitmapButton.cpp \
	SoWinBitmapButton.h

nodist_libSoWinWidgets_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltFiles)
#	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

############################################################################
# custom setup

widgets_lst_SOURCES = \
	SoWinThumbWheel.cpp \
	SoWinThumbWheel.h \
	WinNativePopupMenu.cpp \
	WinNativePopupMenu.h \
	SoWinBitmapButton.cpp \
	SoWinBitmapButton.h

nodist_widgets_lst_SOURCES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltFiles)
#	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

widgets.lst: Makefile $(widgets_lst_OBJECTS)
	@echo "Linking widgets.lst..."; \
	rm -f $@; \
	for i in $(widgets_lst_OBJECTS); do echo $$i >>$@; done

############################################################################
# header installation

SoWinWidgetsincdir = $(includedir)/Inventor/Win/widgets
SoWinWidgetsinc_HEADERS = 
nodist_SoWinWidgetsinc_HEADERS = SoWinPopupMenu.h

install-SoWinWidgetsincHEADERS: $(SoWinWidgetsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(SoWinWidgetsincdir)
	@list='$(SoWinWidgetsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(SoWinWidgetsincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi; \
	done

install-nodist_SoWinWidgetsincHEADERS: $(nodist_SoWinWidgetsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(SoWinWidgetsincdir)
	@list='$(nodist_SoWinWidgetsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(SoWinWidgetsincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi; \
	done

# EOF ######################################################################
