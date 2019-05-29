############################################################################
## SoWin/src/Inventor/Win/nodes/Makefile.am

include ../common/Makefile.common

if BUILD_WITH_MSVC
noinst_LIBRARIES = nodes.lst
else
noinst_LTLIBRARIES = libSoGuiNodes.la
endif

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_nodes_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_nodes_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_nodes_BuiltMetaFiles)

############################################################################

libSoGuiNodes_la_SOURCES =

nodist_libSoGuiNodes_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_nodes_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_nodes_BuiltMetaFiles)

############################################################################

nodes_lst_SOURCES = 

nodist_nodes_lst_SOURCES = \
	$(SoGui_src_Inventor_Gui_nodes_BuiltFiles)
#	$(SoGui_src_Inventor_Gui_nodes_BuiltMetaFiles)

nodes.lst: Makefile $(nodes_lst_OBJECTS)
	@echo "Linking nodes.lst..."; \
	rm -f $@; \
	for i in $(nodes_lst_OBJECTS); do echo $$i >>$@; done

############################################################################
# Headers are kept private.

SoGuiNodesincdir = $(includedir)/Inventor/@Gui@/nodes

nodist_SoGuiNodesinc_HEADERS = \
	SoGuiColorEditor.h \
	SoGuiMaterialEditor.h

############################################################################
