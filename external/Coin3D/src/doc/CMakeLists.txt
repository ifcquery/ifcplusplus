#add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(COIN_BUILD_DOCUMENTATION "Create and install API documentation (requires Doxygen)" ${DOXYGEN_FOUND})
INCLUDE(CMakeDependentOption)
CMAKE_DEPENDENT_OPTION(COIN_BUILD_INTERNAL_DOCUMENTATION "Document internal code not part of the API (depends on COIN_BUILD_DOCUMENTATION)" OFF "COIN_BUILD_DOCUMENTATION" OFF)
file(GLOB_RECURSE DOX *.dox*)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/docs/releases.dox
	${DOX}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

if(COIN_BUILD_DOCUMENTATION)
	if(NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen is needed to build the documentation.")
	endif()
	
	set(GENERATE_HTMLHELP NO)
	if (WIN32)
		set(DOXYGEN_GENERATE_MAN NO)
		find_program(HHC_PROGRAM NAMES hhc.exe PATHS "c:/Program Files/HTML Help Workshop" "c:/Program Files (x86)/HTML Help Workshop" DOC "HTML Help Compiler program")
		if(HHC_PROGRAM)
			option(COIN_BUILD_DOCUMENTATION_CHM "Create compressed HTML help (requires HHC)" OFF)
            mark_as_advanced(HHC_PROGRAM)
		else()
			set(COIN_BUILD_DOCUMENTATION_CHM OFF)
		endif()
		if(COIN_BUILD_DOCUMENTATION_CHM)
			set(GENERATE_HTMLHELP YES)
			set(GENERATE_TREEVIEW NO)
		else()
			set(HHC_PROGRAM)
			set(GENERATE_TREEVIEW YES)
		endif()
	else()
      if (HAVE_MAN STREQUAL "ON")
        set(DOXYGEN_GENERATE_MAN YES)
        install(DIRECTORY ${CMAKE_BINARY_DIR}/man/man3 DESTINATION ${CMAKE_INSTALL_MANDIR} COMPONENT documentation)
        message(STATUS "CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_MANDIR}")
      else()
        set(DOXYGEN_GENERATE_MAN NO)
      endif()
      
      if (COIN_QT_HELP)
        find_program(QHG_LOCATION NAMES qhelpgenerator qhelpgenerator-qt5 DOC "Qt qhelpgenerator")
        if (QHG_LOCATION)
          set(GENERATE_QHP "YES")
        else()
          set(GENERATE_QHP "NO")
          message(FATAL_ERROR "Missing qhelpgenerator")
        endif ()
      endif(COIN_QT_HELP)

      set(GENERATE_TREEVIEW YES)
      set(HHC_PROGRAM)
	endif()

	set(DOXYGEN_INTERNAL_DOCS NO)
	set(DOXYGEN_EXTRACT_PRIVATE NO)
	set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
	set(DOXYGEN_EXCLUDE)
	if(COIN_BUILD_INTERNAL_DOCUMENTATION)
		set(DOXYGEN_EXTRACT_PRIVATE YES)
		set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
		set(DOXYGEN_INTERNAL_DOCS YES)
		set(COIN_DOCUMENTATION_FILES
			${COIN_DOCUMENTATION_FILES}
			${COIN_INTERNAL_DOCUMENTATION_FILES}
			CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
		)
	else()
		set(COIN_DOCUMENTATION_EXCLUDE_SYMBOLS SoProfilerP)
		foreach(XF ${COIN_INTERNAL_DOCUMENTATION_FILES})
			set(DOXYGEN_EXCLUDE "${DOXYGEN_EXCLUDE}                         ${XF} \\\n")
		endforeach(XF)
	endif()
	set(DOXYGEN_INPUT)
	list(REMOVE_DUPLICATES COIN_DOCUMENTATION_FILES)
	foreach(XF ${COIN_DOCUMENTATION_FILES})
		set(DOXYGEN_INPUT "${DOXYGEN_INPUT}                         ${XF} \\\n")
	endforeach(XF)
	set(DOXYGEN_INPUT "${DOXYGEN_INPUT}                         ${CMAKE_SOURCE_DIR}/src/tidbits.cpp")
	set(DOXYGEN_EXCLUDE_SYMBOLS)
	foreach(XF ${COIN_DOCUMENTATION_EXCLUDE_SYMBOLS})
		set(DOXYGEN_EXCLUDE_SYMBOLS "${DOXYGEN_EXCLUDE_SYMBOLS}                         ${XF} \\\n")
	endforeach(XF)
	set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	set(DOXYGEN_OUTPUT ${CMAKE_BINARY_DIR}/html/index.html)
	configure_file(${CMAKE_SOURCE_DIR}/docs/coin.doxygen.cmake.in ${DOXYFILE} @ONLY)
	
	add_custom_command(
		OUTPUT ${DOXYGEN_OUTPUT}
		COMMAND ${CMAKE_COMMAND} -E echo_append "Generating API documentation with Doxygen"
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
		COMMAND ${CMAKE_COMMAND} -E echo "Done."
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		DEPENDS ${DOXYFILE}
	)
	add_custom_target(documentation ALL DEPENDS ${DOXYGEN_OUTPUT})
	install(DIRECTORY ${CMAKE_BINARY_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation)
endif()
