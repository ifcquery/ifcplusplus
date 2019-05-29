/* src/config.h.  Generated from config.h.in by configure.  */
/* src/config.h.in.  Generated from configure.ac by autoheader.  */


#ifndef SOWIN_INTERNAL
#error this is a private header file
#endif /* !SOWIN_INTERNAL */


/* Version string for Windows */
#define GUI_TOOLKIT_VERSION GetOSDisplayString()

/* The CPP compiler has a variable containing the current function name */
#define HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __FUNCTION__

/* Define to 1 if you have the <crtdbg.h> header file. */
#define HAVE_CRTDBG_H 1

/* The C compiler has a variable containing the current function name */
#define HAVE_C_COMPILER_FUNCTION_NAME_VAR __FUNCTION__

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Define to use GetEnvironmentVariable() instead of getenv() */
#define HAVE_GETENVIRONMENTVARIABLE 1

/* Define to 1 if you have the <GL/glu.h> header file. */
#define HAVE_GL_GLU_H 1

/* Define to 1 if you have the <GL/gl.h> header file. */
#define HAVE_GL_GL_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
/* #undef HAVE_INTTYPES_H */

/* Define if the Ivf API is to be included in the SoWin library */
/* #undef HAVE_IVF */

/* Define if LONG_PTR typedef / define is available */
#define HAVE_LONG_PTR_TYPE 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
/* #undef HAVE_NETINET_IN_H */

/* Define to 1 if you have the <OpenGL/glu.h> header file. */
/* #undef HAVE_OPENGL_GLU_H */

/* Define to 1 if you have the <OpenGL/gl.h> header file. */
/* #undef HAVE_OPENGL_GL_H */

/* Define to use SetClassLongPtr() instead of SetClassLong() */
#define HAVE_SETCLASSLONGPTR 1

/* Define to use SetWindowLongPtr() instead of SetWindowLong() */
#define HAVE_SETWINDOWLONGPTR 1

/* Define to enable use of Inventor feature */
#define HAVE_SOCAMERA_SETSTEREOMODE 1

/* Define to enable use of Inventor feature */
#define HAVE_SOKEYBOARDEVENT_DELETE 1

/* Define to enable use of Inventor feature */
#define HAVE_SOMOUSEBUTTONEVENT_BUTTON5 1

/* Define to enable use of the Open Inventor SoPolygonOffset node */
#define HAVE_SOPOLYGONOFFSET 1

/* Define to enable use of Inventor feature */
#define HAVE_SORTED_LAYERS_BLEND 1

/* Define if the Space Navigator support should be included */
#define HAVE_SPACENAV_SUPPORT 1

/* Define to 1 if you have the <stdint.h> header file. */
/* #undef HAVE_STDINT_H */

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
/* #undef HAVE_SYS_TIME_H */

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <tlhelp32.h> header file. */
#define HAVE_TLHELP32_H 1

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */

/* Define if the Win32 API is available */
#define HAVE_WIN32_API 1

/* Define to use Win32 LoadLibrary() */
#define HAVE_WIN32_LOADLIBRARY 1

/* Define to 1 if you have the <windows.h> header file. */
#define HAVE_WINDOWS_H 1

/* Define to 1 if you have the <X11/extensions/SGIMisc.h> header file. */
/* #undef HAVE_X11_EXTENSIONS_SGIMISC_H */

/* Define to 1 if you have the <X11/extensions/XInput.h> header file. */
/* #undef HAVE_X11_EXTENSIONS_XINPUT_H */

/* Define to 1 if you have the <X11/Xproto.h> header file. */
/* #undef HAVE_X11_XPROTO_H */

/* Name of package */
#define PACKAGE "SoWin"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "coin-bugs@coin3d.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "SoWin"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "SoWin 1.6.0a"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "sowin"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.6.0a"

/* define this to the SoWin beta version letter */
/* #undef SOWIN_BETA_VERSION */

/* Define to the major version of SoWin */
#define SOWIN_MAJOR_VERSION 1

/* Define to the micro version of SoWin */
#define SOWIN_MICRO_VERSION 0

/* Define to the minor version of SoWin */
#define SOWIN_MINOR_VERSION 6

/* Version string for SoWin */
#define SOWIN_VERSION "1.6.0a"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to make Coin use/catch exceptions at some particular places. */
#define USE_EXCEPTIONS 1

/* Version number of package */
#define VERSION "1.6.0a"

/* Define this if you want defaults to be linked into SoXt */
#define WITH_STATIC_DEFAULTS /**/
