configure_file(config.h.cmake.in config.h)
configure_file(setup.h.cmake.in setup.h)
if(WIN32)
	configure_file(Coin.rc.cmake.in Coin.rc)
endif()

add_definitions(-DCOIN_INTERNAL -DHAVE_CONFIG_H)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCOIN_DEBUG=1)
else()
    add_definitions(-DCOIN_DEBUG=0)
endif()

include_directories(BEFORE ${CMAKE_BINARY_DIR}/data)
include_directories(BEFORE ../include/Inventor/annex)
include_directories(BEFORE .)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
include_directories(BEFORE ../include)
include_directories(BEFORE ${CMAKE_BINARY_DIR}/include)

if(COIN_BUILD_SINGLE_LIB)
	if(WIN32 AND COIN_BUILD_SHARED_LIBS)
		set(COIN_SOURCE tidbits.cpp ${CMAKE_CURRENT_BINARY_DIR}/Coin.rc CACHE INTERNAL "COIN_SOURCE")
	else()
		set(COIN_SOURCE tidbits.cpp CACHE INTERNAL "COIN_SOURCE")
	endif()
else()
	set(COIN_SOURCE)
endif()

set(SUBDIRS
	extensions xml actions base bundles caches details draggers elements
	engines errors events fields fonts glue io manips misc rendering lists nodekits
	navigation nodes shapenodes projectors sensors upgraders
	3ds collision hardcopy shadows geo threads shaders profiler
	vrml97 foreignfiles scxml soscxml
)

foreach(SUBDIR ${SUBDIRS})
    add_subdirectory(${SUBDIR})
endforeach(SUBDIR)
add_subdirectory(doc)

if(COIN_BUILD_SINGLE_LIB)
	if(COIN_BUILD_SHARED_LIBS)
		add_library(Coin SHARED ${COIN_SOURCE})
	else()
		add_library(Coin STATIC ${COIN_SOURCE})
	endif()
else()
	if(COIN_BUILD_SHARED_LIBS)
		if(WIN32 AND COIN_BUILD_SHARED_LIBS)
			set(COIN_SOURCE tidbits.cpp ${CMAKE_CURRENT_BINARY_DIR}/Coin.rc)
		else()
			set(COIN_SOURCE tidbits.cpp)
		endif()
		add_library(Coin SHARED
			$<TARGET_OBJECTS:3ds>
			$<TARGET_OBJECTS:actions>
			$<TARGET_OBJECTS:base>
			$<TARGET_OBJECTS:bundles>
			$<TARGET_OBJECTS:caches>
			$<TARGET_OBJECTS:collision>
			$<TARGET_OBJECTS:details>
			$<TARGET_OBJECTS:draggers>
			$<TARGET_OBJECTS:elements>
			$<TARGET_OBJECTS:engines>
			$<TARGET_OBJECTS:errors>
			$<TARGET_OBJECTS:events>
			$<TARGET_OBJECTS:expat>
			$<TARGET_OBJECTS:fields>
			$<TARGET_OBJECTS:fonts>
			$<TARGET_OBJECTS:foreignfiles>
			$<TARGET_OBJECTS:geo>
			$<TARGET_OBJECTS:glelements>
			$<TARGET_OBJECTS:glue>
			$<TARGET_OBJECTS:hardcopy>
			$<TARGET_OBJECTS:io>
			$<TARGET_OBJECTS:lists>
			$<TARGET_OBJECTS:manips>
			$<TARGET_OBJECTS:misc>
			$<TARGET_OBJECTS:navigation>
			$<TARGET_OBJECTS:nodekits>
			$<TARGET_OBJECTS:nodes>
			$<TARGET_OBJECTS:profiler>
			$<TARGET_OBJECTS:projectors>
			$<TARGET_OBJECTS:rendering>
			$<TARGET_OBJECTS:scxml>
			$<TARGET_OBJECTS:sensors>
			$<TARGET_OBJECTS:shaders>
			$<TARGET_OBJECTS:shadows>
			$<TARGET_OBJECTS:shapenodes>
			$<TARGET_OBJECTS:soscxml>
			$<TARGET_OBJECTS:threads>
			$<TARGET_OBJECTS:upgraders>
			$<TARGET_OBJECTS:vrml97>
			$<TARGET_OBJECTS:xml>
			${COIN_SOURCE}
		)
	else()
		add_library(Coin STATIC
			$<TARGET_OBJECTS:3ds>
			$<TARGET_OBJECTS:actions>
			$<TARGET_OBJECTS:base>
			$<TARGET_OBJECTS:bundles>
			$<TARGET_OBJECTS:caches>
			$<TARGET_OBJECTS:collision>
			$<TARGET_OBJECTS:details>
			$<TARGET_OBJECTS:draggers>
			$<TARGET_OBJECTS:elements>
			$<TARGET_OBJECTS:engines>
			$<TARGET_OBJECTS:errors>
			$<TARGET_OBJECTS:events>
			$<TARGET_OBJECTS:expat>
			$<TARGET_OBJECTS:fields>
			$<TARGET_OBJECTS:fonts>
			$<TARGET_OBJECTS:foreignfiles>
			$<TARGET_OBJECTS:geo>
			$<TARGET_OBJECTS:glelements>
			$<TARGET_OBJECTS:glue>
			$<TARGET_OBJECTS:hardcopy>
			$<TARGET_OBJECTS:io>
			$<TARGET_OBJECTS:lists>
			$<TARGET_OBJECTS:manips>
			$<TARGET_OBJECTS:misc>
			$<TARGET_OBJECTS:navigation>
			$<TARGET_OBJECTS:nodekits>
			$<TARGET_OBJECTS:nodes>
			$<TARGET_OBJECTS:profiler>
			$<TARGET_OBJECTS:projectors>
			$<TARGET_OBJECTS:rendering>
			$<TARGET_OBJECTS:scxml>
			$<TARGET_OBJECTS:sensors>
			$<TARGET_OBJECTS:shaders>
			$<TARGET_OBJECTS:shadows>
			$<TARGET_OBJECTS:shapenodes>
			$<TARGET_OBJECTS:soscxml>
			$<TARGET_OBJECTS:threads>
			$<TARGET_OBJECTS:upgraders>
			$<TARGET_OBJECTS:vrml97>
			$<TARGET_OBJECTS:xml>
			tidbits.cpp
		)
	endif()
endif()
 
if(COIN_BUILD_SHARED_LIBS)
	target_compile_definitions(Coin INTERFACE -DCOIN_DLL)
else()
	target_compile_definitions(Coin INTERFACE -DCOIN_NOT_DLL)
endif()

if(HAVE_LIBSIMAGE)
	target_link_libraries(Coin simage::simage)
endif()

find_package(OpenGL)
target_link_libraries(Coin ${OPENGL_LIBRARIES})
target_include_directories(Coin INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(Coin PROPERTIES VERSION ${COIN_MAJOR_VERSION}.${COIN_MINOR_VERSION}.${COIN_MICRO_VERSION})

install(TARGETS Coin EXPORT Coin
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
)

if(COIN_BUILD_SHARED_LIBS)
	install(TARGETS	Coin EXPORT Coin LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development NAMELINK_ONLY)
endif()

if(MSVC)
	if(COIN_BUILD_SHARED_LIBS)
		if(${CMAKE_MAJOR_VERSION} GREATER 2 AND ${CMAKE_MINOR_VERSION} GREATER 0)
			install(FILES $<TARGET_PDB_FILE:Coin> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
		else()
			install(FILES ${PROJECT_BINARY_DIR}/src/Debug/Coin${CMAKE_DEBUG_POSTFIX}.pdb DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug COMPONENT debug)
			install(FILES ${PROJECT_BINARY_DIR}/src/RelWithDebInfo/Coin${CMAKE_RELWITHDEBINFO_POSTFIX}.pdb DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS RelWithDebInfo COMPONENT debug)
		endif()
	else()
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")                       # Override default /Zi to embed
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Z7")     # debugging info in the .lib.
	endif()
endif()

# Support find_package(Coin NO_MODULE).
set(COIN_DOC_DIR ${CMAKE_INSTALL_DOCDIR})
set(COIN_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(COIN_LIB_DIR ${CMAKE_INSTALL_LIBDIR})
set(COINDIR ${CMAKE_INSTALL_PREFIX})
include(CMakePackageConfigHelpers)
configure_package_config_file(CoinConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/CoinConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Coin-${COIN_VERSION}
                              PATH_VARS COINDIR COIN_INCLUDE_DIR COIN_LIB_DIR COIN_DOC_DIR)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/CoinConfigVersion.cmake"
  VERSION ${COIN_MAJOR_VERSION}.${COIN_MINOR_VERSION}.${COIN_MICRO_VERSION}
  COMPATIBILITY SameMajorVersion
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CoinConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/CoinConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Coin-${COIN_VERSION} COMPONENT development)
install(EXPORT Coin
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Coin-${COIN_VERSION}
        NAMESPACE Coin::
        FILE CoinExport.cmake
        COMPONENT development)

# Suppress warning about empty switch statement in Bison-generated files:
if (MSVC)
	set_property(SOURCE scxml/eval-minimum-tab.cpp APPEND PROPERTY COMPILE_FLAGS /wd4065)
	set_property(SOURCE soscxml/eval-coin-tab.cpp APPEND PROPERTY COMPILE_FLAGS /wd4065)
endif()
