# ==============================================================================
# GLS CMake File
# ==============================================================================

SET(GLS_LIB_SRC
        gls.c
        )

ADD_DEFINITIONS(-DGLS_BUILD)

# create shared library --------------------------------------------------------

ADD_LIBRARY(gls_shared SHARED ${GLS_LIB_SRC})

TARGET_LINK_LIBRARIES(gls_shared ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY})

SET_TARGET_PROPERTIES(gls_shared PROPERTIES
                        OUTPUT_NAME ${GLS_LIB_NAME}
                        CLEAN_DIRECT_OUTPUT 1
                        VERSION ${GLS_VERSION})
INSTALL(TARGETS
            gls_shared
        DESTINATION
            lib
        )

#-------------------------------------------------------------------------------
