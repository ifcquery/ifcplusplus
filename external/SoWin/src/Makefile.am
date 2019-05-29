############################################################################
## So@Gui@/src/Makefile.am

include Inventor/Win/common/Makefile.common

if BUILD_IVF
IvfDir = Ivf
else
IvfDir =
endif

SUBDIRS = . $(IvfDir) Inventor

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_BuiltFiles) \
	$(SoGui_src_BuiltMetaFiles)

if BUILD_IVF
ivfdir = $(includedir)
ivf_HEADERS = \
	IvfEnterScope.h \
	IvfLeaveScope.h
endif

