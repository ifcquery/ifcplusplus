install(DIRECTORY carve
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
        FILES_MATCHING
          PATTERN "*.hpp"
          PATTERN "internal" EXCLUDE
)
