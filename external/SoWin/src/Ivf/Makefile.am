
DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

noinst_LIBRARIES = ivf.lst

ivf_lst_SOURCES = \
	IvfBasic.h \
	IvfApp.cpp IvfApp.h \
	IvfArchiveInput.cpp IvfArchiveInput.h \
	IvfArchiveOutput.cpp IvfArchiveOutput.h \
	IvfAsyncFile.cpp IvfAsyncFile.h \
	IvfComponent.cpp IvfComponent.h \
	IvfDocument.cpp IvfDocument.h \
	IvfExaminerViewer.cpp Viewers/IvfExaminerViewer.h \
	IvfFlyViewer.cpp Viewers/IvfFlyViewer.h \
	IvfMainFrm.cpp IvfMainFrm.h \
	IvfPlaneViewer.cpp Viewers/IvfPlaneViewer.h \
	IvfRenderArea.cpp Viewers/IvfRenderArea.h \
	IvfResource.cpp IvfResource.h IvfRes.h \
	IvfSceneViewer.cpp Viewers/IvfSceneViewer.h \
	IvfSrvrItem.cpp IvfSrvrItem.h \
	IvfStdViewer.cpp Viewers/IvfStdViewer.h \
	IvfWalkViewer.cpp Viewers/IvfWalkViewer.h \
	SoSceneMenu.cpp Viewers/SoSceneMenu.h \
	SoSceneViewer.cpp Viewers/SoSceneViewer.h \
	SvManipList.cpp Viewers/SvManipList.h

ivf_lst_CPPFLAGS = -DIVF_DLL_API="__declspec(dllexport)"

ivfincdir = $(includedir)/Ivf
ivfinc_HEADERS = \
	IvfBasic.h \
	IvfApp.h \
	IvfArchiveInput.h \
	IvfArchiveOutput.h \
	IvfComponent.h \
	IvfDocument.h \
	IvfMainFrm.h \
	IvfRes.h \
	IvfResource.h \
	IvfSrvrItem.h

ivfviewersincdir = $(includedir)/Ivf/Viewers
ivfviewersinc_HEADERS = \
	Viewers/IvfExaminerViewer.h \
	Viewers/IvfFlyViewer.h \
	Viewers/IvfPlaneViewer.h \
	Viewers/IvfRenderArea.h \
	Viewers/IvfSceneViewer.h \
	Viewers/IvfStdViewer.h \
	Viewers/IvfWalkViewer.h \
	Viewers/SoSceneMenu.h \
	Viewers/SoSceneViewer.h \
	Viewers/SvManipList.h

ivf.lst: Makefile $(ivf_lst_OBJECTS)
	@echo "Linking ivf.lst..."; \
	rm -f $@; \
	for i in $(ivf_lst_OBJECTS); do echo $$i >>$@; done

