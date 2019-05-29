############################################################################
## SoWin/src/Inventor/Win/editors/Makefile.am

include ../common/Makefile.common

if BUILD_WITH_MSVC
noinst_LIBRARIES = editors.lst
else
noinst_LTLIBRARIES = libSoWinEditors.la
endif

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_editors_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_editors_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_editors_BuiltMetaFiles)

############################################################################

libSoWinEditors_la_SOURCES =

nodist_libSoWinEditors_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_editors_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_editors_BuiltMetaFiles)

############################################################################

editors_lst_SOURCES = 

nodist_editors_lst_SOURCES = \
	$(SoGui_src_Inventor_Gui_editors_BuiltFiles)
#	$(SoGui_src_Inventor_Gui_editors_BuiltMetaFiles)

editors.lst: Makefile $(editors_lst_OBJECTS)
	@echo "Linking editors.lst..."; \
	rm -f $@; \
	for i in $(editors_lst_OBJECTS); do echo $$i >>$@; done

############################################################################

SoWinEditorsincdir = $(includedir)/Inventor/@Gui@/editors

SoWinEditorsinc_HEADERS =

nodist_SoWinEditorsinc_HEADERS = \
	So@Gui@ColorEditor.h \
	So@Gui@MaterialEditor.h

############################################################################
