set(GIT_BRANCH "")
set(GIT_SHORT_REV "")
set(GIT_FULL_REV "")

find_program(GIT_SCM git DOC "Git version control")
find_file(GITDIR NAMES .git PATHS ${CMAKE_SOURCE_DIR} NO_DEFAULT_PATH)
if (GIT_SCM AND GITDIR)
    execute_process(
        COMMAND "${GIT_SCM}" branch --show-current
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND "${GIT_SCM}" log -1 "--pretty=format:%h"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_SHORT_REV
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND "${GIT_SCM}" log -1 "--pretty=format:%H"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_FULL_REV
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("Based on GIT ${GIT_FULL_REV} [${GIT_BRANCH}]")
else()
    message("Non GIT build")
endif()

# Force project to reconfigure if the Git index file is touched
set_property(GLOBAL APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/.git/index"
)

configure_file("tools/gitinfo.h.in" "${CMAKE_SOURCE_DIR}/src/utils/gitinfo.h" @ONLY)
