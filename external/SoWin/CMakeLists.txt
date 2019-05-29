cmake_minimum_required(VERSION 2.8.12)

set(gui win)
set(Gui Win)
set(GUI WIN)
set(WIDGET HWND)
set(EVENT MSG*)

project(So${Gui})

include(CheckCXXSourceCompiles)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckStructHasMember)
include(CheckSymbolExists)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(COIN_IV_EXTENSIONS "Enable extra open inventor extensions" ON)
option(SO${GUI}_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(WITH_STATIC_DEFAULTS "Enable defaults from being statically linked in" ON)
option(HAVE_SPACENAV_SUPPORT "Enable Space Navigator support" ON)

find_package(OpenGL REQUIRED)
find_package(Coin REQUIRED CONFIG)
find_package(X11)

set(SO${GUI}_MAJOR_VERSION "1")
set(SO${GUI}_MINOR_VERSION "6")
set(SO${GUI}_MICRO_VERSION "0")
set(SO${GUI}_BETA_VERSION "a")
set(SO${GUI}_VERSION "${SO${GUI}_MAJOR_VERSION}.${SO${GUI}_MINOR_VERSION}.${SO${GUI}_MICRO_VERSION}${SO${GUI}_BETA_VERSION}")

check_include_files(windows.h HAVE_WINDOWS_H)
check_include_files(crtdbg.h HAVE_CRTDBG_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)

set(GUI_TOOLKIT_VERSION "GetOSDisplayString()")
check_symbol_exists(__func__ "" FUNC)
check_symbol_exists(__PRETTY_FUNCTION__ "" PRETTY_FUNCTION)
check_symbol_exists(__FUNCTION__ "" FUNCTION)
if(FUNC)
	set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __func__)
	set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __func__)
elseif(PRETTY_FUNCTION)
	set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __PRETTY_FUNCTION__)
	set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __PRETTY_FUNCTION__)
elseif(FUNCTION)
	set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __FUNCTION__)
	set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __FUNCTION__)
endif()
check_include_files(dlfcn.h HAVE_DLFCN_H)

set(CMAKE_REQUIRED_INCLUDES ${OPENGL_INCLUDE_DIR})
set(CMAKE_REQUIRED_LIBRARIES ${OPENGL_LIBRARIES})

if(HAVE_WINDOWS_H)
	check_include_files("windows.h;GL/glu.h" HAVE_GL_GLU_H)
	check_include_files("windows.h;GL/gl.h" HAVE_GL_GL_H)
else()
	check_include_files(GL/glu.h HAVE_GL_GLU_H)
	check_include_files(GL/gl.h HAVE_GL_GL_H)
endif()
check_include_files(OpenGL/glu.h HAVE_OPENGL_GLU_H)
check_include_files(OpenGL/gl.h HAVE_OPENGL_GL_H)

unset(CMAKE_REQUIRED_INCLUDES)
unset(CMAKE_REQUIRED_LIBRARIES)

check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(netinet/in.h HAVE_NETINET_IN_H)
if(HAVE_SYS_TYPES_H)
	check_cxx_source_compiles("
		#include <sys/types.h>
		#include <pthread.h>
		int main() { struct timespec timeout; timeout.tv_nsec = 0; return 0; }
	" HAVE_PTHREAD_TIMESPEC_NSEC)
else()
	check_cxx_source_compiles("
		#include <pthread.h>
		int main() { struct timespec timeout; timeout.tv_nsec = 0; return 0; }
	" HAVE_PTHREAD_TIMESPEC_NSEC)
endif()

set(CMAKE_REQUIRED_LIBRARIES Coin::Coin)

check_cxx_source_compiles("
	#include <Inventor/SbImage.h>
	int main() { SbImage::addReadImageCB(NULL, NULL); return 0; }
" HAVE_SBIMAGE_ADDREADIMAGECB)
check_cxx_source_compiles("
	#include <Inventor/nodes/SoPerspectiveCamera.h>
	int main() { SoPerspectiveCamera * c = new SoPerspectiveCamera; c->setStereoMode(SoCamera::MONOSCOPIC); return 0; }
" HAVE_SOCAMERA_SETSTEREOMODE)
check_cxx_source_compiles("
	#include <Inventor/misc/SoContextHandler.h>
	int main() { SoContextHandler::destructingContext(0); return 0; }
" HAVE_SOCONTEXTHANDLER)
check_cxx_source_compiles("
	#include <Inventor/events/SoKeyboardEvent.h>
	int main() { SoKeyboardEvent::Key key = SoKeyboardEvent::DELETE; return 0; }
" HAVE_SOKEYBOARDEVENT_DELETE)
check_cxx_source_compiles("
	#include <Inventor/events/SoMouseButtonEvent.h>
	int main() { SoMouseButtonEvent::Button button = SoMouseButtonEvent::BUTTON5; return 0; }
" HAVE_SOMOUSEBUTTONEVENT_BUTTON5)
check_cxx_source_compiles("
	#include <Inventor/nodes/SoPolygonOffset.h>
	int main() { SoPolygonOffset * p = new SoPolygonOffset; return 0; }
" HAVE_SOPOLYGONOFFSET)
check_cxx_source_compiles("
	#include <Inventor/actions/SoGLRenderAction.h>
	int main() { int num = (int) SoGLRenderAction::SORTED_LAYERS_BLEND; return 0; }
" HAVE_SORTED_LAYERS_BLEND)
check_cxx_source_compiles("
	#include <Inventor/VRMLnodes/SoVRMLBackground.h>
	int main() { SoVRMLBackground * p = new SoVRMLBackground; return 0; }
" HAVE_SOVRMLBACKGROUND)
check_cxx_source_compiles("
	#include <Inventor/VRMLnodes/SoVRMLFog.h>
	int main() { SoVRMLFog * p = new SoVRMLFog; return 0; }
" HAVE_SOVRMLFOG)
check_cxx_source_compiles("
	#include <Inventor/VRMLnodes/SoVRMLMaterial.h>
	int main() { SoVRMLMaterial * p = new SoVRMLMaterial; return 0; }
" HAVE_SOVRMLMATERIAL)
check_cxx_source_compiles("
	#include <Inventor/VRMLnodes/SoVRMLViewpoint.h>
	int main() { SoVRMLViewpoint * p = new SoVRMLViewpoint; return 0; }
" HAVE_SOVRMLVIEWPOINT)

unset(CMAKE_REQUIRED_LIBRARIES)

check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/time.h HAVE_SYS_TIME_H)
if(HAVE_WINDOWS_H)
	check_include_files("windows.h;tlhelp32.h" HAVE_TLHELP32_H)
endif()
check_include_files(unistd.h HAVE_UNISTD_H)
check_cxx_source_compiles("
	#include <windows.h>
	int main() {
		CreateDirectory(NULL, NULL);
		RemoveDirectory(NULL);
		SetLastError(0);
		GetLastError();
		LocalAlloc(0, 1);
		LocalFree(NULL);
		return 0;
	}
" HAVE_WIN32_API)
check_symbol_exists(LoadLibrary windows.h HAVE_WIN32_LOADLIBRARY)
check_symbol_exists(GetEnvironmentVariable windows.h HAVE_GETENVIRONMENTVARIABLE)
check_symbol_exists(SetClassLongPtr windows.h HAVE_SETCLASSLONGPTR)
check_symbol_exists(SetWindowLongPtr windows.h HAVE_SETWINDOWLONGPTR)
include(CheckTypeSize)
set(CMAKE_EXTRA_INCLUDE_FILES "windows.h;")
check_type_size(LONG_PTR LONG_PTR_TYPE)
set(CMAKE_EXTRA_INCLUDE_FILES)
set(PACKAGE "So${Gui}")
set(PACKAGE_BUGREPORT "coin-support@coin3d.org")
set(PACKAGE_NAME "So${Gui}")
set(PACKAGE_STRING "So${Gui} ${SO${GUI}_VERSION}")
set(PACKAGE_TARNAME "so${Gui}")
set(PACKAGE_URL "https://bitbucket.org/Coin3D/so${Gui}")
set(PACKAGE_VERSION "${SO${GUI}_VERSION}")
check_include_files("assert.h;ctype.h;errno.h;float.h;limits.h;locale.h;math.h;setjmp.h;signal.h;stdarg.h;stddef.h;stdio.h;stdlib.h;string.h;time.h" STDC_HEADERS)
set(USE_EXCEPTIONS ON)
set(HAVE_DYNAMIC_LINKING 1)

configure_file(src/config.h.cmake.in src/config.h)

add_subdirectory(src/Inventor/${Gui}/common)

file(GLOB_RECURSE COMMON_HDRS ${CMAKE_CURRENT_BINARY_DIR}/src/*.h)
file(GLOB_RECURSE COMMON_SRCS ${CMAKE_CURRENT_BINARY_DIR}/src/*.cpp)

set(
	HDRS
	src/Inventor/${Gui}/So${Gui}Def.h
	src/Inventor/${Gui}/So${Gui}Res.h
	src/Inventor/${Gui}/So${Gui}Resource.h
	src/Inventor/${Gui}/So${Gui}BeginStrict.h
	src/Inventor/${Gui}/So${Gui}EndStrict.h
	src/Inventor/${Gui}/So${Gui}ComponentP.h
	src/Inventor/${Gui}/So${Gui}GLWidgetP.h
	src/Inventor/${Gui}/Win32API.h
	src/Inventor/${Gui}/devices/So${Gui}DeviceP.h
	src/Inventor/${Gui}/devices/So${Gui}SpacenavDevice.h
	src/Inventor/${Gui}/viewers/So${Gui}ExaminerViewerP.h
	src/Inventor/${Gui}/viewers/So${Gui}FullViewerP.h
	src/Inventor/${Gui}/viewers/So${Gui}PlaneViewerP.h
	src/Inventor/${Gui}/viewers/So${Gui}ViewerResource.h
	src/Inventor/${Gui}/widgets/So${Gui}BitmapButton.h
	src/Inventor/${Gui}/widgets/So${Gui}ThumbWheel.h
	src/Inventor/${Gui}/widgets/${Gui}NativePopupMenu.h
)

set(
	SRCS
	src/Inventor/${Gui}/So${Gui}.cpp
	src/Inventor/${Gui}/So${Gui}Component.cpp
	src/Inventor/${Gui}/So${Gui}GLWidget.cpp
	#src/Inventor/${Gui}/So${Gui}MaterialList.cpp
	src/Inventor/${Gui}/Win32API.cpp
	src/Inventor/${Gui}/devices/So${Gui}Device.cpp
	src/Inventor/${Gui}/devices/So${Gui}InputFocus.cpp
	src/Inventor/${Gui}/devices/So${Gui}Keyboard.cpp
	src/Inventor/${Gui}/devices/So${Gui}Mouse.cpp
	src/Inventor/${Gui}/devices/So${Gui}SpacenavDevice.cpp
	src/Inventor/${Gui}/viewers/ExaminerViewer.cpp
	src/Inventor/${Gui}/viewers/FullViewer.cpp
	src/Inventor/${Gui}/viewers/PlaneViewer.cpp
	src/Inventor/${Gui}/widgets/So${Gui}BitmapButton.cpp
	src/Inventor/${Gui}/widgets/So${Gui}ThumbWheel.cpp
	src/Inventor/${Gui}/widgets/${Gui}NativePopupMenu.cpp
)

if(SO${GUI}_BUILD_SHARED_LIBS)
	if(WIN32)
		set(CMAKE_RELEASE_POSTFIX "${SO${GUI}_MAJOR_VERSION}")
		set(CMAKE_MINSIZEREL_POSTFIX "${SO${GUI}_MAJOR_VERSION}")
		set(CMAKE_RELWITHDEBINFO_POSTFIX "${SO${GUI}_MAJOR_VERSION}")
		set(CMAKE_DEBUG_POSTFIX "${SO${GUI}_MAJOR_VERSION}d")
	endif()
	add_library(So${Gui} SHARED ${HDRS} ${COMMON_HDRS} ${SRCS} ${COMMON_SRCS})
	target_compile_definitions(So${Gui} PRIVATE SO${GUI}_MAKE_DLL INTERFACE SO${GUI}_DLL)
else()
	if(WIN32)
		set(CMAKE_RELEASE_POSTFIX "${SO${GUI}_MAJOR_VERSION}s")
		set(CMAKE_MINSIZEREL_POSTFIX "${SO${GUI}_MAJOR_VERSION}s")
		set(CMAKE_RELWITHDEBINFO_POSTFIX "${SO${GUI}_MAJOR_VERSION}s")
		set(CMAKE_DEBUG_POSTFIX "${SO${GUI}_MAJOR_VERSION}sd")
	endif()
	add_library(So${Gui} STATIC ${HDRS} ${COMMON_HDRS} ${SRCS} ${COMMON_SRCS})
	target_compile_definitions(So${Gui} INTERFACE SO${GUI}_NOT_DLL)
endif()

set_target_properties(So${Gui} PROPERTIES VERSION ${SO${GUI}_VERSION})

target_compile_definitions(So${Gui} PRIVATE HAVE_CONFIG_H SO${GUI}_INTERNAL)

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
	target_compile_definitions(So${Gui} PRIVATE SO${GUI}_DEBUG=1)
else()
	target_compile_definitions(So${Gui} PRIVATE SO${GUI}_DEBUG=0)
endif()

target_include_directories(
	So${Gui}
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(So${Gui} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(So${Gui} Coin::Coin ${OPENGL_LIBRARIES})

set(
	INST_HDRS
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}Basic.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}ColorEditor.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}Component.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}Cursor.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}GLWidget.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}MaterialEditor.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}Object.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}RenderArea.h
)

set(
	INST_DEVICES_HDRS
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/devices/So${Gui}Device.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/devices/So${Gui}InputFocus.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/devices/So${Gui}Keyboard.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/devices/So${Gui}Mouse.h
)

set(
	INST_EDITORS_HDRS
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/editors/So${Gui}ColorEditor.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/editors/So${Gui}MaterialEditor.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/devices/So${Gui}SpacenavDevice.h
)

set(
	INST_NODES_HDRS
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/nodes/SoGuiColorEditor.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/nodes/SoGuiMaterialEditor.h
)

set(
	INST_VIEWERS_HDRS
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/viewers/So${Gui}ConstrainedViewer.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/viewers/So${Gui}ExaminerViewer.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/viewers/So${Gui}FlyViewer.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/viewers/So${Gui}FullViewer.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/viewers/So${Gui}PlaneViewer.h
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/viewers/So${Gui}Viewer.h
)

set(
	INST_WIDGETS_HDRS
	${CMAKE_CURRENT_BINARY_DIR}/src/Inventor/${Gui}/widgets/So${Gui}PopupMenu.h
)

install(FILES ${INST_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui} COMPONENT development)
install(FILES ${INST_DEVICES_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/devices COMPONENT development)
install(FILES ${INST_EDITORS_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/editors COMPONENT development)
install(FILES ${INST_NODES_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/nodes COMPONENT development)
install(FILES ${INST_VIEWERS_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/viewers COMPONENT development)
install(FILES ${INST_WIDGETS_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/widgets COMPONENT development)

install(
	TARGETS	So${Gui}
	EXPORT So${Gui}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
)

if(SO${GUI}_BUILD_SHARED_LIBS)
	install(
		TARGETS	So${Gui}
		EXPORT So${Gui}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development NAMELINK_ONLY
	)
endif()

if(MSVC AND SO${GUI}_BUILD_SHARED_LIBS)
	install(FILES $<TARGET_PDB_FILE:So${Gui}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
endif()

configure_package_config_file(
  so${gui}-config.cmake.in so${gui}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/So${Gui}-${SO${GUI}_VERSION}
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/so${gui}-config.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/So${Gui}-${SO${GUI}_VERSION}
	COMPONENT development
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/so${gui}-config-version.cmake
	VERSION ${SO${GUI}_VERSION}
	COMPATIBILITY ExactVersion
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/so${gui}-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/So${Gui}-${SO${GUI}_VERSION}
	COMPONENT development
)

install(
	EXPORT So${Gui}
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/So${Gui}-${SO${GUI}_VERSION}
	NAMESPACE So${Gui}::
	FILE so${gui}-export.cmake
	COMPONENT development
)

set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS runtime)
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_DEPENDS "lib${PACKAGE_TARNAME}-20")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_NAME "lib${PACKAGE_TARNAME}-dev")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${PACKAGE_URL})
set(CPACK_DEBIAN_PACKAGE_NAME "lib${PACKAGE_TARNAME}")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME "lib${PACKAGE_TARNAME}-20")
set(CPACK_DEBIAN_RUNTIME_PACKAGE_SECTION "libs")
set(CPACK_NSIS_PACKAGE_NAME ${PACKAGE_STRING})
set(CPACK_NSIS_URL_INFO_ABOUT ${PACKAGE_URL})
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README")
set(CPACK_PACKAGE_CONTACT ${PACKAGE_BUGREPORT})
set(CPACK_PACKAGE_DISPLAY_NAME ${PACKAGE_STRING})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PACKAGE_TARNAME}-${PACKAGE_VERSION}")
set(CPACK_PACKAGE_NAME ${PACKAGE_TARNAME})
set(CPACK_PACKAGE_VERSION ${SO${GUI}_MAJOR_VERSION}.${SO${GUI}_MINOR_VERSION}.${SO${GUI}_MICRO_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${SO${GUI}_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${SO${GUI}_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${SO${GUI}_MICRO_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/COPYING)
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_development_PACKAGE_NAME "${PACKAGE_TARNAME}-devel")
set(CPACK_RPM_development_PACKAGE_REQUIRES "${PACKAGE_TARNAME}")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_LICENSE "BSD")
set(CPACK_RPM_PACKAGE_NAME ${PACKAGE_TARNAME})
set(CPACK_RPM_PACKAGE_URL ${PACKAGE_URL})
set(CPACK_RPM_runtime_PACKAGE_NAME "${PACKAGE_TARNAME}")
# workaround for erroneous inclusion of system directories
# see https://stackoverflow.com/questions/24983181/cpack-generates-rpms-with-files-entries-that-conflict-with-the-rpm-spec-how-to
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/lib64/cmake")

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
endif()

include(CPack)
