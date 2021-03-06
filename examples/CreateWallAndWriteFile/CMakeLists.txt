CMAKE_MINIMUM_REQUIRED (VERSION 3.7.2)

IF(NOT WIN32)
    IF(NOT CMAKE_BUILD_TYPE)
        SET(CMAKE_BUILD_TYPE "Release")
    ENDIF()

    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
ENDIF(NOT WIN32)

find_package(OpenMP)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/IfcPlusPlus/Debug)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/IfcPlusPlus/${CMAKE_BUILD_TYPE})

ADD_EXECUTABLE(CreateWallAndWriteFile 
    ${IFCPP_SOURCE_DIR}/examples/CreateWallAndWriteFile/src/main.cpp
)

set_target_properties(CreateWallAndWriteFile PROPERTIES DEBUG_POSTFIX "d")

if(OpenMP_CXX_FOUND)
    TARGET_LINK_LIBRARIES(CreateWallAndWriteFile debug IfcPlusPlusd)
	TARGET_LINK_LIBRARIES(CreateWallAndWriteFile optimized IfcPlusPlus OpenMP::OpenMP_CXX)
else(OpenMP_CXX_FOUND)
	TARGET_LINK_LIBRARIES(CreateWallAndWriteFile debug IfcPlusPlusd optimized IfcPlusPlus)
endif()

IF(BUILD_STATIC_LIBRARY)
	ADD_DEFINITIONS(-DIFCQUERY_STATIC_LIB)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(CreateWallAndWriteFile
    PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/external
	${CMAKE_CURRENT_SOURCE_DIR}/src/external/RapidJSON
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src
)

INSTALL(
    TARGETS CreateWallAndWriteFile
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)
